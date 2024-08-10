#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int GRAPH_HEIGHT = 40;
const int GRAPH_Y_OFFSET = 10;
const int BAR_WIDTH = 16;
const int BAR_GAP = 2;

void drawGraph(const std::vector<float>& costData);

#endif // GRAPH_H
