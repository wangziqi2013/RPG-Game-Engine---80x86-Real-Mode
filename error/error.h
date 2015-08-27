#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif



class Error
{
public:
 Error(const char *info) { strcpy(errtext,info); }
protected:
 virtual Print() const = 0;
 char errtext[80];
};



class FatalError : public Error
{
public:
 FatalError(const char *info) : Error(info) {};
 Print() const { printf(errtext); getchar(); exit(0); }
};


class GeneralError : public Error
{
public:
 GeneralError(const char *info) : Error(info) {};
 Print() const { printf(errtext); }
};


void FileNotFound(const char *info)
{
 FatalError e(info);
 e.Print();
 return;
}


void NotEnoughMemory()
{
 FatalError e("Not Enough Memory!\n");
 e.Print();
 return;
}
