#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

typedef struct __Color__
{
    unsigned char red, green, blue;
}Color;

int init(int argc, char* argv[],  const char* name ,const int width, const int height, double framesPerSec);
int end(void);
Color GetColor(void);
void SetColor(unsigned char red, unsigned char green, unsigned char blue);
void PlotPixel(int x, int y);
void ClearBuffer(void);

void PlotLine(int x1, int y1, int x2, int y2);
void PlotToroid(int n, int r1, int r2, int freq, int CenterX, int CenterY);

#endif