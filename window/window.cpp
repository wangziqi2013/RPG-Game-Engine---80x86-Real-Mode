#include ".\window\window.h"


Window::Window(const char *filename,SVGA *pscr)
{
 SetBasic(filename,pscr);
}

Window::Window(const char *filename,World *pworld)
{
 SetBasic(filename,pworld->scr);
}


void Window::SetBasic(const char *filename,SVGA *pscr)
{
 cnstring = new CnString(filename,pscr);

 if(firstdraw == 1)
 {
  back = new Map(".\\res\\window.bmp",pscr);
  arrow = new Map(".\\res\\arrow.bmp",pscr);
  arrow->SetDrawWH();   /* Use default argument, that is the original size */
  firstdraw = 0;
 }

 x = y = 0;
 scr = pscr;
 numtable = 16; //The most frequently used value!!!!
}


Window::~Window()
{
 /* map shouldn't be deleted */
 delete cnstring;
}


/*************************************************************/
/* This function will call draw back with the x and y stored */
/* in the object. Firstly it will set Map *back					 */
/*************************************************************/
void Window::DrawBack(int pdw,int pdh)
{
 dw = pdw;
 dh = pdh;

 back->SetXY(x,y);
 back->SetDrawWH(dw,dh);

 back->DrawOnXY();

 return;
}



void Window::DrawBack(int px,int py,int pdw,int pdh)
{
 x = px;
 y = py;
 dw = pdw;
 dh = pdh;
 back->SetXY(x,y);

 back->SetDrawWH(dw,dh);

 back->DrawOnXY();
 //scr->DrawLine(9,px + 8,py + 8,px + pdw - 8,py + 8);
 //scr->DrawLine(9,px + 8,py + pdh - 8,px + pdw - 8,py + pdh - 8);
 //scr->DrawLine(9,px + 8,py + 8,px + 8,py + pdh - 8);
 //scr->DrawLine(9,px + pdw - 8,py + pdh - 8,px + pdw - 8,py + pdh - 8);

 return;
}


/*************************************************************/
/* This function will draw an arrow on a relative coordinate */
/*************************************************************/
/* rx : The relative coorinate of x (to the left of window)  */
/* ry : The ......................y... 		top				 */
/*************************************************************/
void Window::DrawArrow(int rx,int ry)
{
 arrow->SetXY(back->x+rx,back->y+ry);
 arrow->DrawWithTrans();
}


/************************************************************/
/* This fuction will set the draw x,y to the member x and y */
/* to serve the two parameter version of DrawBack           */
/* (int dw,int dh) 														*/
/************************************************************/
void Window::SetXY(int px,int py)
{
 x = px;
 y = py;
}


/************************************************************/
/* tHIS FUNCTION WILL PRINT STRINGS ON THE WINDOW			   */
/************************************************************/
/* pcolor : The color of the characters						   */
/* pnum   : The index of the string in the script file	   */
/* pline  : The line to be printed on the window.           */
/*          ont line == 16 pixel 									*/
/* palign : Look the definitions of the macro					*/
/************************************************************/
void Window::Print(int pcolor,int pnum,int pline,int palign)
{
 int tempx,tempy;
 int len;

 tempy = (pline << 4) + y;
 len = cnstring->StrLen(pnum);

 if(len > (dw >> 4)) len = (dw >> 4);

 if(palign == WINDOW_CENTER) tempx = x + (dw >> 1) - ((len >> 1) << 4);
 else if(palign = WINDOW_LEFT) tempx = x;
 else if(palign = WINDOW_RIGHT) tempx = x + dw - (len << 4);

 cnstring->Print(pcolor,pnum,tempx,tempy,(dw >> 4));

 return;
}


/************************************************************/
/* Notice : Only the window that uses proper file as script	*/
/* is suitable for this												   */
/************************************************************/
/* pnum : the start number of the property-related string   */
/*        in case that the file changes		   			   */
/************************************************************/
void Window::PrintProperty(Property *p,int pcolor,int pnum,int pline,int ismonster)
{
 int tempx,tempy;
 char *temp = new char[10];
 int len;

 tempy = (pline << 4) + y;
 strcpy(temp,cnstring->str[pnum++]);
 strcat(temp,Property::nstr->str[p->name]);
 len = strlen(temp) >> 1;
 tempx = x + (dw >> 1) - ((len >> 1) << 4);
 cnstring->PrintString(temp,pcolor,tempx,tempy,(dw >> 4));
 pline++;

 GenerateProperty(pcolor,p->life,pnum++,pline++);
 if(!ismonster)
 {
  GenerateProperty(pcolor,p->attack,pnum++,pline++);
  GenerateProperty(pcolor,p->defence,pnum++,pline++);
 }

 delete temp;

 return;
}

void Window::PrintNumber(int pline,int pcolor,int pnum)
{
 char *temp = new char[10];

 int tempx,tempy;
 int len;

 tempy = (pline << 4) + y;
 NumtoStr(pnum,temp);
 len = strlen(temp) >> 1;
 tempx = x + (dw >> 1) - ((len >> 1) << 4);
 cnstring->PrintString(temp,pcolor,tempx,tempy,480);

 delete temp;
}


void Window::GenerateProperty(int pcolor,int pproperty,int pnum,int pline)
{
 char *num = new char[10];
 char *temp = new char[10];
 int tempx,tempy;
 int len;

 tempy = (pline << 4) + y;
 strcpy(temp,cnstring->str[pnum]);
 NumtoStr(pproperty,num);
 strcat(temp,num);
 len = strlen(temp) >> 1;
 tempx = x + (dw >> 1) - ((len >> 1) << 4);
 cnstring->PrintString(temp,pcolor,tempx,tempy,(dw >> 4));

 delete num;
 delete temp;

 return;
}


void Window::NumtoStr(int pnum,char pstr[])
{
 int i = 0;
 int j = 0;
 char temp[10];

 while(pnum > 0)
 {
  temp[i] = Property::nstr->str[numtable][ (pnum % 10) << 1 ];
  temp[i+1] = Property::nstr->str[numtable][ ( ( pnum % 10) << 1 ) + 1 ];
  pnum /= 10;
  i += 2;
 }
 /* Now i / 2 is the number of characters */
 for( j = 0 ; j < i ; j += 2 )
 {
  pstr[ j ] = temp[ i - j - 2 ];
  pstr[ j + 1 ] = temp[ i - j - 1 ];
 }
 pstr[j] = pstr[j+1] = NULL;

 return;
}


/***************************************************************/
/* This function will do the same thing as DrawBack(), but the */
/* parameter of this one has different meaning, though in the  */
/* same name. 																   */
/***************************************************************/
/* px : the block on x direction (start from 0) 					*/
/* py : .............y.......................						*/
/* pdw: the number of blocks to draw on x direction			   */
/* pdh: ................................y..........            */
/***************************************************************/
inline void Window::DrawGrid(int px,int py,int pdw,int pdh)
{
 DrawBack(px << 4,py << 4,pdw << 4,pdh << 4);
}
