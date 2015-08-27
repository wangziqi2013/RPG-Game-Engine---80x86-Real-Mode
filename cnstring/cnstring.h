#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif



#ifndef _CNSTR_H
#define _CNSTR_H

#define MAXSTR 200
#define CNSTR_BUFFERLEN 200

class CnString
{
public:
 static  FILE   *HZK;
 char       *str[MAXSTR];
 int        num;
 SVGA       *scr;
 static  int    firstrun;

 CnString(const char *filename,SVGA *pscr);
 ~CnString();
 void Print(int color,int pnum,int px,int py,int dw = 480 / 16,int delay = 0);
 void PrintChar(char c[2],int color,int px,int py);
 void PrintString(char c[],int color,int px,int py,int dw);
 int  StrLen(int pnum);
 void Init(const char *filename);
 void SetSVGA(SVGA *pscr);
};

int CnString::firstrun = 1;
FILE *CnString::HZK = NULL;

#endif
