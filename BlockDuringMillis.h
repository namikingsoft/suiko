#ifndef __BlockDuringMillis_h__
#define __BlockDuringMillis_h__

class BlockDuringMillis
{
private:
  unsigned long duringMillis;
  unsigned long prevMillis;
  
public:
  BlockDuringMillis(int const duringMillis);
  bool isBlock();
  void setDuringMillis(int const duringMillis);
  void resetPrevMillis();
};

#endif /* __BlockDuringMillis_h__ */
