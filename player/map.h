#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif

class Map
{
public:
 Map(const char *filename,SVGA *scr,int ismap = 0);
 ~Map();
 unsigned int tw; /* The total width of the map */
 unsigned int th;   /* The total height of the map */
 unsigned int x;
 unsigned int y;    /* The coordinates of current viewport(on the bitmap, not on the screen) */
 unsigned int dw;
 unsigned int dh;   /* The dimension to draw */
 //unsigned int sx;   /* The start point when drawing */
 //unsigned int sy;   /* Can't be set automatically */
 unsigned int oldx;
 unsigned int oldy;
 unsigned int transparent;

 EMM xmsbuffer; /* There's no transparent color to the map */
 bitmap *pic;       /* Control class for class Map */
 unsigned int far *access;/* The matrix for ... */
 static unsigned char far *buffer1;
 static unsigned char far *buffer2;

 void Draw();       /* This function will just draw an entire block. x, y is position on the bitmap, not screen */
 void Read();       /* Read the bmp file into XMS */
 void DrawWithTrans(int color = -1);    /* color == -1 means we will fill with color */
 void DrawOnXY();   /* Here the x, y represent coordinate on screen */
 void SetTransparent(int px = 0,int py = 0);   /* Set transparent color. Default to be (0,0) */
 void SetXY(int px,int py);
 void SetDrawWH(int px,int py);
 void SetDrawWH(void);
 void FillWithColor(int color);
 void Shake();
 void UnShake();


private:
 Map(Map &);

};

/* These two buffer are used for drawing whole objects that is not a map */
/* with transparent color. So buffer = 320 is enough                             */
/* And we know that two map will not draw together, so it's safe to share*/
/* the buffer                                                       						 */
unsigned char far *Map::buffer1 = new char[320];
unsigned char far *Map::buffer2 = new char[320];
