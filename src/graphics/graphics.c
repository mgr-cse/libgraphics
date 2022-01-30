#ifdef __APPLE__
  #include <GLUT/glut.h>
#else 
  #include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
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

// get draw info
Color GetColor(void)
{
  return drawColor;
}

Color GetPixelColor(int x, int y)
{
  Color color = {0, 0, 0};
  if(x>=fbWidth || y>=fbHeight) return color;
  if(x<0 || y<0) return color;
  unsigned long long pixelPos = colorChannels*(fbWidth*y + x);
  color.red   = framebuffer[pixelPos + 0];
  color.green = framebuffer[pixelPos + 1];
  color.blue  = framebuffer[pixelPos + 2];
  return color;

}

// set draw info
void SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
  drawColor.red = red;
  drawColor.blue = blue;
  drawColor.green = green;
}

// drawing functions
void PlotPixel(int x, int y)
{
  if(x>=fbWidth || y>=fbHeight) return;
  if(x<0 || y<0) return;
  unsigned long long pixelPos = colorChannels*(fbWidth*y + x);
  framebuffer[pixelPos + 0] = drawColor.red;
  framebuffer[pixelPos + 1] = drawColor.green;
  framebuffer[pixelPos + 2] = drawColor.blue;
}

void ClearBuffer(void)
{
    for(int x=0; x<fbWidth; x++)
        for(int y=0; y<fbHeight; y++) PlotPixel(x, y);
}

