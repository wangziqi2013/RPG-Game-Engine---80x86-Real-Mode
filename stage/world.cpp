#include ".\stage\world.h"


/****************************************************************/
/* The constructor will initialize the coordinate of player and */
/* map. Mind that the coordinates are all absolute                   */
/****************************************************************/
World::World(Player *pplayer,Map *pmap,Timer *ptimer,SVGA *pscr,Keyboard *pkeyboard,FlyBack *pflyback)
{
 player = pplayer;
 map = pmap;
 timer = ptimer;
 scr = pscr;
 keyboard = pkeyboard;
 flyback = pflyback;

 /* This will set the map's total dimension */
 mw = map->tw;
 mh = map->th;

 SetStartPoint(0,0);

 //Init();

 return;
}


World::~World()
{
 delete player;
 delete map;
 delete timer;
 delete scr;
 delete keyboard;

 return;
}


/****************************************************************/
/* This function will set the start point of the player. Notice */
/* that the psx and psy are all absoulute coordinates, and the  */
/* coordinates of map will adjust automatically to the player   */
/****************************************************************/
void World::SetStartPoint(int psx,int psy)
{
 /* Notice that mx and my and px and py are all global */
 /* while the x,y of map and player respectively are relative */
 /* (on-screen coordinate) */
 mx = map->x = psx - 320;
 my = map->y = psy - 240;

 if(mx < 0) map->x = 0;
 if(my < 0) map->y = 0;

 px = psx;
 py = psy;
 /* Player->x and y is the relative coordinate on the screen */
 player->x = psx % 640;
 player->y = psy % 480;

 return;
}


/*******************************************************/
/* This function will do the essential save work and   */
/* then draw out the map and player                     	 */
/*******************************************************/
void World::Init()
{
 bitmap *temp = new bitmap(".\\res\\1.bmp",scr);
 temp->Read();
 temp->InitPalette();
 delete temp;

 map->Draw();
 //player->SaveBack();
 player->Draw();

 return;
}


inline void World::GoLeft()
{
 //CheckAccess();
 px -= 6;
 if(px < 0) px = 0;
 mx = px - 320;
 if(px - 320 < 0) map->x = 0;
 else if(px + 320 >= mw) map->x = mw - 640 - 1;
 else map->x = mx;
 player->x = px - map->x;

 map->Draw();
 player->NextStep();
 player->Draw();
}


inline void World::GoRight()
{
 //CheckAccess();
 px += 6;
 if(px + 32 >= mw) px = mw - 1 - 32;
 mx = px - 320;
 if(px - 320 < 0) map->x = 0;
 else if(px + 320 >= mw) map->x = mw - 640 - 1;
 else map->x = mx;

 player->x = px - map->x;

 map->Draw();
 player->NextStep();
 player->Draw();
}


inline void World::GoUp()
{
 //CheckAccess();
 py -= 6;
 if(py < 0) py = 0;
 my = py - 240;
 if(py - 240 < 0) map->y = 0;
 else if(py + 240 >= mh - 1) map->y = mh - 480 - 1;
 else map->y = my;
 player->y = py - map->y;

 map->Draw();
 player->NextStep();
 player->Draw();
}



inline void World::GoDown()
{
 //CheckAccess();
 py += 6;
 if(py + 48 >= mh) py = mh - 1 - 48;
 my = py - 240;
 if(py - 240 < 0) map->y = 0;
 else if(py + 240 >= mh - 1) map->y = mh - 480 - 1;
 else map->y = my;
 player->y = py - map->y;

 map->Draw();
 player->NextStep();
 player->Draw();
}


/*******************************************************************/
/* This function will check the key pressed and then set necessary */
/* variables for a running                              						 */
/*******************************************************************/
/* Notice : No essential work of drawing is done here.               */
/*******************************************************************/
void World::Move()
{
 switch(key_code)
 {
  case KEYA:
	  if(player->direction == 2 && player_run != 0) player->Stop();
	  else
	  {
		player->Run();
		player->GoLeft();
	  }
	  break;
  case KEYD:
		if(player->direction == 1 && player_run != 0) player->Stop();
		else
	  {
		player->Run();
		player->GoRight();
	  }
	  break;
  case KEYW:
	  if(player->direction == 0 && player_run != 0) player->Stop();
	  else
	  {
		player->Run();
		player->GoUp();
	  }
	  break;
  case KEYS:
	  if(player->direction == 3 && player_run != 0) player->Stop();
	  else
	  {
		player->Run();
		player->GoDown();
	  }
	  break;
  case KEYE:
	  map->Draw();player->Draw();
	  break;
  case KEYR:
	  printf("%d %d %d  ",px, py,map->access[ px + py * (map->tw / 32) ]);
	  break;
  default:
	  break;
  }// switch

  CheckAccess();
  return;
}



void World::CheckAccess()
{
 static int tb = map->tw / 32;
 int access,temp1,temp2;

 switch(player->direction)
 {
  case 0:
	temp1 = (py) + player->dy + 40;
	temp2 = temp1 >> 5;
	if((temp1 % 32) >= 25) temp2++;
	access = map->access[ (px + player->dx + 16) / 32 + tb * temp2 ];
	break;
  case 1:
	temp1 = px + player->dx;
	temp2 = temp1 >> 5;
	if((temp1 % 32) <= 1) temp2--;
	access = map->access[ temp2 + tb * ((py + 48) / 32) ];
	break;
  case 2:
	temp1 = px + player->dx + 32 - 10;
	temp2 = temp1 >> 5;
	if((temp1 % 32) >= 31) temp2++;
	access = map->access[temp2 + tb * ((py + 48) / 32)];
	break;
  case 3:
	temp1 = (py + player->dy + 48 - 12);
	temp2 = temp1 >> 5;
	if(temp1 % 32 <= 1) temp2--;
	access = map->access[ (px + player->dx + 16) / 32 + tb * temp2 ];
	break;
 }

 if(!access) player->Stop();
 if(access & WD_DIALOG)
 {
  int dialognum = access >> 8;
  ShowDialog(dialognum);
 }
 else if(access & WD_BATTLE)
 {
  int battlenum = access >> 8;
  MakeBattle(battlenum);
 }
 else if(access & WD_SWITCHMAP)
 {
  int mapnum = access >> 8;
  SwitchMap(mapnum);
 }

 return;
}


void World::SwitchMap(int mapnum)
{
 switch(mapnum)
 {
  case 1:
   
	break;
  case 2:
	break;
  case 3:
	break;
  case 4:
	break;
  case 5:
	break;
  case 6:
	break;
 }
}


void World::MakeBattle(int battlenum)
{
 return;
}

void World::ShowDialog(int dialognum)
{
 return;
}



void World::KeepMove()
{
 if(player_changed == 1)
 {
  player_changed = 0;

  CheckAccess();

  switch(player->direction)
  {
	 case 0: GoDown();break;
	 case 1: GoLeft();break;
	 case 2: GoRight();break;
	 case 3: GoUp();break;
  }
 }
}
