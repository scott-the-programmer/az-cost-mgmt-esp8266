package main

import (
	"sync"
	"time"
)

type CostDataCache struct {
	data       []CostData
	expiration time.Time
	mutex      sync.RWMutex
}

func (c *CostDataCache) Set(data []CostData) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	c.data = data
	c.expiration = time.Now().Add(1 * time.Hour)
}

func (c *CostDataCache) Get() ([]CostData, bool) {
	c.mutex.RLock()
	defer c.mutex.RUnlock()
	if time.Now().Before(c.expiration) {
		return c.data, true
	}
	return nil, false
}
