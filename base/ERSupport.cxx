#include "ERSupport.h"

#include <stdio.h>
#include <string.h>

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
//-----------------------------------------------------------------------------
int ReadRint(char* Fname,double Ranges[][105]){
  char DummyA[256];
  char termA[]="---";
  char unR[10],un3[10],un4[10],dee[10],b[10],cc[10];
  char unE[10]="begin";
  double Energy,Rng,e,r,den;
  double x[4],y[4],c[4];
  int i;
  double a[4];
  
  FILE *F2 = fopen(Fname,"r");
  if(F2==NULL) {printf("ReadRint: File %s has not been read\n",Fname);
  return 1;}
  else
  {
    printf("ReadRint: File %s has been read\n",Fname);
    while (fgets(DummyA,256,F2)) {if(strstr(DummyA,termA)) break;}
    i=0;
    while (strcmp(unE,"GeV"))
    {
      int res;
      //printf("%i\n",i);
      res = fscanf(F2,"%lf %s %s %lf %lf %s %s %s %s %s\n",&e,unE,dee,
      &den,&r,unR,b,un3,cc,un4);
      if(!strcmp(unE,"keV")) {Energy=e/1000.0;}
      else if(!strcmp(unE,"MeV")) {Energy=e;}
      else if(!strcmp(unE,"GeV")) {Energy=e*1000.0;}
      else {printf("Error of reading in ReadRint a: %s %s\n",unE,unR);}

      if(!strcmp(unR,"A")) {Rng=r*1.0E-8;}
      else if(!strcmp(unR,"um")) {Rng=r/10000.0;}
      else if(!strcmp(unR,"mm")) {Rng=r/10.0;}
      else if(!strcmp(unR,"cm")) {Rng=r;}
      else if(!strcmp(unR,"m")) {Rng=r*100.0;}
      else if(!strcmp(unR,"km")) {Rng=r*100000.0;}
      else {printf("Error of reading in ReadRint b: %s %s\n",unE,unR);}
      
      Ranges[0][i]=Energy;
      Ranges[1][i]=Rng;
      i++;
    }
    fclose(F2);
  }
  for (i=0;i<4;++i)
  {
    x[i]=Ranges[0][i];
    y[i]=Ranges[1][i];
  }
  if(intrp4(x,y,c))
  {
    printf("1 Interpolation error 1 intrp4=%i\n",intrp4(x,y,c));
    return 1;
  }

  Ranges[2][0]=c[0];
  Ranges[3][0]=c[1];
  Ranges[4][0]=c[2];
  Ranges[5][0]=c[3];
      
  for (i=0;i<4;++i)
  {
    x[i]=Ranges[1][i];
    y[i]=Ranges[0][i]; 
  }

  if(intrp4(x,y,c))
  {
    printf("2 Interpolation error 2 intrp4=%i\n",intrp4(x,y,c));return 1;
  }

  Ranges[6][0]= c[0];
  Ranges[7][0]= c[1];
  Ranges[8][0]= c[2];
  Ranges[9][0]= c[3];

  for(i=1;i<103;++i)
  {
    x[0] = Ranges[0][i-1];
    x[1] = Ranges[0][i];
    x[2] = Ranges[0][i+1];
    x[3] = Ranges[0][i+2];
    y[0] = Ranges[1][i-1];
    y[1] = Ranges[1][i];
    y[2] = Ranges[1][i+1];
    y[3] = Ranges[1][i+2];

    if(intrp4(x,y,c))
    {printf("3 Interpolation error 3 intrp4=%i\n",intrp4(x,y,c));return 1;}
    Ranges[2][i] = c[0];
    Ranges[3][i] = c[1];
    Ranges[4][i] = c[2];
    Ranges[5][i] = c[3];
  }
  Ranges[2][i] = c[0];
  Ranges[3][i] = c[1];
  Ranges[4][i] = c[2];
  Ranges[5][i] = c[3];

  Ranges[2][i+1] = c[0];
  Ranges[3][i+1] = c[1];
  Ranges[4][i+1] = c[2];
  Ranges[5][i+1] = c[3];
  
  for(i=1;i<103;++i)
  {
    y[0] = Ranges[0][i-1];
    y[1] = Ranges[0][i];
    y[2] = Ranges[0][i+1];
    y[3] = Ranges[0][i+2];

    x[0] = Ranges[1][i-1];
    x[1] = Ranges[1][i];
    x[2] = Ranges[1][i+1];
    x[3] = Ranges[1][i+2];

    if(intrp4(x,y,c))
    {
      printf("4 Interpolation error 4 intrp4=%i\n",intrp4(x,y,c));return 1;
    }
    Ranges[6][i] = c[0];
    Ranges[7][i] = c[1];
    Ranges[8][i] = c[2];
    Ranges[9][i] = c[3];
  }

  Ranges[6][i] = c[0];
  Ranges[7][i] = c[1];
  Ranges[8][i] = c[2];
  Ranges[9][i] = c[3];

  Ranges[6][i+1] = c[0];
  Ranges[7][i+1] = c[1];
  Ranges[8][i+1] = c[2];
  Ranges[9][i+1] = c[3];

  return 0;
}

//-----------------------------------------------------------------------------
int intrp4(double* x,double* y, double* c)
{
  //______________________________________________________________________
  //  returns c0,c1,c2,c3 coeff. of y= c0 + c1*x + c2*x^2 + c3*x^3 function 
  //     passing through 4 points:
  //     x1,y1; x2,y2; x3,y3; x4,y4
  //______________________________________________________________________|
  double d0,d1,d2,d3;
  double x12,x13,x22,x23,x32,x33,x42,x43;
  int rp4;

  x12 = x[0]*x[0];
  x13 = x12*x[0];
  x22 = x[1]*x[1];
  x23 = x22*x[1];
  x32 = x[2]*x[2];
  x33 = x32*x[2];
  x42 = x[3]*x[3];
  x43 = x42*x[3];

  d3=(x13*x22*x[2]-x12*x23*x[2]-x13*x[1]*x32+x[0]*x23*x32+x12*x[1]*x33-
  x[0]*x22*x33-x13*x22*x[3]+x12*x23*x[3]+x13*x32*x[3]-x23*x32*x[3]-
  x12*x33*x[3]+x22*x33*x[3]+x13*x[1]*x42-x[0]*x23*x42-x13*x[2]*x42+
  x23*x[2]*x42+x[0]*x33*x42-x[1]*x33*x42-x12*x[1]*x43+x[0]*x22*x43+
  x12*x[2]*x43-x22*x[2]*x43-x[0]*x32*x43+x[1]*x32*x43);

  c[3]=(x22*x[2]*y[0]-x[1]*x32*y[0]-x22*x[3]*y[0]+
  x32*x[3]*y[0]+x[1]*x42*y[0]-x[2]*x42*y[0]-
  x12*x[2]*y[1]+x[0]*x32*y[1]+x12*x[3]*y[1]- 
  x32*x[3]*y[1]-x[0]*x42*y[1]+x[2]*x42*y[1]+
  x12*x[1]*y[2]-x[0]*x22*y[2]-x12*x[3]*y[2]+ 
  x22*x[3]*y[2]+x[0]*x42*y[2]-x[1]*x42*y[2]-
  x12*x[1]*y[3]+x[0]*x22*y[3]+x12*x[2]*y[3]- 
  x22*x[2]*y[3]-x[0]*x32*y[3]+x[1]*x32*y[3])/d3;

  d2=(x13*x22*x[2]-x12*x23*x[2]-x13*x[1]*x32+x[0]*x23*x32+x12*x[1]*x33-
  x[0]*x22*x33-x13*x22*x[3]+x12*x23*x[3]+x13*x32*x[3]-x23*x32*x[3]-
  x12*x33*x[3]+x22*x33*x[3]+x13*x[1]*x42-x[0]*x23*x42-x13*x[2]*x42+
  x23*x[2]*x42+x[0]*x33*x42-x[1]*x33*x42-x12*x[1]*x43+x[0]*x22*x43+
  x12*x[2]*x43-x22*x[2]*x43-x[0]*x32*x43+x[1]*x32*x43);

  c[2]=(-(x23*x[2]*y[0])+x[1]*x33*y[0]+x23*x[3]*y[0]-x33*x[3]*y[0]-x[1]*x43*y[0]+
  x[2]*x43*y[0]+x13*x[2]*y[1]-x[0]*x33*y[1]- 
  x13*x[3]*y[1]+x33*x[3]*y[1]+x[0]*x43*y[1]- 
  x[2]*x43*y[1]-x13*x[1]*y[2]+x[0]*x23*y[2]+ 
  x13*x[3]*y[2]-x23*x[3]*y[2]-x[0]*x43*y[2]+
  x[1]*x43*y[2]+x13*x[1]*y[3]-x[0]*x23*y[3]- 
  x13*x[2]*y[3]+x23*x[2]*y[3]+x[0]*x33*y[3]-
  x[1]*x33*y[3])/d2;

  d1=(x13*x22*x[2]-x12*x23*x[2]-x13*x[1]*x32+x[0]*x23*x32+x12*x[1]*x33-
  x[0]*x22*x33-x13*x22*x[3]+x12*x23*x[3]+x13*x32*x[3]-x23*x32*x[3]-
  x12*x33*x[3]+x22*x33*x[3]+x13*x[1]*x42-x[0]*x23*x42-x13*x[2]*x42+
  x23*x[2]*x42+x[0]*x33*x42-x[1]*x33*x42-x12*x[1]*x43+x[0]*x22*x43+
  x12*x[2]*x43-x22*x[2]*x43-x[0]*x32*x43+x[1]*x32*x43);

  c[1]=(x23*x32*y[0]-x22*x33*y[0]-x23*x42*y[0]+x33*x42*y[0]+x22*x43*y[0]-
  x32*x43*y[0]-x13*x32*y[1]+x12*x33*y[1]+x13*x42*y[1]-x33*x42*y[1]-
  x12*x43*y[1]+x32*x43*y[1]+x13*x22*y[2]-x12*x23*y[2]-x13*x42*y[2]+
  x23*x42*y[2]+x12*x43*y[2]-x22*x43*y[2]-x13*x22*y[3]+x12*x23*y[3]+
  x13*x32*y[3]-x23*x32*y[3]-x12*x33*y[3]+x22*x33*y[3])/d1;

  d0=(x13*x22*x[2]-x12*x23*x[2]-x13*x[1]*x32+x[0]*x23*x32+x12*x[1]*x33-
  x[0]*x22*x33-x13*x22*x[3]+x12*x23*x[3]+x13*x32*x[3]-x23*x32*x[3]-
  x12*x33*x[3]+x22*x33*x[3]+x13*x[1]*x42-x[0]*x23*x42-x13*x[2]*x42+
  x23*x[2]*x42+x[0]*x33*x42-x[1]*x33*x42-x12*x[1]*x43+x[0]*x22*x43+
  x12*x[2]*x43-x22*x[2]*x43-x[0]*x32*x43+x[1]*x32*x43);

  c[0]=(-(x23*x32*x[3]*y[0])+x22*x33*x[3]*y[0]+x23*x[2]*x42*y[0]-x[1]*x33*x42*y[0]-
  x22*x[2]*x43*y[0]+x[1]*x32*x43*y[0]+x13*x32*x[3]*y[1]-x12*x33*x[3]*y[1]-
  x13*x[2]*x42*y[1]+x[0]*x33*x42*y[1]+x12*x[2]*x43*y[1]-x[0]*x32*x43*y[1]-
  x13*x22*x[3]*y[2]+x12*x23*x[3]*y[2]+x13*x[1]*x42*y[2]-x[0]*x23*x42*y[2]-
  x12*x[1]*x43*y[2]+x[0]*x22*x43*y[2]+x13*x22*x[2]*y[3]-x12*x23*x[2]*y[3]-
  x13*x[1]*x32*y[3]+x[0]*x23*x32*y[3]+x12*x[1]*x33*y[3]-x[0]*x22*x33*y[3])/d0;

  if(d0*d1*d2*d3!=0.0) {rp4=0;}
  else {rp4=-1;}              
  
  return rp4;
}
