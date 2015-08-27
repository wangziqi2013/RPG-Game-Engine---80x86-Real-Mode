#ifndef _STDLIB_H

#define _STDLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif


typedef void far *far_ptr;

class EMM;
class XMS;


class int_driver
{
 public:
     int_driver() {};
     virtual ~int_driver() = 0 {};
     virtual void Change(far_ptr address) = 0;
     virtual void Uninstall() = 0;
 private:
     int_driver(int_driver&);
     virtual void Install(far_ptr address) = 0;
 protected:
    far_ptr old_address;
};



class Timer : public int_driver
{
 public:
   Timer() {}       //We can have null constructor
    Timer(far_ptr address);
    ~Timer() { Uninstall(); };
   void Change(far_ptr address);
   void Uninstall();
   void Install(far_ptr address);
 private:
  Timer(Timer &);
};



class Graphic
{
 public:
  Graphic() {}
  virtual void Init() = 0;
  virtual void Exit() = 0;
  virtual void Change(unsigned char) = 0;
  virtual void DrawLine(unsigned char color,int x,int y,int w,int h) = 0;
  virtual void DrawPixel(unsigned char color,int x,int y) = 0;
  virtual void DrawBlock(void far *buffer,int x,int y,int w,int h) = 0;
  virtual void RetrieveBlock(void far *buffer,int x,int y,int w,int h) = 0;
  virtual void Flush() = 0;
 private:
  Graphic(Screen &);
};

class VGA : public Graphic
{
 public:
  VGA() {}
  void Init();
  void Exit();
  void Change(unsigned char);
  void DrawLine(unsigned char color,int x,int y,int w,int h);
  void DrawPixel(unsigned char color,int x,int y);
  void DrawBlock(void far *buffer,int x,int y,int w,int h);
  void RetrieveBlock(void far *buffer,int x,int y,int w,int h);
  void Fulsh() {}   /* Null function, just for compatibility */
 private:
  VGA(VGA &);
};


class SVGA : public Graphic
{
 public:
  SVGA();
  void Init();
  void Exit();
  void Change(unsigned char);
  void DrawLine(unsigned char color,int x,int y,int w,int h);
  void DrawPixel(unsigned char color,int x,int y);
  void DrawBlock(void far *buffer,int x,int y,int w,int h);
  void RetrieveBlock(void far *buffer,int x,int y,int w,int h);
  void Flush();

  EMM xmsbuffer;
 private:
  void ChangePage(int page);
  SVGA(SVGA &);
  int page;
};

void KeyUp(char);
void KeyDown(char);

/* These two will be called in the ISR, so can't be member */
extern void(far *onup)(char) = (void(far *)(char))KeyUp;
extern void(far *ondown)(char) = (void(far *)(char))KeyDown;

/* These three will be changed during ISR so can't be member */
/* and you must clear them manually before finishing keyboard precessing */
extern unsigned char key_up = 0;
extern unsigned char key_down = 0;
extern unsigned char key_code = 0;

class Keyboard : public int_driver
{
 public:
  Keyboard();
  Keyboard(far_ptr driver);
  ~Keyboard();
  void Uninstall();
  void Change(far_ptr driver);
  void Install(far_ptr driver);
  void RegisterKeyDown(void(far *func)(char)) { ondown = func; }
  void RegisterKeyUp(void(far *func)(char)) { onup = func; }
 private:
  Keyboard(Keyboard &);
};


class FlyBack
{
public:
 int OntheFly();
 FlyBack();
private:
 int action_flag;
};
