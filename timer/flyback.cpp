
FlyBack::FlyBack()
{
 action_flag = 1;
}




inline int FlyBack::OntheFly()
{
 unsigned char port = inportb(0x3da);

 if( ((port & 0x08) != 0) && (action_flag == 0) )
 {
  action_flag = 1;
  return 1;
 }
 else if(( (port & 0x08) == 0) && (action_flag != 0) )
 {
  action_flag = 0;
 }
 return 0;
}
