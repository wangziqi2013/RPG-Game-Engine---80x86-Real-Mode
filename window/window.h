#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif




#ifndef _WINDOW_H
#define _WINDOW_H

#define WINDOW_XMS 308
#define WINDOW_CENTER 1
#define WINDOW_LEFT 2
#define WINDOW_RIGHT 3


/* For a Window class we guarantee the 640*480 bitmap resident in main memory */
/* so if we want to draw we only need to scratch a corner from the left top   */
class Window
{
public:
 Window(const char *filename,SVGA *scr);
 Window(const char *filename,World *pworld);

 ~Window();

 int x;     /* Position on the screen */
 int y;     /* Position on the screen */
 int numtable;	/* Number table for generating number strings */

 void SetXY(int px,int py);

 void DrawBack(int x,int y,int dw,int dh);
 void DrawBack(int dw,int dh);
 void DrawGrid(int x,int y,int dw,int dh);  /* In this function the parameter are all block, not pixel */
 /* So we cannot use current dw, dh to draw a window on the blocks */
 void DrawArrow(int rx,int ry); /* This function uses relative coordinates */
 void Print(int pcolor,int pnum,int pline,int palign);  /* Must drawback first */
 void PrintProperty(Property *p,int pcolor,int pnum,int pline,int ismonster = 0);
 void NumtoStr(int num,char str[]);
 void GenerateProperty(int pcolor,int pproperty,int pnum,int pline);
 void PrintNumber(int pline,int pcolor,int pnum);	/* Relative, Grid !!!!! */

private:
 int dw;    /* The dw and dh of static Map *back are not valid */
 int dh;
 static int firstdraw;
 static Map *back;
 static Map *arrow;

 CnString *cnstring;
 SVGA *scr;
 void SetBasic(const char *,SVGA *);

 Window(Window &);
};

int Window::firstdraw = 1;
Map *Window::back = NULL;
Map *Window::arrow = NULL;



#endif
