/************************************************************/
/*                                                          */
/*                      BMP FILE LIBRARY                    */
/*                                                          */
/*                MADE BY WANG ZIQI, ZJU, 2011              */
/*                      FOR OOP CLASS                       */
/************************************************************/

#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif



typedef unsigned short int  U16;
typedef unsigned char       U8;
typedef unsigned long int   U32;
typedef char                S8;
typedef signed short int    S16;
typedef signed long int     S32;



class bitmap_head
{
 public:
     virtual void Read(FILE *fp) {};
     virtual void Print() {};
     //virtual bool IsValid() { return validity == 1; }
 protected:
     U8  validity;
};



class bf_head : public bitmap_head
{
 public:
     void Read(FILE *fp);
     void Print();

     U8     bftype[2];          /* For windows it's 'BM' */
     U32    bfsize;             /* Total size */
     U16    bfreserved1;        /* Always 0 */
     U16    bfreserved2;        /* Always 0 */
     U32    bfoffbits;          /* Offset of bit data */
 private:
};

class bi_head : public bitmap_head
{
 public:
     void Read(FILE *fp);
     void Print();

     U32    bisize;             /* The size of this structure */
     U32    biwidth;            /* Width of the image */
     U32    biheight;           /* Height of the image */
     U16    biplanes;           /* Always 0 */
     U16    bibitcount;         /* How many bytes are used to represent a pixel(color depth) */
     U32    bicompression;      /* Compression Type: 0 None; 1 RLE 8; 2:RLE 4; 3:Determined by the mask of each pixel  */
     U32    bisizeimage;        /* This is the size only of bit data. Must be 4n */
     U32    bixpelspermeter;    /* Resolution on x direction */
     U32    biypelspermeter;    /* Resolution on y direction */
     U32    biclrused;          /* Color used. 0 = Use all. For 256 color bmp must be 100h(0x100) */
     U32    biclrimportant;     /* Important color number. 0 = all important */
 private:
};


class bmp_palette
{
 public:
     U8     blue;
     U8     green;
     U8     red;
     U8     reserved;           /* Always 0 */
};



class bitmap
{
 public:
     bitmap(const char *filename,SVGA *s);
     ~bitmap();
     void Read();
     void Print();
     void ReadLine(int line);
     void InitPalette();            /* This should be done manually */
     void Split();
     void Close();
     U32  GetHeight();
     U32  GetWidth();

     U32         bytesperline; /* This is the real bytes(including filling 0) */
     U8        *data[4][4];
     U8          *line;         //It's used for a buffer in ReadLine()
     SVGA          *scr;         //And don't forget to free() these two blocks of memory
     bf_head     bfhead;
     bi_head     bihead;
     //U16             w;             /* w and h are both the dimension of single character */
     //U16             h;            /* w << 2 and h << 2 is the real dimension of the bitmap */
     //U8              trans_color;
 private:
     void        CheckBuffer();
     int             buffervalid;
     bmp_palette palette;
     FILE        *fp;
     void SetPalette(U8 n);
     void ReadPalette(U8 n);
};




