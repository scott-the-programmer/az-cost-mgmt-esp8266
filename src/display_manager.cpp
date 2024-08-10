#include "display_manager.h"
#include "wifi_manager.h"

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14, /* reset=*/ U8X8_PIN_NONE);

void initializeDisplay() {
  u8g2.begin();
}

void clearDisplay() {
  u8g2.clearBuffer();
}

void updateDisplay() {
  u8g2.sendBuffer();
}

void displayConnectingStatus() {
  clearDisplay();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 20, "Connecting to WiFi");
  u8g2.drawStr(0, 40, "Please wait...");
  updateDisplay();
}

void displayWiFiStatus() {
  clearDisplay();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "WiFi connected");
  u8g2.drawStr(0, 20, "IP:");
  u8g2.drawStr(0, 30, getIPAddress().c_str());
  updateDisplay();
}
