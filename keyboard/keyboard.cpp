
#include ".\keyboard\keyport.cpp"
#include ".\keyboard\default.cpp"
#include ".\keyboard\keyboard.h"


void interrupt ProcessKey2()
{
 printf("Process Key 2 Installed.");
 SendEOI();
}

void interrupt ProcessKey3()
{
 printf("Process Key 3 Installed.");
 SendEOI();
}

/**************************************************************/
/* This function will install the keyboard process function   */
/* as the driver                                              */
/* Notice that the interrupt should generally set segment reg */
/* according to current CS but here bacause we'll use small   */
/* compile mode, so when interrupt comes, DS is always equal  */
/* to CS, so there's no need to update DS and ES.             */
/**************************************************************/
void Keyboard::Install(far_ptr address)
{
 far_ptr int_9h_local;

 asm push ax
 asm push es
 asm push bx
 asm push cx

 asm cli

 asm xor ax, ax
 asm mov es, ax

 asm mov ax, 9h
 asm mov cl, 2h
 asm shl ax, cl
 /* Because each item in the table is 4 bytes long */
 asm mov bx, ax

 asm mov ax, es:[bx]
 asm mov word ptr [int_9h_local], ax
 asm mov ax, es:[bx+2]
 asm mov word ptr [int_9h_local+2], ax

 asm mov ax, word ptr [address]
 asm mov es:[bx], ax
 asm mov ax, word ptr [address+2]
 asm mov es:[bx+2], ax

 asm sti

 asm pop cx
 asm pop bx
 asm pop es
 asm pop ax

 old_address = int_9h_local;
}


/***********************************************************/
/* This function will change the existing driver to a new  */
/* one without modifying the stored previous address       */
/***********************************************************/
void Keyboard::Change(far_ptr address)
{
 asm push ax
 asm push es
 asm push bx
 asm push cx

 asm cli

 asm xor ax, ax
 asm mov es, ax

 asm mov ax, 9h
 asm mov cl, 2h
 asm shl ax, cl
 /* Because each item in the table is 4 bytes long */
 asm mov bx, ax

 asm mov ax, word ptr [address]
 asm mov es:[bx], ax
 asm mov ax, word ptr [address+2]
 asm mov es:[bx+2], ax

 asm sti

 asm pop cx
 asm pop bx
 asm pop es
 asm pop ax
}


/********************************************************/
/* This function will uninstall a existing driver and   */
/* retrieve from the class member the previous address  */
/********************************************************/

void Keyboard::Uninstall(void)
{
 far_ptr int_9h_local = old_address;

 asm push ax
 asm push bx
 asm push cx
 asm push es

 asm cli

 asm xor ax, ax
 asm mov es, ax

 asm mov ax, 9h
 asm mov cl, 2h
 asm shl ax, cl
 asm mov bx, ax

 asm mov ax, word ptr [int_9h_local]
 asm mov es:[bx], ax
 asm mov ax, word ptr [int_9h_local+2]
 asm mov es:[bx+2], ax

 asm sti

 asm pop es
 asm pop cx
 asm pop bx
 asm pop ax
}



/***********************************************/
/* Constructor & Destructor for class Keyboard */
/***********************************************/
Keyboard::Keyboard(void far *driver)
{
 Install(driver);
}

Keyboard::~Keyboard()
{
 Uninstall();
}
