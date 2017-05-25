#include <stdio.h>

//-----------------------------------------------------------------------------
double EiEo(double tableER[][105],double Tp,double Rp){
  if(Tp<0.1||Tp>1000.)
  {printf("Energy is out of range\n"); return -1.;}
  
  int look=0;
  while (Tp>tableER[0][look]) {look++;}

  double R1=tableER[2][look-1]+
  +tableER[3][look-1]*Tp+
  +tableER[4][look-1]*Tp*Tp+
  +tableER[5][look-1]*Tp*Tp*Tp;
  
  double R2 = R1 - Rp;
  
  if(Rp>0.)
  {if(R2<tableER[1][0]) {return 0.;}}
  else
  {if(R2>=tableER[1][104]) {return -2.;
  printf("table is out of range\n");}}
  
  look=0;
  while (R2>tableER[1][look]) {look++;}
  
  double E1=tableER[6][look-1]+
  +tableER[7][look-1]*R2+
  +tableER[8][look-1]*R2*R2+
  +tableER[9][look-1]*R2*R2*R2;
  
  return E1;
};