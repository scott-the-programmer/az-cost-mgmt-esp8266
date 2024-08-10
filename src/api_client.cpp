#include "api_client.h"

ApiClient::ApiClient(const char* endpoint) : m_endpoint(endpoint) {}

bool ApiClient::fetchCostData(std::vector<float>& costData) {
    HTTPClient http;
    
    if (http.begin(m_wifiClient, m_endpoint)) {  
        int httpResponseCode = http.GET();
        
        if (httpResponseCode == 200) {
            String payload = http.getString();
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, payload);
            
            if (error) {
                Serial.print("JSON parsing failed: ");
                Serial.println(error.c_str());
                http.end();
                return false;
            }
            
            costData.clear();
            for (JsonObject obj : doc.as<JsonArray>()) {
                float cost = obj["totalCost"];
                costData.push_back(cost);
            }
            http.end();
            return true;
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
            http.end();
            return false;
        }
    } else {
        Serial.println("Unable to connect");
        return false;
    }
}
