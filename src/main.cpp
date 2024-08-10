#include <Arduino.h>
#include "wifi_manager.h"
#include "display_manager.h"
#include "graph.h"
#include "api_client.h"

const char* API_ENDPOINT = "http://your-api-server:8080/cost-data";
const unsigned long API_REFRESH_INTERVAL = 3600000; // 1 hour in milliseconds

unsigned long lastApiCall = 0;
std::vector<float> costData;
ApiClient apiClient(API_ENDPOINT);

void setup() {
  Serial.begin(115200);
  
  initializeDisplay();
  displayConnectingStatus();
  connectToWiFi();
  
  displayWiFiStatus();
  delay(2000);
  
  if (apiClient.fetchCostData(costData)) {
    Serial.println("Initial data fetch successful");
  } else {
    Serial.println("Initial data fetch failed");
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastApiCall >= API_REFRESH_INTERVAL) {
    if (apiClient.fetchCostData(costData)) {
      Serial.println("Data fetch successful");
    } else {
      Serial.println("Data fetch failed");
    }
    lastApiCall = currentTime;
  }
  
  clearDisplay();
  drawGraph(costData);
  updateDisplay();
  delay(100);
}
