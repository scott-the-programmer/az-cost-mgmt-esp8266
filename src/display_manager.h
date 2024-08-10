#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;

void initializeDisplay();
void clearDisplay();
void updateDisplay();
void displayConnectingStatus();
void displayWiFiStatus();

#endif
