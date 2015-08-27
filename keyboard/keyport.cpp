
/********************************************************/
/* This function is used to send a function to 8259 and */
/* enable a new interrupt. If efficiency is important   */
/* then we should use the inline version                */
/********************************************************/
void SendEOI()
{
 asm push ax
 asm mov al, 20h
 asm out 20h, al
 asm pop ax
}

/****************************************************/
/* To notify the 8048 MCU that we have alredy taken */
/* the char on the port so that it can present next */
/****************************************************/
void NotifyKeyboard()
{
 asm push ax
 asm in al, 61h
 asm or al, 80h
 asm out 61h, al
 asm in al, 61h
 asm and al, 7fh
 asm out 61h, al
 asm pop ax
}
