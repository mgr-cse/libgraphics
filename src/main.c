#ifdef __APPLE__
  #include <GLUT/glut.h>
#else 
  #include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// constants for some "magic numbers"
unsigned int screenWidth = 500;
unsigned int screenHeight = 500;
unsigned int colorChannels = 3;

// framebuffer
unsigned long long framebufferSize;
unsigned char* framebuffer;

// data structures
typedef struct Point_{
  GLint64 x,y;
}Point;

typedef struct Color_{
  unsigned char red, green, blue;
} Color;


void PlotPixel(Point point, Color color)
{
  if(point.x>=screenWidth || point.y>=screenHeight) return;
  if(point.x<0 || point.y<0) return;
  framebuffer[screenWidth*colorChannels*point.y + colorChannels*point.x + 0] = color.red;
  framebuffer[screenWidth*colorChannels*point.y + colorChannels*point.x + 1] = color.green;
  framebuffer[screenWidth*colorChannels*point.y + colorChannels*point.x + 2] = color.blue;
}

void BresTowardsX(Point p1, Point p2, Color c)
{
  int dx = p2.x - p1.x;
  int dy = p2.y - p1.y;
  int inc = 1;

  if(dy<0){
    inc = -1;
    dy = -dy;
  }
  int diff = (2*dy) - dx;
  GLint64 y = p1.y;

  for(GLint64 x=p1.x; x<=p2.x; x++)
  {
    Point p = {x,y};
    PlotPixel(p, c);
    if(diff>0){
      y += inc;
      diff += 2*(dy-dx);
    }else{
      diff += 2*dy;
    }
  }
}

void BresTowardsY(Point p1, Point p2, Color c)
{
  int dx = p2.x - p1.x;
  int dy = p2.y - p1.y;
  int inc = 1;

  if(dx<0){
    inc = -1;
    dx = -dx;
  }
  int diff = (2*dx) - dy;
  GLint64 x = p1.x;

  for(GLint64 y=p1.y; y<=p2.y; y++)
  {
    Point p = {x,y};
    PlotPixel(p, c);
    if(diff>0){
    x += inc;
      diff += 2*(dx-dy);
    }else{
      diff += 2*dx;
    }
  }  
}

void Bresenham(Point p1, Point p2, Color c)
{
  if(abs(p2.y-p1.y)< abs(p2.x-p1.x)){
    if(p1.x>p2.x) BresTowardsX(p2,p1,c);
    else BresTowardsX(p1,p2,c);
  }else{
    if(p1.y>p2.y) BresTowardsY(p2,p1,c);
    else BresTowardsY(p1,p2,c);
  }
}

void toroid(int n, int r1, int r2, int freq, Point center, Color c)
{
  double angleInc = 2*acos(-1)/n;
  double angle = 0;
  freq += 1;

  Point p1={
    r1*cos(angle) + r2*cos(freq*angle) + center.x + 0.5,
    r1*sin(angle) + r2*sin(freq*angle) + center.y + 0.5
  };

  for(int i=0; i<n; i++)
  {
    Point p2={
      r1*cos(angle+angleInc) + r2*cos(freq*(angle+angleInc)) + center.x + 0.5,
      r1*sin(angle+angleInc) + r2*sin(freq*(angle+angleInc)) + center.y + 0.5
    };
    Bresenham(p1,p2,c);
    p1 = p2;
    angle += angleInc;

  }
}

void Display ()
{
    glClear(GL_COLOR_BUFFER_BIT);
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    glPixelZoom((float)w/500, (float)h/500);
    glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
    glutSwapBuffers();
}

void DrawBuffer(int w, int h)
{
  //glutReshapeWindow(500, 500);

  if(w*h!=0){ 
    screenWidth = w;
    screenHeight = h;
    if(framebuffer) free(framebuffer);
    framebufferSize = w*h*colorChannels;
    framebuffer = (unsigned char*)malloc(framebufferSize*sizeof(unsigned char));
  }

  for(int i=0; i<w; i++)
    for(int j=0; j<h; j++){
      Point p = {i,j};
      Color c = {255,255,255};
      PlotPixel(p,c);
    }
  
  Point p1 = {0,h/2}, p2 = {w-1,h/2};
  Color c = {255,0,0};
  Bresenham(p1,p2,c);

  p1.x = w/2, p1.y = 0;
  p2.x = w/2, p2.y = h-1;
  c.red = 255, c.blue = 0, c.green = 0;
  Bresenham(p1,p2,c);

  c.red = 0, c.green = 0, c.blue = 0;
  Point center ={w/2,h/2};
  toroid(720, w/5, w/5*0.7, 12, center, c);
}

void Timer(int value)
{
  int w = glutGet(GLUT_WINDOW_WIDTH);
  int h = glutGet(GLUT_WINDOW_HEIGHT);
  DrawBuffer(w,h);
  Display();
  glutTimerFunc(300, Timer, value);
}

int main ( int argc, char * argv[] )
{
  // glut initialization
  glutInit(&argc,argv);
  glutInitWindowSize(screenWidth, screenHeight);
  glutInitWindowPosition(0,0);
  glutInitDisplayMode(GLUT_RGB);
  glutCreateWindow("Bresenham!");

  // framebuffer initialization
  framebuffer=NULL;
  DrawBuffer(screenWidth, screenHeight);

  glutDisplayFunc(Display);
  //glutReshapeFunc(DrawBuffer);
  glutMainLoop();
}