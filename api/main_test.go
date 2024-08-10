package main

import (
	"context"
	"os"
	"testing"

	"github.com/Azure/azure-sdk-for-go/sdk/resourcemanager/costmanagement/armcostmanagement"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
)

// MockQueryClient is a mock for the Azure QueryClient
type MockQueryClient struct {
	mock.Mock
}

func (m *MockQueryClient) Usage(ctx context.Context, scope string, queryDefinition armcostmanagement.QueryDefinition, options *armcostmanagement.QueryClientUsageOptions) (armcostmanagement.QueryClientUsageResponse, error) {
	args := m.Called(ctx, scope, queryDefinition, options)
	return args.Get(0).(armcostmanagement.QueryClientUsageResponse), args.Error(1)
}

func TestGetCostData(t *testing.T) {
	// Setup
	os.Setenv("AZURE_SUBSCRIPTION_ID", "12345678-1234-1234-1234-123456789012")

	mockClient := new(MockQueryClient)

	client = mockClient

	// Mock the response
	mockResponse := armcostmanagement.QueryClientUsageResponse{
		QueryResult: armcostmanagement.QueryResult{
			Properties: &armcostmanagement.QueryProperties{
				Rows: [][]any{
					{100.0, 20230601.0, "USD"},
					{200.0, 20230602.0, "USD"},
				},
			},
		},
	}

	mockClient.On("Usage", mock.Anything, mock.Anything, mock.Anything, mock.Anything).Return(mockResponse, nil)

	mockClient.On("Usage", mock.Anything, mock.Anything, mock.Anything, mock.Anything).Return(mockResponse, nil)

	// Test
	data, err := getCostData()

	// Assert
	assert.NoError(t, err)
	assert.Len(t, data, 2)
	assert.Equal(t, 100.0, data[0].TotalCost)
	assert.Equal(t, "2023-06-01", data[0].Date)
	assert.Equal(t, "USD", data[0].Currency)
	assert.Equal(t, 200.0, data[1].TotalCost)
	assert.Equal(t, "2023-06-02", data[1].Date)
	assert.Equal(t, "USD", data[1].Currency)

	mockClient.AssertExpectations(t)
}
