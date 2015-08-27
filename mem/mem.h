/***************************************************/
/* This class must be contained in the class using */
/* XMS service while XMS itself will not use it    */
/* but some functions will take EMM * as parameter */
/***************************************************/
/* Used for memory transmitting                             	*/
/* When the XMS is being used as a register parameter */
/* DS:SI must be set to a valid one. So a far point may be needed */


#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif


class XMS;

class XMSPara
{
public:
 /* NOTICE: we must guarantee the four member is consecutive */
 /* both in the memory and in definition. Because DOS ISR    */
 /* will take advantage of this                                 	 */
 unsigned long        size;              /* Notice: This is measured in byte, not KB */
 unsigned short int sourcehandle;      /* 0 represents <= 640k (only used for XMS memory)*/
 unsigned char far *sourceaddress;     /* NOTICE here we need both a segment and an offset */
 unsigned short int targethandle;      /* 0 represents <= 640k (only used for XMS memory)*/
 unsigned char far *targetaddress;     /* NOTICE here we need both a segment and an offset */
 /* NOTICE: When the handle is not 0, which means that the memory is on the higher */
 /* memory. This time the target address means the offset(32bit) into our allocated */
 /* memory block (so that we can access the interior) */
};

class EMM
{
public:
 static XMSPara far *para;                  /* Both used to change parameter and as the address of the parameter */
 unsigned short handle;                     /* This entry stored the XMS handle owned by this EMM */
                                                                     /* and must be initialized as soon as possible */
 EMM(unsigned int allocsize = 0);
 ~EMM();
 void OuttoNor(unsigned char far *targetaddress,unsigned long offset,unsigned long size);
 void InfromNor(unsigned char far *sourceaddress,unsigned long offset,unsigned long size);
 /* The offset means offset into XMS, not into normal memory */
 void OuttoXMS(unsigned short int targethandle,unsigned long doffset,unsigned long soffset,unsigned long size);
 void InfromXMS(unsigned short int sourcehandle,unsigned long doffset,unsigned long soffset,unsigned long size);

 void Allocate(unsigned int allocsize);
 void DeAllocate();

 static XMS *xms;
private:
 EMM(EMM &);
 void CallDriver();
};


class XMS
{
public:
 XMS();
 unsigned char  Test();
 void           EnableA20();
 void           Install();
 void far *     GetDriver();
 unsigned short int Allocate(int kbsize);
 void Free();
 void CheckA20();
 void MaxXMS();

 friend class EMM;
private:
 XMS(XMS &);
 void far *driver;
};

static XMS Memory;
static XMSPara Para;
/* We need a global memory manager to control */
/* global work.(XMS can't be installed twice) */
XMS *EMM::xms = &Memory;
XMSPara far *EMM::para = (XMSPara far *)(&Para);
