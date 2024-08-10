#include "wifi_manager.h"
#include "display_manager.h"

const char* ssid = "";
const char* password = "";

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  int dotCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    clearDisplay();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 20, "Connecting to WiFi");
    
    String dots = "";
    for (int i = 0; i <= dotCount; i++) {
      dots += ".";
    }
    u8g2.drawStr(0, 40, dots.c_str());
    updateDisplay();
    
    dotCount = (dotCount + 1) % 3; 
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String getIPAddress() {
  return WiFi.localIP().toString();
}
