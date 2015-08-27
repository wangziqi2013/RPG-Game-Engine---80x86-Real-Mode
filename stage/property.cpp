#include ".\stage\property.h"



Property::Property(int pname,int pattack,int plife,int pdefence)
{
 SetBasic(pname,pattack,plife,pdefence);
 return;
}


void Property::SetBasic(int pname,int pattack,int plife,int pdefence)
{
 name = pname;
 attack = pattack;
 life = plife;
 defence = pdefence;

 return;
}


void Property::SetIntrinsic(int pspeed,int pstrength,int pintelligence)
{
 speed = pspeed;
 strength = pstrength;
 intelligence = pintelligence;
}



void Property::SetDefence(int ppoision,int pfire,int pwater,int pwind)
{
 poision_defence = ppoision;
 fire_defence = pfire;
 water_defence = pwater;
 wind_defence = pwind;
}
