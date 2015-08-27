#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif


#ifndef _MONSTER_H
#define _MONSTER_H

#define MAXMONSTER 5
#define MONSTER_NORMAL 0
#define MONSTER_DEAD 1
#define MONSTER_PARALYZED 2
#define MONSTER_DIZZY 3
#define MONSTER_DAZED 4
#define MONSTER_POISION 5
#define MONSTER_SELECTED (0x80)
#define ISSELECTED(x) ((x) & 0x80)
#define SELECT(x) ((x) |= 0x80)
#define UNSELECT(x) ((x) &= ~0x80)
#define WITHOUTSELECT(x) ((x) & ~0x80)
#define ISDEAD(x) (x == MONSTER_DEAD)


class Monster
{
public:
 int firstdraw;
 int num;
 int type[MAXMONSTER];		/* if type == -1 then no matter what type it is */
 int status[MAXMONSTER];
 Map *map[MAXMONSTER];
 char filename[16];
 Property property[MAXMONSTER];
 SVGA *scr;

 Monster(int monsternum,SVGA *pscr);
 Monster(int monsternum,World *pworld);
 Monster(int monsternum,World *pworld,int = 0,int = 0,int = 0,int = 0,int = 0);
 void SetBasic(int monsternum,SVGA *pscr);
 ~Monster();
 void GenerateName(int type);
 char *GenerateFileName(int type);
 void Draw();
 void Read();
 void SetMonsterType(int which,int type);
 void Shake(int i);
 void UnShake(int i);
};

#endif
