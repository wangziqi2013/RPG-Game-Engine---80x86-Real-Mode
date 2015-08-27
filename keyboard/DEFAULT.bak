void interrupt ProcessKey()
{
 unsigned char keycode;

 asm push ax

 asm in al, 60h
 asm mov keycode, al
 /* No registers need to be saved here */
 if(keycode & 0x80)
  ((void(*)(char))(onup))(keycode);
 else
  ((void(*)(char))(ondown))(keycode);

 /* We should first process the key and then enable interrupt   */
 /* or we may get a nested interrupt call (which is disastrous) */
 //NotifyKeyboard();
 /* To notify the keyboard that we have retrived the char */
 SendEOI();

 asm pop ax
}


/**************************************************************/
/* These two is the default function to process key event     */
/**************************************************************/
/* chr : The scan code(not ascii) in the port                 */
/**************************************************************/

void KeyDown(char chr)
{
 //printf("KeyDown: %u  ",chr);
 key_down = 1;
 key_code = chr;
 if(chr == 1) exit(0);
}


void KeyUp(char chr)
{
 //printf("KeyUp: %u  ",chr);
 key_up = 1;
 key_code = chr;
}
