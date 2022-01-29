#ifdef __APPLE__
  #include <GLUT/glut.h>
#else 
  #include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// constants for some "magic numbers"
static unsigned int fbWidth;
static unsigned int fbHeight;
static const unsigned int colorChannels = 3;

// framebuffer
static unsigned long long fbSize;
static unsigned char* framebuffer;
static const char* fbName;

static pthread_t loop_id;

static int argCnt;
static char** argList;

// glut initialization
static void Display ()
{
    glClear(GL_COLOR_BUFFER_BIT);
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    glPixelZoom((float)w/fbWidth, (float)h/fbHeight);
    glDrawPixels(fbWidth, fbHeight, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
    glFlush();
}

static void* _glutLoopThread(void* param)
{
  // glut initialization
  glutInit(&argCnt,argList);
  glutInitWindowSize(fbWidth, fbHeight);
  glutInitWindowPosition(0,0);
  glutInitDisplayMode(GLUT_RGB);
  glutCreateWindow(fbName);
  glutDisplayFunc(Display);
  glutMainLoop();
}

int init(int argc, char* argv[],  const char* name ,const int width, const int height)
{
  // framebuffer initialization
  fbWidth = width;
  fbHeight = height;
  fbSize = (long long)width*height*colorChannels;
  framebuffer = (unsigned char*)malloc(fbSize*sizeof(unsigned char)); 
  argCnt = argc;
  argList = argv;
  fbName = name;

  // loop thread
  pthread_create(&loop_id, NULL, _glutLoopThread, NULL);
  return 0;
}

int end(void)
{
    pthread_join(loop_id, NULL);
    free(framebuffer);
}

// drawing functions
void PlotPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue)
{
  if(x>=fbWidth || y>=fbHeight) return;
  if(x<0 || y<0) return;
  framebuffer[fbWidth*colorChannels*y + colorChannels*x + 0] = red;
  framebuffer[fbWidth*colorChannels*y + colorChannels*x + 1] = green;
  framebuffer[fbWidth*colorChannels*y + colorChannels*x + 2] = blue;
}

void ClearBuffer(unsigned char red, unsigned char green, unsigned char blue)
{
    for(int x=0; x<fbWidth; x++)
        for(int y=0; y<fbHeight; y++) PlotPixel(x, y, red, green, blue);
}

static void BresTowardsX(int x1, int y1, int x2, int y2, unsigned char red, unsigned char green, unsigned char blue)
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

  for(GLint64 x=x1; x<=x2; x++)
  {
    PlotPixel(x, y, red, green, blue);
    if(diff>0){
      y += inc;
      diff += 2*(dy-dx);
    }else{
      diff += 2*dy;
    }
  }
}

static void BresTowardsY(int x1, int y1, int x2, int y2, unsigned char red, unsigned char green, unsigned char blue)
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int inc = 1;

  if(dx<0){
    inc = -1;
    dx = -dx;
  }
  int diff = (2*dx) - dy;
  GLint64 x = x1;

  for(GLint64 y=y1; y<=y2; y++)
  {
    PlotPixel(x, y, red, green, blue);
    if(diff>0){
    x += inc;
      diff += 2*(dx-dy);
    }else{
      diff += 2*dx;
    }
  }  
}

void PlotLine(int x1, int y1, int x2, int y2, unsigned char red, unsigned char green, unsigned char blue)
{
  if(abs(y2-y1)< abs(x2-x1)){
    if(x1>x2) BresTowardsX(x2, y2, x1, y1, red, green, blue);
    else BresTowardsX(x1, y1, x2, y2, red, green, blue);
  }else{
    if(y1>y2) BresTowardsY(x2, y2, x1, y1, red, green, blue);
    else BresTowardsY(x1, y1, x2, y2, red, green, blue);
  }
}

void PlotToroid(int n, int r1, int r2, int freq, int CenterX, int CenterY, unsigned int red, unsigned int green, unsigned int blue)
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
    PlotLine(x1, y1, x2, y2, red, green, blue);
    x1 = x2;
    y1 = y2;
  }
}
