#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <vector>

class ApiClient {
public:
    ApiClient(const char* endpoint);
    bool fetchCostData(std::vector<float>& costData);

private:
    const char* m_endpoint;
    WiFiClient m_wifiClient;
};

#endif // API_CLIENT_H
