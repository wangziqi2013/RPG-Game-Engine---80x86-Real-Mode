
#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif


#define WD_DIALOG 0x10
#define WD_BATTLE 0x20
#define WD_SWITCHMAP 0x40


class World
{
public:
 int px;            /* x coordinate for player   */
 int py;       /* y coordinate for player   */
						  /* Notice that these two nuber are the relative coordinate */
						  /* The absolute coordinate is px + mx, py + my */
 int mx;       /* x coordinate for map    */
 int my;       /* y coordinate for map    *//* They are virtual view port coordinates */
 int mw;       /* width coordinate for map  */
 int mh;       /* height coordinate for map */

 Player *player;
 Map *map;
 Timer *timer;
 SVGA *scr;
 Keyboard *keyboard;
 FlyBack *flyback;

 World(Player *,Map *,Timer *,SVGA *,Keyboard *,FlyBack *);
 ~World();
 void Init();
 void Go(unsigned char key);
 void GoLeft();
 void GoRight();
 void GoUp();
 void GoDown();
 void Move();
 void KeepMove();
 void SetStartPoint(int psx,int psy);
 void CheckAccess();
 void ShowDialog(int);
 void MakeBattle(int);
 void SwitchMap(int);
};



