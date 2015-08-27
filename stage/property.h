#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif

/*****************************************************************/
/* Some implementation of this class is achieved inline in the   */
/* class definition                                                           */
/*****************************************************************/
class Property
{
public:
 /* Basic property */
 static CnString *nstr;
 int name;        /* Take care: we may use Chinese characters so... */
 int attack;
 int life;
 int defence;

 /* Intrinsic Property */
 int speed;
 int strength;
 int intelligence;

 /* Defensive property */
 int poision_defence;
 int fire_defence;
 int water_defence;
 int wind_defence;

 /* Status property */
 int status;        /* Dead, poision, dazed, dizzy....... */

 Property(int pname = 0,int pattack = 0,int plife = 0,int pdefence = 0);
 void SetBasic(int pname,int pattack,int plife,int pdefence);
 void SetIntrinsic(int pspeed,int pstrength,int pintelligence);
 void SetDefence(int ppoision,int pfire,int pwater,int pwind);

};

CnString *Property::nstr = NULL;//new CnString(".\\script\\name.txt",NULL);
Property property[9] =
{
    Property(0,10,70,10),           //ÓÈÀûÎ÷Ë¹
    Property(1,50,100,30),          //À­ÏÄ
    Property(2,15,80,0),          //Ñ×ÀÇ
    Property(3,30,120,20),          //ÈýÍ·Ä§ÀÇ
    Property(4,10,40,50),       	//ÓÄÁé
    Property(5,10,100,60),          //±¦Ïä¹Ö
    Property(6,40,120,50),      	//Ë®Áú
    Property(7,50,130,55),      	//Ñ×Áú
    Property(8,80,200,60),          //Ä§Íõ
};

