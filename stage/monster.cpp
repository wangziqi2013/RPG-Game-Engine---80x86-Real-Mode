
#include ".\stage\monster.h"


Monster::Monster(int monsternum,SVGA *pscr)
{
 SetBasic(monsternum,pscr);
}


Monster::Monster(int monsternum,World *pworld)
{
 SetBasic(monsternum,pworld->scr);
}


Monster::Monster(int monsternum,World *pworld,int m1,int m2,int m3,int m4,int m5)
{
 SetBasic(monsternum,pworld->scr);
 switch(monsternum)
 {
  case 5: SetMonsterType(4,m5); 	/* Fall through!!!!!!!!! */
  case 4: SetMonsterType(3,m4);
  case 3: SetMonsterType(2,m3);
  case 2: SetMonsterType(1,m2);
  case 1: SetMonsterType(0,m1);
  default: break;
 }

 for( int i = 0 ; i < MAXMONSTER ; i++ )
 {
  status[i] = MONSTER_NORMAL;			/* All monsters are normal at first */
 }

 return;
}



void Monster::SetBasic(int monsternum,SVGA *pscr)
{
 strcpy(filename,".\\res\\m0.bmp");
 /* index = 7 is the number */
 scr = pscr;
 num = monsternum;
 firstdraw = 1;
}

/*******************************************************/
/* This function is designed to initialize the Monster */
/* with a monster number and type.							 */
/*******************************************************/
/* Because the internal implement of Monster is not    */
/* guaranteed to remain stable, so please use the int- */
/* erface instead of directly access						 */
/*******************************************************/
/* This function do not require much speed, so we add  */
/* error detect													 */
/* Once an error happen, it will print a message but	 */
/* not fatal.														 */
/*******************************************************/
void Monster::SetMonsterType(int which,int ptype)
{
 if(which >= num)/* which = 0,1,2,...;num = 1,2,3,... so it's off-by-one */
 {
  GeneralError e("Monster error: not a valid number.");
  e.Print();
 }
 else
 {
  type[which] = ptype;
  property[which] = ::property[ptype + 1];    /* for monsters we must copy the property */
 }

 return;
}


Monster::~Monster()
{
 int i;

 for(i=0;i<num;i++)
 {
  delete map[i];
 }

 return;
}

char *Monster::GenerateFileName(int ptype)
{
 sprintf(filename,".\\res\\m%d.bmp",ptype);

 return filename;
}


void Monster::GenerateName(int ptype)
{

}


void Monster::Shake(int i)
{
 map[i]->Shake();
 return;
}


void Monster::UnShake(int i)
{
 map[i]->UnShake();
 return;
}


void Monster::Draw()
{
 int i;

 if(firstdraw == 1)
 {
  Read();
  firstdraw = 0;
 }

 for(i=0;i<num;i++)
 {
  if(ISSELECTED(status[i])) map[i]->FillWithColor(9);
  else if(status[i] != MONSTER_DEAD) map[i]->DrawWithTrans();
 }
}


void Monster::Read()
{
 int i,j;
 int set = 0;

 for(i=0;i<num;i++)
 {
  /* And now we must make new class Map */
  GenerateFileName(type[i]);
  //printf("%s\n",filename);
  //printf("w = %lu, h = %lu\n",map[i]->pic->bihead.biwidth,map[i]->pic->bihead.biheight);
  map[i] = new Map(filename,scr);
  //printf("%s has been read.\n");
  map[i]->x = 640 / (num + 1) * (i + 1)  - (map[i]->tw / 2);
  map[i]->y = 320 / 2 - (map[i]->th / 2);
  map[i]->dw = map[i]->tw;
  map[i]->dh = map[i]->th;
  map[i]->SetTransparent(); 	/* Use default transparent color */
  //printf("x = %u, y = %u dw = %u dh = %u",map[i]->x,map[i]->y,map[i]->dw,map[i]->dh);
 }

 return;
}
