#include "graph.h"
#include "display_manager.h"
#include <Arduino.h>

void drawGraph(const std::vector<float>& costData) {
    u8g2.drawLine(0, SCREEN_HEIGHT - GRAPH_Y_OFFSET, SCREEN_WIDTH, SCREEN_HEIGHT - GRAPH_Y_OFFSET);
    u8g2.drawLine(0, GRAPH_Y_OFFSET, 0, SCREEN_HEIGHT - GRAPH_Y_OFFSET);

    float maxCost = 0;
    for (float cost : costData) {
        if (cost > maxCost) maxCost = cost;
    }

    int x = 2; 
    for (size_t i = 0; i < costData.size() && x < SCREEN_WIDTH; ++i) {
        int barHeight = (int)((costData[i] / maxCost) * GRAPH_HEIGHT);
        u8g2.drawBox(x, SCREEN_HEIGHT - GRAPH_Y_OFFSET - barHeight, BAR_WIDTH, barHeight);
        x += BAR_WIDTH + BAR_GAP;
    }

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(2, 8, "Daily Cost (NZD)");
    
    char maxCostStr[10];
    snprintf(maxCostStr, sizeof(maxCostStr), "%.0f", maxCost);
    u8g2.drawStr(2, GRAPH_Y_OFFSET + 8, maxCostStr);

    u8g2.drawStr(2, SCREEN_HEIGHT - 1, "Time");
}
