#include "render/render.h"

int main(int argc, char* argv[])
{
  init(argc, argv, "hello!", 1280, 720);
  ClearBuffer(255,255,255);
  PlotToroid(360, 200, 140, 12, 1280/2, 720/2, 0,0,0);
  PlotLine(0,0,1279,719, 0,0,0);
  end();
}