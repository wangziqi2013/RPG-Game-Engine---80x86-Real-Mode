#include ".\mem\mem.h"

unsigned char DynamLoad(const char *);

////////////////////////////////////////////////////////
//                 XMS MEMORY CONTROL                 //
//                                                    //
//                 MADE BY WANG ZIQI                  //
//                                                    //
//                  FOR OOP PROJECT                   //                 
//                                                    //
////////////////////////////////////////////////////////

/* Actually NTVDM automatically support XMS, but as a */
/* routine we must check it before using.                  */


/******************************************************/
/* This function will enable the 20th address line on */
/* 8042 chip. Activate A20 before using XMS (or the ) */
/* (result will be disastrous)                             */
/******************************************************/
void XMS::EnableA20()
{
 DynamLoad("ENA20.com");
}

/******************************************************/
/* This function will check whether the driver of XMS */
/* has been installed on the DOS (NTVDM also)          */
/******************************************************/
/* return value : 0x80 if driver is valid, or else    */
/* XMS not installed                                               */
/******************************************************/
unsigned char XMS::Test()
{
 unsigned char temp;

 asm push ax

 asm mov ax, 4300h
 asm int 0x2f
 asm mov byte ptr [temp], al

 asm pop ax

 return temp;
}


void far *XMS::GetDriver()
{
 void far *drv = (void far *)0xffff0000;
 /* Set this initial value so that if the call failed */
 /* but someone insist on using it, this address will */
 /* bring the computer to restart (BIOS ROM START)    */
 asm push ax
 asm push es
 asm push bx

 asm mov ax, 4310h
 asm int 2fh
 asm mov word ptr [drv], bx
 asm mov word ptr [drv+2], es
 /* low : offset; high : segment */

 asm pop bx
 asm pop es
 asm pop ax

 return drv;
}


/*********************************************************/
/* This function will allocate a block of XMS memory,    */
/* and return the handle. If the allocation failed then  */
/* this function won't return, and call FatalError       */
/*********************************************************/
/* NOTICE: The handle must be saved immediately after    */
/* being returned                                                      */
/*********************************************************/
/* kbsize : total size allocated by XMS. Must be in KB   */
/*********************************************************/
unsigned short int XMS::Allocate(int kbsize)
{
 void far *drv = driver;
 //printf("Allocated: %d",kbsize);

 unsigned short int handle = 0;
 asm push ax
 asm push dx

 asm mov ah, 9h
 asm mov dx, word ptr kbsize
 asm call dword ptr drv

 asm cmp ax, 0h
 asm je fail
 asm mov word ptr handle, dx

 asm pop dx
 asm pop ax

 return handle;
fail:
 FatalError e("Can't allocate XMS!");
 e.Print();
 return 0;
}

/*****************************************************/
/* This function will set the member driver to the   */
/* address returned by interrupt 4310h                */
/*****************************************************/
void XMS::Install()
{
 void far *drv = (void far *)0xffff0000;

 drv = GetDriver();
 driver = drv;

 printf("XMS Install successful, address at %lx\n",driver);

 return;
}


XMS::XMS()
{
 EnableA20();

 if(Test() != 0x80)
 {
  FatalError noxms("No XMS driver detected!");
  noxms.Print();
 }
 Install();

 return;
}


/*********************************************************/
/* This function will check the A20 line                       */
/*********************************************************/
/* There's no return value. All information will be      */
/* printed in the function                                         */
/*********************************************************/
void XMS::CheckA20()
{
 void far *drv = (void far *)driver;

 asm push ax
 asm push bx

 asm mov ah, 07h
 asm mov bx, 0ffffh         /* Just for fun */
 asm call dword ptr [drv]

 asm cmp ax, 0h
 asm je fail
 asm cmp bl, 0h
 asm jne fail

 printf("A20 Check successful!\n");

 asm pop bx
 asm pop ax

 return;
fail:
 GeneralError a20("A20 Detect Error!");
 a20.Print();
}


/*******************************************************/
/* This function only prints the information. No ret   */
/*******************************************************/
void XMS::MaxXMS()
{
 unsigned short int max = 0;
 unsigned short int avail = 0;

 void far *drv = driver;

 asm push ax
 asm push bx
 asm push dx

 asm mov ax, 0ffffh
 asm mov dx, 0ffffh
 asm mov ah, 8h
 asm call dword ptr [drv]

 asm mov word ptr [max], ax
 asm mov word ptr [avail], dx

 printf("Maximum XMS memory: %uKB; Total available: %uKB\n",max,avail);

 asm pop dx
 asm pop bx
 asm pop ax
}

/***************************************************************/
/*                                                                                 */
/*                        EMM MEMBER FUNCTION                     */
/*                                                                             */
/*                                                                                  */
/***************************************************************/

void EMM::CallDriver()
{
 static void far *drv = xms -> driver;
 static XMSPara *par = para;
 static unsigned char errcode = 0;
 //unsigned short ahret = 0;
 //unsigned long temp;

 asm push ds
 asm push si
 asm push ax
 asm push bx

 asm mov ah, 0bh        /* Move memory block(4 direction both OK) */
                            /* We inline it here by hand instead of writing as a function to save time! */
 asm mov si, word ptr [par]
 asm mov ds, word ptr [par+2]

 //asm mov bx, ds:[si]
 //asm mov word ptr [temp], bx
 //asm mov bx, ds:[si+2]
 //asm mov word ptr [temp+2], bx
 //printf("Size = %lu   sizeof = %d\n",temp,sizeof(XMSPara));

 asm call dword ptr [drv]
                            /* Call the driver with a far call */
 //asm mov byte ptr errcode, bl
 //asm mov word ptr ahret, ax

 //printf("%lu,%d,%lu,%d,%lu\n",*par,*(par+4),*(par+6),*(par+10),*(par+12));

 //printf("AX = %X  BL = %X\n",ahret,errcode);
 //while(1);

 asm cmp ax, 0
 asm je fail

 asm pop bx
 asm pop ax
 asm pop si
 asm pop ds

 return;
fail:
 asm mov byte ptr errcode, bl
 FatalError e("XMS Coping Error!");
 printf("Error code: %x\n",errcode);
 e.Print();
}

/***********************************************/
/* If there's no parameter or a 0, we will not */
/* allocate memory for EMM. Call Allocate()    */
/* later by hand.                                         */
/***********************************************/
EMM::EMM(unsigned int allocsize)
{
 if(allocsize > 0)
  handle = xms -> Allocate(allocsize);
 else
  return;
}

EMM::~EMM()
{
 DeAllocate();
}

void EMM::DeAllocate()
{
 void far *drv = xms->driver;
 unsigned char errcode = 0;
 unsigned int temp = handle;

 asm push ax
 asm push bx

 asm mov ah, 0Ah
 asm mov dx, word ptr temp
 asm call dword ptr [drv]
 asm cmp ax, 0h
 asm jne ret
 asm mov byte ptr [errcode], bl
 printf("XMS Free error! code: %X",errcode);

ret:
 asm pop bx
 asm pop ax

 return;
}


inline void EMM::Allocate(unsigned int allocsize)
{
 handle = xms -> Allocate(allocsize);
}

void EMM::OuttoNor(unsigned char far *ptargetaddress,unsigned long offset,unsigned long psize)
{
 para->targetaddress = ptargetaddress;  /* CS:IP and IP is on the lower word(a far *) */
 para->targethandle = 0;                    /* Normal memory */
 para->size = psize;
 para->sourceaddress = (unsigned char *)offset;  /* I hope this won't lose information... */
 /* If source handle != 0 then this is the offset into this XMS memory block */
 para->sourcehandle = handle;               /* Copy from this XMS block */

 CallDriver();
}
void EMM::InfromNor(unsigned char far *psourceaddress,unsigned long offset,unsigned long psize)
{
 para->targetaddress = (unsigned char far *)offset;
 para->targethandle = handle;
 para->size = psize;
 para->sourceaddress = psourceaddress;
 para->sourcehandle = 0;

 CallDriver();
}

/************************************************************/
/* These two functions will move memory blocks between XMSs */
/* and we always put the destination on the left side on    */
/* parameter table                                                     */
/************************************************************/
void EMM::OuttoXMS(unsigned short int ptargethandle,unsigned long doffset,unsigned long soffset,unsigned long psize)
{
 para->targetaddress = (unsigned char far *)doffset;
 para->targethandle = ptargethandle;
 para->size = psize;
 para->sourceaddress = (unsigned char far *)soffset;
 para->sourcehandle = handle;

 CallDriver();
}
void EMM::InfromXMS(unsigned short int psourcehandle,unsigned long doffset,unsigned long soffset,unsigned long psize)
{
 para->targetaddress = (unsigned char far *)doffset;
 para->targethandle = handle;
 para->size = psize;
 para->sourceaddress = (unsigned char far *)soffset;
 para->sourcehandle = psourcehandle;

 CallDriver();
}



//AH�зŹ��ܺ�,��Զ���õķ�ʽ������������
// BL�з��صĴ������
// 80hû���ṩ�Ĺ���
// 81h��⵽������(Vdisk)
// 82h����A20��ַ�ߴ���
// 8Ehһ�������������
// 8Fh�����������������
// 90h�߶��ڴ�(HMA)������
// 91h�߶��ڴ�(HMA)�ѱ�ʹ��
// 92hDXislessthanthe/HMAMIN=parameter
// 93h�߶��ڴ�(HMA)δ������
// 94hA20��ַ���ѱ�����
// A0h���������ڴ��ѱ�����
// A1h���п��õľ���ѱ�����
// A2h��Ч�ľ��
// A3h��Ч��Դ���
// A4h��Ч��Դƫ��
// A5h��Ч��Ŀ�ľ��
// A6h��Ч��Ŀ��ƫ��
// A7h��Ч�ĳ���
// A8h�ƶ��зǷ����ص�
// A9h������żУ�����
// AAh��δ����
// ABh���ѱ�����
// ACh�������������
// ADh����ʧ��
// B0hֻ�и�СһЩ��UMB�ռ�
// B1hû�п��õ�UMB�ռ�