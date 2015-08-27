#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif




#ifndef _PLAYER_H

#define _PLAYER_H

#define MAX_PLAYER 4

#define ACTION_DEFENCE 2
#define ACTION_ATTACK 0
#define ACTION_MAGIC 1
#define ACTION_PRODUCT 3
class Map;

class Player
{
public:
 Player(const char *filename,SVGA *pscr);
 ~Player();
 void SetXY(int px,int py);
 void Draw();
 void Read();
 void Move();
 void NextStep();
 /* Inline functions */
 void Stop();
 void GoDown();
 void GoUp();
 void GoLeft();
 void GoRight();
 void Run();   /* Uses external */

 char *GenerateFileName(int type);   /* We can either use the return value or not */

 static unsigned char far *buffer1;
 static unsigned char far *buffer2;
 unsigned char far *buffer3;
 unsigned int x;
 unsigned int y;    /* The coordinates of current viewport */
 unsigned int dx;
 unsigned int dy;
 unsigned int w;
 unsigned int h;
 int tl;                /* The lines of blocks(NOTICE: NOT PIXEL LINES) */
 EMM xmsbuffer; /* There's no transparent color to the map */
 bitmap *pic;       /* Control class for class Map */
 int transparent;  /* Empirically it's the last pixel on the map data file */

 Property *property[MAX_PLAYER];  /* NULL for invalid */
 int type[MAX_PLAYER];
 Map *large_pic[MAX_PLAYER];
 int action[MAX_PLAYER];
 int target[MAX_PLAYER];
 SVGA *scr;
 char filename[16];
 void SetPlayerProperty(int pnumber,Property *p);
 void ClearLargePic();
 void DrawLargePic();
 void InitLargePic();
 void Shake(int i);
 void UnShake(int i);

 int direction;
 int step;
 int buf3valid;
 static int bufvalid;
};

unsigned char far *Player::buffer1 = new unsigned char[32];
unsigned char far *Player::buffer2 = new unsigned char[32];

#endif
