#include ".\cnstring\cnstring.h"


CnString::CnString(const char *filename,SVGA *pscr)
{
 scr = pscr;
 Init(filename);	/* Read in the script file */

 if(firstrun == 1)
 {
  firstrun = 0;

  HZK = fopen("HZK16","rb");
  if(HZK == NULL)
  {
	FatalError e("Cannot open HZK16 file.");
	e.Print();
  }
 }

 return;
}

CnString::~CnString()
{
 int i;

 for(i = 0;i < num;i++) free(str[i]);
 //if(HZK) fclose(HZK);

 return;
}


void CnString::Init(const char *filename)
{
 FILE *strfp = NULL;
 int i = 0;
 int len = 0;
 char buffer[CNSTR_BUFFERLEN];

 strfp = fopen(filename,"r");
 if(strfp == NULL)
 {
  FatalError e("Cannot open string table!");
  e.Print();
 }

 while(!feof(strfp))
 {
  fscanf(strfp,"%s",buffer);
  len = strlen(buffer);
  //printf("%u\n",len);
  str[i] = (char *)malloc(len + 2);
  strcpy(str[i],buffer);
  i++;
 }

 num = i;	/* Num is used to remember how many strings are read for delete */

 fclose(strfp);
 return;
}


void CnString::PrintChar(char chr[2],int color,int px,int py)
{
 static char far *buffer = new char[16];
 static char far *temp = new char[32];
 /* Let the memory leak... */
 /* The Borland compiler has a fragile stack frame... */

 chr[0] -= 0xA1;
 chr[1] -= 0xA1;

 fseek(HZK,( 94 * (unsigned long)chr[0] + (unsigned long)chr[1]) << 5 ,0);

 fread(temp,32,1,HZK);
 int tempy = py;

 for(int line = 0;line < 16;line++)
 {
  unsigned long offset = (unsigned long)tempy * 640 + (unsigned long)px;
  tempy++;
  scr->xmsbuffer.OuttoNor((unsigned char far *)buffer,offset,16);
  for( int k = 0 ; k < 8 ; k++ )
  {
	if( temp[line << 1] & (0x80 >> k) ) buffer[k] = color;
	if( temp[(line << 1) + 1] & (0x80 >> k) ) buffer[8 + k] = color;
  }

  scr->xmsbuffer.InfromNor((unsigned char far *)buffer,offset,16);
 }//for(line = 0;line < 16;line++)
 px += 16;

 return;
}



void CnString::PrintString(char c[],int color,int px,int py,int dw)
{
 int i = 0;
 int numinline = 0;
 char chr[2];
 const int x_bak = px;

 while(1)
 {
  int fseekret = 0;

  if(numinline >= dw)     /* dw is the number of characters */
  {
	numinline = 0;
	py += 16;
	px = x_bak;
  }
  if(py >= 480) return;		/* If the range exceeds the screen then exit the function */
  numinline++;             /* Every time we get here we'll print a char, so numinline++ */

  chr[0] = c[i << 1];
  chr[1] = c[(i << 1) + 1];
  i++;

  if(chr[0] == 0 || chr[1] == 0) break;

  PrintChar(chr,color,px,py);
  px += 16;
 }//while(1)
 return;
}



void CnString::Print(int color,int pnum,int px,int py,int dw,int delay)
{
 PrintString(str[pnum],color,px,py,dw);
}


void CnString::SetSVGA(SVGA *pscr)
{
 scr = pscr;
}


inline int CnString::StrLen(int pnum)
{
 return strlen(str[pnum]) >> 1;
}
