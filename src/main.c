#include <unistd.h>
#include "graphics/graphics.h"

int main(int argc, char* argv[])
{
	int w = 1280, h = 720;
	init(argc, argv, "hello!", w, h, 10.0);

	SetColor(255,255,255);
		ClearBuffer();
	
	SetColor(0, 0, 0);
		PlotToroid(360, 200, 140, 12, w/2, h/2);
		PlotCircle(200, w/2, h/2);
		sleep(2);
		PlotLine(0, 0, w-1, h-1);
		sleep(2);
		PlotLine(w-1, h-1, 600, 600);
		sleep(2);
		PlotLine(600, 600, 0, 0);
	end();
}