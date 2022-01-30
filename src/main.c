#include "graphics/graphics.h"

struct point{
  char g;
};

struct point p;

int main(int argc, char* argv[])
{
  init(argc, argv, "hello!", 1280, 720);
  ClearBuffer(255,255,255);
  PlotToroid(360, 200, 140, 12, 1280/2, 720/2, 0,0,0);
  PlotLine(0,0,1279,719, 0,0,0);
  PlotLine(1279, 719, 600, 600, 0, 0, 0);
  PlotLine(600, 600, 0, 0, 0,0,0);
  end();
}