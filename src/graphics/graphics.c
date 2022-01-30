#ifdef __APPLE__
  #include <GLUT/glut.h>
#else 
  #include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#include "graphics.h"

// framebuffer specifications
static unsigned int fbWidth;
static unsigned int fbHeight;
static const unsigned int colorChannels = 3;
static unsigned long long fbSize;
static const char* fbName;
static double frameTime;
static int framesPassed = 0;
static char windowTitle[256];

// framebuffer
static unsigned char* framebuffer;

// data for thread spawned by init()
static pthread_t loop_id;
static int argCnt;
static char** argList;

// drawing info
static Color drawColor;

// glut functions
static void Display ()
{
    framesPassed++;
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(fbWidth, fbHeight, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
    glFlush();
}

static void Reshape (int width, int height)
{
  glPixelZoom((float)width/fbWidth, (float)height/fbHeight);
}

static void NextFrame(int val)
{
  glutPostRedisplay();
  glutTimerFunc(frameTime, NextFrame, val);
}

static void CalcFps(int val)
{
  sprintf(windowTitle, "%s @ %d FPS", fbName, framesPassed);
  glutSetWindowTitle(windowTitle);
  framesPassed = 0;
  glutTimerFunc(1000, CalcFps, val);
}

// glut initialization
static void* _glutLoopThread(void* param)
{
  // glut initialization
  glutInit(&argCnt,argList);
  glutInitWindowSize(fbWidth, fbHeight);
  glutInitWindowPosition(0,0);
  glutInitDisplayMode(GLUT_RGB);
  glutCreateWindow(fbName);

  // set functions for events
  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);
  if(frameTime>0) NextFrame(0);
  CalcFps(0);

  // run glut window loop
  glutMainLoop();
}

// initalize library
int init(int argc, char* argv[],  const char* name ,const int width, const int height, double framesPerSec)
{
  // framebuffer initialization
  fbWidth = width;
  fbHeight = height;
  fbSize = (long long)width*height*colorChannels;
  framebuffer = (unsigned char*)malloc(fbSize*sizeof(unsigned char)); 
  argCnt = argc;
  argList = argv;
  fbName = name;
  if(!(framesPerSec<=0)) frameTime = (1000/framesPerSec) + 0.5;
  else frameTime = -1;

  // loop thread
  pthread_create(&loop_id, NULL, _glutLoopThread, NULL);
  return 0;
}

int end(void)
{
    glutPostRedisplay();
    pthread_join(loop_id, NULL);
    free(framebuffer);
}

// set draw info
void SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
  drawColor.red = red;
  drawColor.blue = blue;
  drawColor.green = green;
}

Color GetColor(void)
{
  return drawColor;
}

// drawing functions
void PlotPixel(int x, int y)
{
  if(x>=fbWidth || y>=fbHeight) return;
  if(x<0 || y<0) return;
  framebuffer[fbWidth*colorChannels*y + colorChannels*x + 0] = drawColor.red;
  framebuffer[fbWidth*colorChannels*y + colorChannels*x + 1] = drawColor.green;
  framebuffer[fbWidth*colorChannels*y + colorChannels*x + 2] = drawColor.blue;
}

void ClearBuffer(void)
{
    for(int x=0; x<fbWidth; x++)
        for(int y=0; y<fbHeight; y++) PlotPixel(x, y);
}

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
