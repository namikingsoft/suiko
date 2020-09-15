#ifndef __BlockDuringMillis_h__
#define __BlockDuringMillis_h__

class BlockDuringMillis
{
private:
  unsigned long duringMillis;
  unsigned long prevMillis;
  
public:
  BlockDuringMillis(int duringMillis);
  bool isBlock();
  void setDuringMillis(int duringMillis);
};

#endif /* __BlockDuringMillis_h__ */
