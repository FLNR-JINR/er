#include "ERParticle.h"

#include "ERSupport.h"

ERParticle::ERParticle(){

}

ERParticle::~ERParticle(){

}

double ERParticle::ReturnMass(char* NON,char* WayMass)
{
  double MassExcess;
  double LifeTime;
  double massa;
  int z;
  int a;
  char Name[5];
  char TimeUnit [6];
  FILE *F1 = fopen(WayMass,"r");
  if(F1==NULL) {printf("ReturnMass: File %s is not found\n",WayMass);}
  else 
  {
  while(!feof(F1))
  {
    int res;
    res = fscanf(F1,"%s %d %d %lf %s %lf\n",Name,&z,&a,&MassExcess,TimeUnit,&LifeTime);
    if (!strcmp(NON,Name))
    {massa=AMU*(double)a+MassExcess;break;}
  }
  fclose(F1);
  }
  return massa;
}
//-----------------------------------------------------------------------------
void ERParticle::ParticleID(char* name, char* path){
  char nucl[5];
  double MassExcess;
  int z;
  int a;
  char TimeUnit[6];
  double LifeTime;

  FILE *F1 = fopen(path,"r");
  if(F1==NULL) {printf("IdParticle: File %s is not found\n",path);}
  else 
  {
    while(!feof(F1))
    {
      int res;
      res = fscanf(F1,"%s %d %d %lf %s %lf\n",nucl,&z,&a,&MassExcess,
        TimeUnit,&LifeTime);
      if (!strcmp(name,nucl))
      {
        Mass=AMU*(double)a+MassExcess;
        NameOfNucleus = new char [strlen(nucl)+1];
        strcpy(NameOfNucleus,nucl);
        AtNumber = z;
        AtMass = a;
        break;
      }
    }
  }
  fclose(F1);
  return;
}
