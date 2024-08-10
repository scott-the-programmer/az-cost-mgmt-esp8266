package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"strconv"
	"time"

	"github.com/Azure/azure-sdk-for-go/sdk/azcore/to"
	"github.com/Azure/azure-sdk-for-go/sdk/azidentity"
	"github.com/Azure/azure-sdk-for-go/sdk/resourcemanager/costmanagement/armcostmanagement"
)

var (
	subscriptionID string
	client         QueryClient
	cache          *CostDataCache
)

type CostData struct {
	TotalCost float64 `json:"totalCost"`
	Date      string  `json:"date"`
	Currency  string  `json:"currency"`
}

func init() {
	cred, err := azidentity.NewDefaultAzureCredential(nil)
	if err != nil {
		log.Fatalf("Failed to create credential: %v", err)
	}

	azureClient, err := armcostmanagement.NewQueryClient(cred, nil)
	if err != nil {
		log.Fatalf("Failed to create client: %v", err)
	}
	client = azureClient
	cache = &CostDataCache{}
}

func getCostData() ([]CostData, error) {
	subscriptionID = os.Getenv("AZURE_SUBSCRIPTION_ID")
	if subscriptionID == "" {
		log.Fatal("AZURE_SUBSCRIPTION_ID environment variable is not set")
	}

	if cachedData, found := cache.Get(); found {
		return cachedData, nil
	}

	now := time.Now().UTC()
	startDate := now.AddDate(0, 0, -7).Format("2006-01-02")
	endDate := now.Format("2006-01-02")

	startTime, _ := time.Parse("2006-01-02", startDate)
	endTime, _ := time.Parse("2006-01-02", endDate)

	scope := fmt.Sprintf("/subscriptions/%s", subscriptionID)
	queryDefinition := armcostmanagement.QueryDefinition{
		Type:      to.Ptr(armcostmanagement.ExportTypeActualCost),
		Timeframe: to.Ptr(armcostmanagement.TimeframeTypeCustom),
		TimePeriod: &armcostmanagement.QueryTimePeriod{
			From: &startTime,
			To:   &endTime,
		},
		Dataset: &armcostmanagement.QueryDataset{
			Granularity: to.Ptr(armcostmanagement.GranularityTypeDaily),
			Aggregation: map[string]*armcostmanagement.QueryAggregation{
				"totalCost": {
					Name:     to.Ptr("Cost"),
					Function: to.Ptr(armcostmanagement.FunctionTypeSum),
				},
			},
		},
	}

	result, err := client.Usage(context.Background(), scope, queryDefinition, nil)
	if err != nil {
		return nil, fmt.Errorf("failed to get cost data: %v", err)
	}

	var costData []CostData
	for _, row := range result.Properties.Rows {
		if len(row) >= 3 {
			cost, _ := row[0].(float64)
			datefl, _ := row[1].(float64)
			currency, _ := row[2].(string)
			datestr := strconv.Itoa(int(datefl))
			date := datestr[:4] + "-" + datestr[4:6] + "-" + datestr[6:]
			costData = append(costData, CostData{
				TotalCost: cost,
				Date:      date,
				Currency:  currency,
			})
		}
	}

	cache.Set(costData)

	return costData, nil
}

func handleCostData(w http.ResponseWriter, r *http.Request) {
	println("Handling cost request")
	data, err := getCostData()
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(data)
}

func main() {
	http.HandleFunc("/cost-data", handleCostData)
	log.Printf("Server starting on :8080")
	log.Fatal(http.ListenAndServe(":8080", nil))
}
