#include "render/render.h"

int main(int argc, char* argv[])
{
  init(argc, argv, "hello!", 500, 500);
  ClearBuffer(255,255,255);
  PlotToroid(360, 100, 70, 12, 250, 250, 0,0,0);
  PlotLine(0,0,499,499, 0,0,0);
  end();
}