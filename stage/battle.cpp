#ifndef _BATTLE_H
#define _bATTLE_H
#include ".\stage\battle.h"
#endif

int battle_delay = 0;
void interrupt battle_timer()
{
 if(battle_delay) battle_delay--;
}

/**************************************************************/
/* The constructor will initialize the XMS in class MAP using */
/* 210 KB, because each background just take 210 kb			  */
/**************************************************************/
Battle::Battle(Player *pplayer,Map *pmap,Timer *ptimer,SVGA *pscr,Keyboard *pkeyboard,FlyBack *pflyback,Monster *pmonster)
{
 SetBasic(pplayer,pmap,ptimer,pscr,pkeyboard,pflyback,pmonster);
 return;
}


Battle::Battle(Map *pmap,Monster *pmonster,World *pworld)
{
 SetBasic(pworld->player,pmap,pworld->timer,pworld->scr,pworld->keyboard,pworld->flyback,pmonster);
}



/********************************************************************/
/* This function will set the pointers to the proper object. What's */
/* more, it will also initialize the finite state machine			  */
/********************************************************************/
void Battle::SetBasic(Player *pplayer,Map *pmap,Timer *ptimer,SVGA *pscr,Keyboard *pkeyboard,FlyBack *pflyback,Monster *pmonster)
{
 back1 = pmap;
 mon = pmonster;
 scr = pscr;
 timer = ptimer;
 keyboard = pkeyboard;
 player = pplayer;
 flyback = pflyback;

 back2 = new Map(".\\res\\b_back.bmp",scr);

 /* Set the fighting background */
 back1->SetXY(0,0);
 back1->SetDrawWH(640,320);
 back2->SetXY(0,320);	/* X,Y is the coordinate on the bmp file */
 back2->SetDrawWH(640,160);

 status = BATTLE_BEGIN;
}


Battle::~Battle()
{
 delete back1;
 delete mon;
 delete back2;
 /* Class monster will be deleted by the destructor, so don't deleit it again */
 /* after returning. */
 //player->ClearLargePic();

 player->Stop();

 return;
}




void Battle::DrawBackground()
{
 //printf("Calling Map::Draw\n");
 back1->DrawOnXY();
 back2->DrawOnXY();
}

inline void Battle::DrawMonster()
{
 mon->Draw();
}



inline void Battle::DrawPlayer()
{
 player->DrawLargePic();
}


void Battle::Fight()
{
 //printf("DrawALL()\n");
 DrawAll();
 //printf("Flush()\n");
 Flush();
 //printf("FSM()\n");

 Timer *timer2 = new Timer((void far *)battle_timer);
 w = new Window(".\\script\\s1.txt",scr);

 FSM();

 delete w;
 delete timer2;
}

void Battle::Flush()
{
 scr->Flush();
}

void Battle::DrawAll()
{
 //printf("DrawBack()\n");
 DrawBackground();
 //printf("DrawMonster()\n");
 DrawMonster();
 //printf("DrawPlayer()\n");
 DrawPlayer();
}


void Battle::FSM()
{
 while(1)
 {                /* Prepare canvas for next drawing */
  DrawAll();		/* Every time we re-enter into status selection we must refresh the buffer */
						/* to invalidate the picture painted before */
  switch(status)
  {
	case BATTLE_BEGIN: SelectBeginAction();break;
	case BATTLE_PLCHOOSE_1: PlayerChooseAction(0);break;
	case BATTLE_PLCHOOSE_2: PlayerChooseAction(1);break;  /* We can't use fall through here */
	case BATTLE_PLCHOOSE_3: PlayerChooseAction(2);break;  /* because we need a refresh */
	case BATTLE_MTCHOOSE: PlayerChooseMonster(aux);break;  /* aux is promised to be the player number */
	case BATTLE_READY: BeginBattle();break;
	case BATTLE_LEAVE: break;
	case BATTLE_FAIL: Fail();
	default:
		  status = BATTLE_BEGIN;break;
  }

  if(status == BATTLE_FINISH) status = BATTLE_LEAVE;
  if(status == BATTLE_LEAVE) {key_down = 1;key_code = KEYE;break;}

  Flush();
 }
}


void Battle::Wait(int ptime)
{
 battle_delay = ptime;
}


void Battle::WaitTime(int ptime)
{
 Wait(ptime);
 while(!TimeOut()) ;
}


int Battle::TimeOut()
{
 return battle_delay == 0;
}



void Battle::Fail()
{
 w = new Window(".\\script\\s2.txt",scr);

 w->DrawGrid(0,0,20,15);
 w->Print(9,7,10,WINDOW_CENTER);

 delete w;
}



void Battle::BeginBattle()
{
 int i;

 Wait(5); i = 0;
 while(!TimeOut())
 {
  DrawAll();
  w->DrawGrid(10,i++,20,3);
  w->Print(9,0,1,WINDOW_CENTER);
  Flush();
 }

 WaitTime(18);

 Wait(6);
 while(!TimeOut())
 {
  DrawAll();
  w->DrawGrid(10,i++,20,3);
  w->Print(9,0,1,WINDOW_CENTER);
  Flush();
 }

 DrawAll();
 Flush();

 while(1)
 {
  i = 0;
  while(player->property[i] && player->property[i]->life > 0) { PlayerAttack(i); i++; }
  i = 0;
  while(i < mon->num && !ISDEAD(mon->status[i])) { MonsterAttack(i); i++; }

  Judge();                     /* We assume that no player will be dead after passing this! */
  if(status != BATTLE_FAIL && status != BATTLE_FINISH ) status = BATTLE_PLCHOOSE_1;

  break;
 }

 return;
}


void Battle::Judge()
{
 int i = 0;
 int flag = 0;

 while(player->type[i] != -1)
 {
  if(player->property[i]->life <= 0) status = BATTLE_FAIL;
  i++;
 }

 for(i=0;i<mon->num;i++)
 {
  if(mon->property[i].life <= 0) mon->status[i] = MONSTER_DEAD;
  if(!ISDEAD(mon->status[i])) flag = 1;
 }
 if(flag == 0) status = BATTLE_FINISH;

 return;
}


#define SHAKE(j,k,i) for(int _tempi = 0;_tempi < (i);_tempi++) { j->Shake(k);DrawAndFlush();WaitTime(4);j->UnShake(k);DrawAndFlush();WaitTime(4); }
	  //       ^ shake who

void Battle::ShakeMonster(Monster *m,int pnum,int i)
{
 while(--i)
 {
  m->Shake(pnum); DrawAndFlush(); WaitTime(4);
  m->UnShake(pnum);DrawAndFlush();WaitTime(4);
 }
}


void Battle::ShakePlayer(Player *p,int pnum,int i)
{
 while(i--)
 {
  p->Shake(pnum); DrawAndFlush(); WaitTime(4);
  p->UnShake(pnum);DrawAndFlush();WaitTime(4);
 }
}

void Battle::PlayerAttack(int i)
{
 if(player->action[i] == ACTION_ATTACK)
 {
  w->DrawGrid(10,8,20,3);
  w->Print(9,1+i,1,WINDOW_CENTER);
  Flush();
  WaitTime(8);
  DrawAndFlush();
  WaitTime(4);
  //////////////////////////////////////
  ShakePlayer(player,i,1);
  WaitTime(6);
  ShakeMonster(mon,player->target[i],4);
  int injury = (int)(1.0*player->property[i]->attack * ((100 - mon->property[i].defence)*1.0 / 100.0));
  mon->property[player->target[i]].life -= injury;
  w->DrawGrid(10,8,20,3);
  w->PrintNumber(1,9,injury);
  Flush();
  WaitTime(12);
 }
}



void Battle::MonsterAttack(int i)
{
 int j = 0;
 int random;
 while(player->property[j]) j++;

 random = rand();
 random %= j;

 ShakeMonster(mon,i,2);
 WaitTime(6);
 ShakePlayer(player,random,4);
 int injury = (int)(1.0 * mon->property[i].attack * ((100 - player->property[random]->defence)*1.0 / 100.0));
 player->property[random]->life -= injury;
 w->DrawGrid(10,8,20,3);
 w->PrintNumber(1,9,injury);
 Flush();
 WaitTime(12);

 return;
}

/**********************************************************/
/* This function will let the user to choose a action in  */
/* the battle. Now the fight has already begun				 */
/**********************************************************/
#define PROPERTY_TABLE 15
void Battle::PlayerChooseAction(int pnum)
{
int choice = 0;
 //int i;
 Window *w = new Window(".\\script\\s2.txt",scr);

 /////////////////////add....................
 while(1)
 {
  w->DrawGrid(10,5,20,19);
  w->Print(9,4,1,WINDOW_CENTER);
  w->Print(9,pnum + 5,3,WINDOW_CENTER);
  w->Print(choice == 0 ? 5 : 9,10,6,WINDOW_CENTER);    	//ATTACK
  w->Print(choice == 1 ? 5 : 9,11,8,WINDOW_CENTER);    	//MAGIC
  w->Print(choice == 2 ? 5 : 9,12,10,WINDOW_CENTER);   	//DEFENCE
  w->Print(choice == 3 ? 5 : 9,13,12,WINDOW_CENTER);  	//PRODUCT ?!
  w->PrintProperty(player->property[pnum],9,PROPERTY_TABLE,14);
  Flush();

  if(key_down == 1)
  {
	 key_down = 0;
	 if(key_code == KEYS) 		choice = (choice == 3 ? 0 : (choice + 1));
	 else if(key_code == KEYW) choice = (choice == 0 ? 3 : (choice - 1));
	 else if(key_code == KEYSPACE) WaitforSpace();

	 //w->Print(5,10 + choice,6 + 2 * choice, WINDOW_CENTER);
  }/* if(key_down == 1) */

  if(key_up == 1)
  {
	key_up = 0;
	if( (key_code & ~0x80) == KEYENTER )
	{
	 player->action[pnum] = choice;
	 aux = pnum; 	/* We use the axu member to indicate the player we're referring to */
	 if(choice == ACTION_ATTACK) status = BATTLE_MTCHOOSE;
	 else if(choice == ACTION_DEFENCE)
	 {
	  if( player->property[pnum + 1] == NULL ) status = BATTLE_READY;
	  else if(pnum == 0) status = BATTLE_PLCHOOSE_2;
	  else if(pnum == 1) status = BATTLE_PLCHOOSE_3;
	  else if(pnum == 2) status = BATTLE_PLCHOOSE_4;
	 }

	 return;
	}/* KEYENTER */
	else if( (key_code & ~0x80) == KEYQ )	/* Go back to the monster choose of the previous player */
	{
	 if(pnum == 0) status = BATTLE_BEGIN;  /* or return to the begin state */
	 else if(pnum == 1) status = BATTLE_PLCHOOSE_1;
	 else if(pnum == 2) status = BATTLE_PLCHOOSE_2;
	 else if(pnum == 3) status = BATTLE_PLCHOOSE_3;
	 else if(pnum == 4) status = BATTLE_PLCHOOSE_4;
	 return;
	}
  }
 }/* while(1) */
 delete w;
}


/***************************************************************/
/* This function will let the player choose from the monsters  */
/* for later action (as targets)										   */
/***************************************************************/
void Battle::PlayerChooseMonster(int pnum)
{
 int choice = 0;

 Window *w = new Window(".\\script\\s2.txt",scr);

 while(1)
 {
  SELECT(mon->status[choice]);
  DrawMonster();
  w->DrawGrid(10,18,20,8);
  w->Print(9,19,1,WINDOW_CENTER);
  w->PrintProperty(&mon->property[choice],9,PROPERTY_TABLE,3,1);
  if(mon->status[choice] == MONSTER_DEAD) w->Print(9,20,6,WINDOW_CENTER);
  else if(WITHOUTSELECT(mon->status[choice]) == MONSTER_DEAD) w->Print(9,21,6,WINDOW_CENTER);
  else if(WITHOUTSELECT(mon->status[choice]) == MONSTER_POISION) w->Print(9,22,6,WINDOW_CENTER);
  else if(WITHOUTSELECT(mon->status[choice]) == MONSTER_DIZZY) w->Print(9,23,6,WINDOW_CENTER);
  else if(WITHOUTSELECT(mon->status[choice]) == MONSTER_PARALYZED) w->Print(9,24,6,WINDOW_CENTER);
  else if(WITHOUTSELECT(mon->status[choice]) == MONSTER_DAZED) w->Print(9,25,6,WINDOW_CENTER);
  else if(WITHOUTSELECT(mon->status[choice]) == MONSTER_NORMAL) w->Print(9,26,6,WINDOW_CENTER);

  Flush();

  if(key_down == 1)
  {
	key_down = 0;
	if(key_code == KEYA)
	{
	 UNSELECT(mon->status[choice]);

	 do choice = (choice == 0 ? mon->num - 1 : choice - 1); while(mon->status[choice] == MONSTER_DEAD);
	}
	else if(key_code == KEYD)
	{
	 UNSELECT(mon->status[choice]);

	 do choice = (choice == mon->num - 1 ? 0 : choice + 1); while(mon->status[choice] == MONSTER_DEAD);
	}
	else if(key_code == KEYSPACE)
	{
	 UNSELECT(mon->status[choice]);
	 WaitforSpace();
	 SELECT(mon->status[choice]);
	}
  }/* if(key_down == 1) */

  if(key_up == 1)
  {
	key_up = 0;
	if( (key_code & ~0x80) == KEYENTER )
	{
	 player->target[pnum] = choice;
	 UNSELECT(mon->status[choice]);	/* For later we can see clearly */

	 if(player->property[pnum + 1] == NULL) status = BATTLE_READY;
	 else if(pnum == 0) status = BATTLE_PLCHOOSE_2;
	 else if(pnum == 1) status = BATTLE_PLCHOOSE_3;
	 else if(pnum == 2) status = BATTLE_PLCHOOSE_4;
	 return;
	}
	else if( (key_code & ~0x80 ) == KEYQ )
	{
	 if(pnum == 0) status = BATTLE_PLCHOOSE_1;
	 else if(pnum == 1) status = BATTLE_PLCHOOSE_2;
	 else if(pnum == 2) status = BATTLE_PLCHOOSE_3;
	 else if(pnum == 3) status = BATTLE_PLCHOOSE_4;
	 return;
	}
  }
 }/* while(1) */

 delete w;
}



/**********************************************************/
/* This function will implement one part of the FSM, to   */
/* let the user to determine whether to run away or fight */
/* by keyboard interaction											 */
/**********************************************************/
void Battle::SelectBeginAction()
{
 int choice = 0;
 Window *w = new Window(".\\script\\s2.txt",scr);

 while(1)
 {
  w->DrawGrid(10,5,20,10);
  w->Print(9,0,1,WINDOW_CENTER);
  w->Print(9,1,3,WINDOW_CENTER);
  w->Print(5,2,6,WINDOW_CENTER);
  w->Print(9,3,8,WINDOW_CENTER);
  w->Print(5,2 + choice,6 + 2 * choice,WINDOW_CENTER);
  w->Print(9,3 - choice,8 - 2 * choice,WINDOW_CENTER);

  Flush();

  if(key_down == 1)
  {
	key_down = 0;

	if(key_code == KEYS) { choice = (choice == 1 ? 0 : 1); }
	else if(key_code == KEYW) { choice = (choice == 0 ? 1 : 0); }
	else if(key_code == KEYSPACE) WaitforSpace();
  }

  if(key_up == 1)
  {
	key_up = 0;

	if( (key_code & ~0x80) == KEYENTER)
	{
	 if(choice == 1) status = BATTLE_LEAVE;
	 else status = BATTLE_PLCHOOSE_1;
	 return;
	}
  } /* if(key_up == 1) */
 } /* while(1) */

 delete w;
}



void Battle::WaitforSpace()
{
 DrawAll();
 Flush();
 while(key_up == 0);
 key_up = 0;
}


void Battle::DrawAndFlush()
{
 DrawAll();Flush();
}
