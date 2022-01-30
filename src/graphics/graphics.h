#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

// data structures
typedef struct __Color__
{
	unsigned char red, green, blue;
}Color;

// primitive library functions
int init(int argc, char* argv[],  const char* name ,const int width, const int height, double framesPerSec);
int end(void);

Color GetColor(void);
Color GetPixelColor(int x, int y);

void SetColor(unsigned char red, unsigned char green, unsigned char blue);

void PlotPixel(int x, int y);
void ClearBuffer(void);

// extras
void PlotLine(int x1, int y1, int x2, int y2);
void PlotToroid(int n, int r1, int r2, int freq, int CenterX, int CenterY);
void PlotCircle(int radius, int centerX, int centerY);

#endif