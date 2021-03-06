#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif



#ifndef _BATTLE_H

#define _BATTLE_H

#define BATTLE_XMSSIZE 210

#define BATTLE_BEGIN 0
#define BATTLE_BEGIN_ACTION 1
#define BATTLE_PLCHOOSE_1 2
#define BATTLE_PLCHOOSE_2 3
#define BATTLE_PLCHOOSE_3 4
#define BATTLE_PLCHOOSE_4 5
#define BATTLE_READY 6
#define BATTLE_FINISH 7

#define BATTLE_LEAVE 8
#define BATTLE_MTCHOOSE 9
#define BATTLE_FAIL 10


class Monster;

class Battle
{
public:
 Battle(Player *,Map *,Timer *,SVGA *,Keyboard *,FlyBack *,Monster *);
 Battle(Map *pmap,Monster *pmonster,World *pworld);
 ~Battle();
 void SetBasic(Player *,Map *,Timer *,SVGA *,Keyboard *,FlyBack *,Monster *);
 void Init();
 void DrawBackground();
 void DrawMonster();
 void DrawPlayer();
 //void DrawAttack();
 void Fight();
 void SelectBeginAction();
 void PlayerChooseAction(int pnum);
 void PlayerChooseMonster(int pnum);
 void BeginBattle();
 void WaitforSpace();
 void Wait(int ptime);
 void WaitTime(int ptime);
 int TimeOut();
 void PlayerAttack(int i);
 void MonsterAttack(int i);
 void Judge();
 void ShakeMonster(Monster *m,int pnum,int i);
 void ShakePlayer(Player *p,int pnum,int i);
 void Fail();

 Map *back1;
 Map *back2;
 Monster *mon;
 Player *player;
 Timer *timer;
 Keyboard *keyboard;
 SVGA *scr;
 FlyBack *flyback;

 Window *w;

 int status;
 int aux;		/* used as an axiliary var for status */
 void FSM();
 void Flush();
 void DrawAll();
 void DrawAndFlush();
private:

};



#endif
