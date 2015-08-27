unsigned char DynamLoad(const char *filename)
{
 unsigned char far *func;
 FILE *fp;
 unsigned long size = 0;
 unsigned char ax;

 fp = fopen(filename,"rb");
 if(fp == NULL)
 {
  FatalError e("File open error during dynamic loading");
  e.Print();
 }

 fseek(fp,0,2);
 size = ftell(fp);
 fseek(fp,0,0);



 func = (unsigned char *)malloc(size);
 if(func == NULL)
 {
  FatalError e("Not enough memory for dynamic loading.\n");
  e.Print();
 }

 fread(func,size,1,fp);

 asm call dword ptr [func]
 ax = _AX;

 free(func);
 fclose(fp);

 return ax;
}
