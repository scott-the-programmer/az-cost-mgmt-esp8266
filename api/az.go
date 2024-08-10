package main

import (
	"context"

	"github.com/Azure/azure-sdk-for-go/sdk/resourcemanager/costmanagement/armcostmanagement"
)

type QueryClient interface {
	Usage(ctx context.Context, scope string, queryDefinition armcostmanagement.QueryDefinition, options *armcostmanagement.QueryClientUsageOptions) (armcostmanagement.QueryClientUsageResponse, error)
}
