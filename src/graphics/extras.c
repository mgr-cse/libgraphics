#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "graphics.h"

//***************LINE DRAWING***************//
static void BresTowardsX(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int inc = 1;

	if(dy<0){
		inc = -1;
		dy = -dy;
	}
	int diff = (2*dy) - dx;
	int y = y1;

	for(int x=x1; x<=x2; x++)
	{
		PlotPixel(x, y);
		if(diff>0){
			y += inc;
			diff += 2*(dy-dx);
		}else{
			diff += 2*dy;
		}
	}
}

static void BresTowardsY(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int inc = 1;

	if(dx<0){
		inc = -1;
		dx = -dx;
	}
	int diff = (2*dx) - dy;
	int x = x1;

	for(int y=y1; y<=y2; y++)
	{
		PlotPixel(x, y);
		if(diff>0){
		x += inc;
			diff += 2*(dx-dy);
		}else{
			diff += 2*dx;
		}
	}  
}

void PlotLine(int x1, int y1, int x2, int y2)
{
	if(abs(y2-y1)< abs(x2-x1)){
		if(x1>x2) BresTowardsX(x2, y2, x1, y1);
		else BresTowardsX(x1, y1, x2, y2);
	}else{
		if(y1>y2) BresTowardsY(x2, y2, x1, y1);
		else BresTowardsY(x1, y1, x2, y2);
	}
}

//*****************TOROID******************//
void PlotToroid(int n, int r1, int r2, int freq, int CenterX, int CenterY)
{
	double angleInc = 2*acos(-1)/n;
	double angle = 0;
	freq += 1;

	int x1 = r1*cos(angle) + r2*cos(freq*angle) + CenterX + 0.5;
	int y1 = r1*sin(angle) + r2*sin(freq*angle) + CenterY + 0.5;

	for(int i=0; i<n; i++)
	{
		angle += angleInc;
		int x2 = r1*cos(angle) + r2*cos(freq*angle) + CenterX + 0.5;
		int y2 = r1*sin(angle) + r2*sin(freq*angle) + CenterY + 0.5;
		PlotLine(x1, y1, x2, y2);
		x1 = x2;
		y1 = y2;
	}
}

//*****************CIRCLE******************//
static void PlotCircleSymetric(int centerX, int centerY, int x, int y)
{
	PlotPixel(centerX+x, centerY+y);
	PlotPixel(centerX-x, centerY+y);
	PlotPixel(centerX-x, centerY-y);
	PlotPixel(centerX+x, centerY-y);

	PlotPixel(centerX+y, centerY+x);
	PlotPixel(centerX-y, centerY+x);
	PlotPixel(centerX-y, centerY-x);
	PlotPixel(centerX+y, centerY-x);
}

void PlotCircle(int radius, int centerX, int centerY)
{
	int x = 0, y = radius;
	int diff = 3 - 2*radius;
	PlotCircleSymetric(centerX, centerY, x, y);
	while (y>=x)
	{
		x++;
		if(diff>0)
		{
			y--;
			diff += 4*(x-y) + 10;
		}
		else diff += 4*x + 6;
		PlotCircleSymetric(centerX, centerY, x, y);
	}
} 