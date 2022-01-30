#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

int init(int argc, char* argv[],  const char* name ,const int width, const int height);
int end(void);
void PlotPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue);
void ClearBuffer(unsigned char red, unsigned char green, unsigned char blue);
void PlotLine(int x1, int y1, int x2, int y2, unsigned char red, unsigned char green, unsigned char blue);
void PlotToroid(int n, int r1, int r2, int freq, int CenterX, int CenterY, unsigned int red, unsigned int green, unsigned int blue);

#endif