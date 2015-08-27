#include ".\bmp\bmp.h"


void bf_head::Read(FILE *fp)
{
 if(fp == NULL) exit(0);

 fread(&bftype[0],14,1,fp); //The size of bf_head is 14d

 return;
}


void bi_head::Read(FILE *fp)
{
 if(fp == NULL) exit(0);

 fread(&bisize,40,1,fp);    //The size of bi_head is 40d

 return;
}



inline U32 bitmap::GetHeight()
{
 return bihead.biheight;
}

inline U32 bitmap::GetWidth()
{
 return bihead.biwidth;
}


void bitmap::Read()
{
 if(fp == NULL) printf("NULL pointer!");

 fseek(fp,0L,0);    //Move the file pointer to the beginning then read

 bfhead.Read(fp);
 bihead.Read(fp);

 U16 width = (short)bihead.biwidth;

 //w = bihead.biwidth >> 2;
 //h = bihead.biheight >> 2;

 asm push ax
 asm push cx

 asm mov ax, width
 asm mov cl, 2
 asm shr ax, cl

 asm mov cx, width
 asm test cx, 3h
 asm je ok
 asm inc ax
ok:
 asm mov cl, 2
 asm shl ax, cl
 asm mov width, ax

 asm pop cx
 asm pop ax

 //Then we'll calculate two more relevant parameters to make the information more clear
 bytesperline = (U32)width;//( (bihead.biwidth >> 2) + ( (bihead.biwidth & 0x0003) != 0x0000 ) ) << 2;
  
 return;
}


void bitmap::Print()
{
 bfhead.Print();
 bihead.Print();
 printf("\n Bytes per line: %lu \n\n",bytesperline);
}


void bf_head::Print()
{
 putchar('\n');
 printf("Bitmap file\n");
 printf("--------------------------------------------\n");
 printf("Bitfile type: %c%c\n", bftype[0], bftype[1]);
 printf("Total size of the file: %lu\n", bfsize);
 printf("Reserved(1): %u\n",  bfreserved1);
 printf("Reserved(2): %u\n",  bfreserved2);
 printf("Offset of bit data: %lu\n", bfoffbits);
 putchar('\n');

 return;
}

void bi_head::Print()
{
 putchar('\n');
 printf("Bitmap information\n");
 printf("--------------------------------------------\n");
 printf("Size of this structure: %lu\n", bisize);
 printf("Width: %lu\n", biwidth);
 printf("Height(Always 0): %lu\n", biheight);
 printf("Planes(Always 1): %u\n", biplanes);
 printf("Color depth: %u\n", bibitcount);
 printf("\tEvery pixel uses %u byte(s)\n", bibitcount >> 3);
 printf("Compression Type: %lu\n", bicompression);
 switch( bicompression)
 {
  case 0:
        printf("\tNo compression\n");
        break;
  case 1:
        printf("\tRLE8\n");
        break;
  case 2:
        printf("\tRLE4\n");
        break;
  case 3:
        printf("\tBit fields\n");
        break;
  default:
        printf("\tUnknown\n");
        break;
 }
 printf("Size of the image: %lu\n", bisizeimage);
 printf("\tMust be divided by 4\n");
 printf("Resolution on x(pixel/meter): %lu\n", bixpelspermeter);
 printf("Resolution on y(pixel/meter): %lu\n", biypelspermeter);
 printf("Color used: %lu\n", biclrused);
 printf("\t0 = Use all\n");
 printf("Important color number: %lu\n", biclrimportant);
 printf("\t0 = All important\n");
 putchar('\n');
}


/****************************************************/
/* This routine will check whether the line buffer  */
/* is ready, if not, it will automatically allocate */
/* one block.                                                    */
/****************************************************/
/* Recommended to be used before any action using   */
/* void far *line                                            */
/****************************************************/
/* Notice: Must be used after Read() for it make    */
/* of some parameters initialized during Read()      */
/* And don't forget to free it! (In the destructor) */
/****************************************************/
void bitmap::CheckBuffer()
{
 if(!buffervalid)
 {
  line = (U8 *)malloc(bihead.biwidth);
  buffervalid = 1;  /* Don't forget this!!! */

  if(line == 0)
  {
    FatalError e("Memory out!!");
    e.Print();
  }
 }
}

/*****************************************************/
/* The destructor close fp and check whether line is */
/* valid. If it was then it should be freed           */
/*****************************************************/
bitmap::~bitmap()
{
 //Close();

 if(buffervalid) free(line);
}



/***************************************************/
/* The constructor will allocate buffer by calling */
/* CheckBuffer()                                               */
/***************************************************/
bitmap::bitmap(const char *filename,SVGA *s)
{
 fp = fopen(filename,"rb");
 scr = s;

 if(fp == NULL)
 {
  FatalError e("Can't open bmp file!");
  e.Print();
 }

 buffervalid = 0;
 Read();
 /* Only read in header, not the data */
 /* Data will be stored in the XMS, which should be contained in the user class */

 /* After Read() all parameters are OK */
 CheckBuffer();

 return;
}


void bitmap::ReadLine(int linenum)
{
 int actualline = 0;
 U32 offset = 0;

 actualline = bihead.biheight - linenum - 1;
 offset = bytesperline*actualline + bfhead.bfoffbits;

 fseek(fp,offset,0);

 CheckBuffer();

 fread(line,bihead.biwidth,1,fp);

 return;
}

void bitmap::SetPalette(U8 n)
{
 U8 r,g,b;

 r = palette.red >> 2;
 g = palette.green >> 2;
 b = palette.blue >> 2;

 asm push ax
 asm push dx

 asm mov dx, 3c8h
 asm mov al, n
 asm out dx, al

 asm mov dx, 3c9h
 asm mov al, r
 asm out dx, al
 asm mov al, g
 asm out dx, al
 asm mov al, b
 asm out dx, al

 asm pop dx
 asm pop ax
}


void bitmap::ReadPalette(U8 n)
{
 fseek(fp,14+40+n*4,0); //14+40 is the length of two headers(this will not be changed)

 fread(&palette,4,1,fp);

 return;
}


void bitmap::InitPalette()
{
 int i = 0;

 for( i = 0 ; i < 256 ; i++ )
 {
  ReadPalette(i);
  SetPalette(i);
 }

 return;
}


/**************************************************/
/* This function is of no use!!!!!! (Very Old)    */
/* It can surely work, but we won't call it. Just */
/* use it as a test                                       */
/**************************************************/
/*
void bitmap::Split()
{
 int size = w * h;

 int i,j;

 for(i=0;i<4;i++)
  for(j=0;j<4;j++)
  {
     data[i][j] = (U8 *)malloc(size);
     if(data[i][j] == NULL) printf("Memory out!");
  }

 CheckBuffer();

 for(i=0;i<bihead.biheight;i++)
 {
  ReadLine(i);
  for(j=0;j<bihead.biwidth;j++)
  {
     data[i/h][j/w][(j%w) + (i%h)*w] = line[j];
  }
 }
 return;
}
************************************************/

void bitmap::Close()
{
 if(fp)
 {
  fclose(fp);
  fp = NULL;
 }
}


/**************************************************************/
/* This function will read the bitmap to the xms. You must    */
/* provide a XMS pointer EMM * to the function.                   */
/* Notice that the whole bitmap will be read linearly as a    */
/* whole.                                                                     */
/* xms must be initialized first!                                     */
/* We'll not use dynamic allocation.                                  */
/**************************************************************/
/*
void bitmap::ReadtoXMS(EMM &xmsbuffer)
{
 int i;
 int line;
 unsigned char far *address;
 unsigned long size;
 unsigned long offset;

 CheckBuffer();

 // Set transform parameters
 size = bihead.biwidth;
 offset = 0;
 address = (unsigned char far *)line;

 // For map file we must guarantee it will be transformed quickly 
 for(line = 0 ; line < bihead.biheight ; line++ )
 {
  ReadLine(line);
  xmsbuffer.InfromNor(address,offset,size);
  offset += size;
 }

 return;
}   */
