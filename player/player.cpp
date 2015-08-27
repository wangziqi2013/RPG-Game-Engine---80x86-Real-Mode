#include ".\player\player.h"


Player::Player(const char *filename,SVGA *pscr) : xmsbuffer(30)
{
 pic = new bitmap(filename,scr);    /* This will not read all data */
 scr = pscr;

 tl = 4;                    /* For a char it's always true */
 w = pic->GetWidth()  / 4;
 h = pic->GetHeight() / 4;  /* >> 2 */

 Read();

 buf3valid = 0;

 pic->ReadLine(0);
 transparent = pic->line[0];
 /* This read the transparent color */
 x = y = 0;
 direction = step = 0;

 for(int i = 0;i < MAX_PLAYER;i++)
 {
  property[i] = NULL;
  large_pic[i] = NULL;
  action[i] = 0;
  type[i] = -1;
 }

 property[0] = &::property[1];
 property[1] = &::property[0];
 type[0] = 1;
 type[1] = 0;

 return;
}


Player::~Player()
{
 if(buf3valid == 1) free(buffer3);

 for(int i = 0;i < MAX_PLAYER;i++) if(property[i] != NULL) delete property[i];

 return;
}


inline void Player::SetXY(int px,int py)
{
 x = px;y = py;
}


char *Player::GenerateFileName(int type)
{
 sprintf(filename,".\\res\\p%d.bmp",type);
 return filename;
}


void Player::DrawLargePic()
{
 static firstdraw = 1;

 if(firstdraw == 1)
 {
  InitLargePic();
  firstdraw = 0;
 }

 for(int i = 0;i < MAX_PLAYER;i++)
  if(property[i] != NULL)
	large_pic[i]->DrawWithTrans();

 return;
}


void Player::InitLargePic()
{
 int i;
 int drawx,drawy;

 for(i = 0;i < MAX_PLAYER;i++)
 {
  if(property[i] != NULL)
  {
	GenerateFileName(type[i]);
	large_pic[i] = new Map(filename,scr);
	drawx = (640 >> 3) * ( (i << 1) + 1 ) - (large_pic[i]->pic->GetWidth() >> 1);
	drawy = 480 - large_pic[i]->pic->GetHeight() - 10;
	large_pic[i]->SetXY(drawx,drawy);
	large_pic[i]->SetDrawWH();
  }
 }

 return;
}


void Player::ClearLargePic()
{
 return;
}


/***********************************************************/
/* This function will read the bitmap file into xms buffer */
/* and we'll put one whole picture together and linearly   */
/* in order to accelerate the display routine                */
/***********************************************************/
void Player::Read()
{
 int i;
 int line;
 unsigned char far *address;
 unsigned long size;
 unsigned long offset;

 size = w * h;   /* w and h are both the dimensions of a single character */
 /* so the offset between two characters is just the value of 'size' */
 address = (unsigned char far *)pic->line;
 for( i = 0 ; i < tl ; i++ )
 {
  for(line = 0 ; line < h ; line++ )
  {
		pic->ReadLine(line + i * h);
		offset = ((unsigned long)i * (size << 2)) + (line * (unsigned long)w);
		/*             ^^ because each row has 4 single characters */
		/*        ^ i means the number of row out of 4(0 - 3) */
		/*                            ^^ This is only the start address of the leftmost 1/4 line  */
		xmsbuffer.InfromNor(address,offset,w);
		offset += size;
		xmsbuffer.InfromNor(address + w,offset,w);
		offset += size;
		xmsbuffer.InfromNor(address + w + w,offset,w);
		offset += size;
		xmsbuffer.InfromNor(address + w + w + w,offset,w);
  }
 }
 return;
}


inline void Player::NextStep()
{
 static unsigned char step_delay = 0;

 step_delay++;

 if(step_delay == 4)
 {
  step++;
  step %= 4;
  step_delay = 0;
 }
}


/********************************************************/
/* This function will put the bitmap in the SVGA buffer */
/* according the information stored in the player class */
/* e.g. x,y,direction...                                          */
/********************************************************/
/* We'll use pic->scr->xmsbuffer.OuttoXMS in the routine*/
/********************************************************/
void Player::Draw()
{
 int i,j;
 unsigned long doffset, soffset; /* doffset is the offset on the SVGA xmsbuffer */
 unsigned long size;             /* and soffset is the offset in the bitmap buffer */

 size = h * w;

 doffset = (unsigned long)y * 640 + (unsigned long)x;

 soffset = (size << 2) * direction + size * step;

 for( i = 0 ; i < h ; i++ )
 {
  scr->xmsbuffer.OuttoNor(buffer1,doffset,w); /* First copy the screen buffer back to buffer1 */
  xmsbuffer.OuttoNor(buffer2,soffset,w);  /* Copy the bitmap to buffer2 */

  for(j=0;j<w;j++)
  {
	if(buffer2[j] != transparent) buffer1[j] = buffer2[j];
  }

  scr->xmsbuffer.InfromNor(buffer1,doffset,w);
  doffset += 640;
  soffset += w;
 }
 return;
}

/* Old Version
void Player::Move(int newx,int newy,int ndirection,int nstep)
{
 RestoreBack(x,y);
 SaveBack(newx,newy);
 x = newx;
 y = newy;
 direction = ndirection;
 step = nstep;
 Draw();
 return;
}
*/
void Player::Move()
{
 x += dx;
 y += dy;

 /* We'll not clear dx and dy for further use */
 /* So if we want to stop then we must clear dx and dy manually */
 Draw();
 return;
}


inline void Player::Stop()
{
 dx = dy = 0;
 step = 0;
 player_run = 0;
 Draw();
}

inline void Player::Run()
{
 player_run = 10;
}


inline void Player::GoLeft()
{
 dx = -6; dy = 0; direction = 1;
}
inline void Player::GoRight()
{
 dx = 6; dy = 0; direction = 2;
}
inline void Player::GoUp()
{
 dx = 0; dy = -6;direction = 3;
}
inline void Player::GoDown()
{
 dx = 0; dy = 6; direction = 0;
}

inline void Player::Shake(int i)
{
 large_pic[i]->Shake();
}

inline void Player::UnShake(int i)
{
 large_pic[i]->UnShake();
}
