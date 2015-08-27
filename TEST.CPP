#include ".\test.h"

#include ".\error\error.cpp"
#include ".\mem\mem.cpp"
/* These two file is the base of other */
/* Must be put at the topmost */
#include ".\driver.h"
#include ".\mem\load.cpp"

#include ".\keyboard\keyboard.cpp"
#include ".\graphic\VGA.cpp"
#include ".\graphic\SVGA.cpp"
#include ".\timer\timer.cpp"
#include ".\cnstring\cnstring.cpp"
#include ".\bmp\bmp.cpp"
#include ".\stage\property.cpp"
#include ".\player\map.cpp"
#include ".\player\player.cpp"
#include ".\timer\flyback.cpp"

#include ".\stage\world.cpp"
#include ".\stage\monster.cpp"
#include ".\window\window.cpp"
#include ".\stage\battle.cpp"



#define DEBUG(s) {printf(s);while(1);}



World *CreateWorld()
{
 SVGA *scr = new SVGA;
 Keyboard *key = new Keyboard((far_ptr)ProcessKey);
 Timer *timer1 = new Timer((far_ptr)KeepRun);
 /* Be careful that we should use // in const char[] */
 Player *p = new Player(".\\res\\1.bmp",scr);
 Map *m = new Map(".\\res\\map1.bmp",scr,1);
 FlyBack *flyback;

 Property::nstr = new CnString(".\\script\\name.txt",scr);

 World *temp = new World(p,m,timer1,scr,key,flyback);
 temp->SetStartPoint(6 << 5,0);
 temp->Init();

 return temp;
}




void main()
{
  /***************************************************
  void *temp = malloc(65535L);     | This code proves
  temp = malloc(65535L);           | that we can at
  temp = malloc(65535L);           | most allocate 8
  temp = malloc(65535L);           | segments from the
  temp = malloc(65535L);           | NTVDM system
  temp = malloc(65535L);           | (may not be true
  temp = malloc(65535L);           |  for a real DOS)
  temp = malloc(65535L);           |
											  |
  if(temp == NULL) printf("Allocate error!\n");
  else printf("Suuessful allocation!");
											  |
  while(1);                        |
  ***************************************************/
  /* So we must guarantee that the maximum memory   */
  /* allocation shouldn't excess 6 ( = 384KB )      */
  /* another 2 for futher use								 */
  /**************************************************/
  World *w = CreateWorld();		/* This must be the first */

  while(1)
  {
	if(w->flyback->OntheFly() == 1) w->scr->Flush();
	else
	{
	 w->KeepMove();
	 if(key_down == 1)
	 {
	  key_down = 0;	/* So that further key down can be catched immediately */

	  w->Move();
	  if(key_code == KEYQ)
	  {
		Monster *mmm = new Monster(3,w,5,6,7);
		Map *scene = new Map(".\\res\\b_back1.bmp",w->scr);
		Battle *bbb = new Battle(scene,mmm,w);
		bbb->Fight();
      delete bbb;
	  }

	 }// if(key_down == 1)
	 else if(key_up == 1) key_up = 0;
	  /* We'll not process this at least till now */
	}// if(OntheFly == 1) else
  }// while(1)
  return;
}
