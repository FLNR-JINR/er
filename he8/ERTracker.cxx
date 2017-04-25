#include "ERTracker.h"

#include<iostream>
using namespace std;

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoVolume.h"


#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERTracker::ERTracker()
  : FairTask("ERTracker"),
  fEvent(0),
  Ntelescopes(0),
  isi(0),
  jsi(0),
  icsi(0),
  jcsi(0),
  si(false),
  csi(false),
  NlayMax(0),
  NdivMax(0),
  NtelMax(5)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTracker::ERTracker(Int_t verbose)
  : FairTask("ERTracker", verbose),
  fEvent(0),
  Ntelescopes(0),
  isi(0),
  jsi(0),
  icsi(0),
  jcsi(0),
  si(false),
  csi(false),
  NlayMax(0),
  NdivMax(0),
  NtelMax(5)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTracker::~ERTracker()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTracker::SetParContainers()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERTracker::Init()
{
  ReadTelescopeParameters();
  //CreateTelescopeGeometry();
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERTracker::Exec(Option_t* opt)
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERTracker::Reset()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTracker::Finish()
{   
}

void ERTracker::ReadTelescopeParameters(){
  layer = new int[NtelMax];
  /********************* Readout Telescopes parameters: ******************/
  FILE* F1 = fopen("/home/vitaliy.schetinin/er/input/detsys.prm","r");
  if(F1==NULL) printf("Main: File detsys.prm was not found\n");
  else
  {
    int it=0;
    int il=0;
    for(int i=0;i<NtelMax;i++) layer[i]=0;

    fgets(DummyC,256,F1);
    while (fgets(DummyC,256,F1)) 
    {
      il++;
      if(strstr(DummyC,"Telescope")) {it++;layer[it-1]=il-1;il=0;}
      if(strstr(DummyC,"Resolution")) {it++;layer[it-1]=il-1;break;}
    }
    Ntelescopes = it;
    for(it=0;it<Ntelescopes;it++)
    {
      if(layer[it]>=NlayMax)
      {NlayMax = layer[it];}
    }
    printf("Main: In file detsys.prm the telscopes have been found:\n");
    for(int it=1;it<=Ntelescopes;it++)
    {printf("Telscope%i Nlayer=%i\n",it,layer[it-1]);}
  }
  //Telescope Det[Ntelescopes][NlayMax];
  cout << "Ntelescopes" << Ntelescopes << endl;
  cout << "NlayMax" << NlayMax << endl;
  cout << "NtelMax" << NtelMax << endl;
  Det = new Telescope*[Ntelescopes];
  for (int i = 0 ;i<Ntelescopes;i++){
    Det[i] = new Telescope[NlayMax];
  }
  rewind(F1);
  for(int i=0;i<Ntelescopes;i++)
  {
    cerr << i << endl;
    fscanf(F1,"%s %lf %lf\n",Zeros,&pdx1,&pdx2);
    for(int j=0;j<layer[i];j++)
    {
      fscanf(F1,"%lf %s %lf %lf %lf %lf %lf %lf %lf %lf %i %i %s %s\n",
      &pd1,pd2,&pd3,&pd31,&pd4,&pd5,&pd6,&pd7,&pd8,&pd9,&pd10,&pd11,pd12,pd13);
      Det[i][j].Angle = pdx1*rad;
      cerr << "pdx1" <<  pdx1 << endl;
      cerr << "pdx2" <<  pdx2 << endl;
      cerr << "pd3" <<  pd3 << endl;
      //strcpy(Det[i][j].StripFB,pd2);
      pdx2 += pd1*fabs(pdx2)/pdx2;
      Det[i][j].Dist = pdx2;
      pdx2 += pd4*fabs(pdx2)/pdx2;
      Det[i][j].DeadZoneF = pd3;
      Det[i][j].DeadZoneB = pd31;
      Det[i][j].Thick = pd4;
      Det[i][j].OffsetX = pd7;
      Det[i][j].OffsetY = pd8;
      Det[i][j].DeltaPhi = pd9;
      //strcpy(Det[i][j].Shape,pd13);
      //strcpy(Det[i][j].Matt,pd12);
      //if(!strcmp(Det[i][j].Matt,"si")) {si=true;isi=i;jsi=j;}
      //if(!strcmp(Det[i][j].Matt,"csi")) {csi=true;icsi=i;jcsi=j;}
      Det[i][j].NstripX = pd10;
      Det[i][j].NstripY = pd11;
      Det[i][j].SizeX = pd5;
      Det[i][j].SizeY = pd6;
      Det[i][j].Nring = pd10;
      Det[i][j].Nsector = pd11;
      Det[i][j].Rin = pd5;
      Det[i][j].Rout = pd6;
    }
  }
  /*
  fscanf(F1,"%s %lf %lf\n",Zeros,&ResolutionSi,&ResolutionCsI);
  fclose(F1);
  for(int it=0;it<Ntelescopes;it++)
  {
    for(int il=0;il<layer[it];il++)
    {
      if(abs(Det[it][il].NstripX)>=NdivMax)
      {NdivMax = abs(Det[it][il].NstripX);}
      if(abs(Det[it][il].NstripY)>=NdivMax)
      {NdivMax = abs(Det[it][il].NstripY);}
      if(abs(Det[it][il].Nring)>=NdivMax)
      {NdivMax = abs(Det[it][il].Nring);}
      if(abs(Det[it][il].Nsector)>=NdivMax)
      {NdivMax = abs(Det[it][il].Nsector);}
    }
  }
  int NofDSD[Ntelescopes];
  for(int it=0;it<Ntelescopes;it++)
  {
    NofDSD[it] = 0;
    for(int il=0;il<layer[it];il++)
    {
      if(!strcmp(Det[it][il].StripFB,"stripXY")||!strcmp(Det[it][il].StripFB,"stripYX")||
      !strcmp(Det[it][il].StripFB,"stripRS")||!strcmp(Det[it][il].StripFB,"stripSR")) 
      {NofDSD[it]++; NdivMax = abs(Det[it][il].Nsector)+abs(Det[it][il].Nring);}
    }
  }
  int NofDSDMax = 0;
  for(int it=0;it<Ntelescopes;it++)
  {
    if(NofDSD[it]>NofDSDMax) NofDSDMax = NofDSD[it];
  }
  printf("******************* Detector setup *************************\n");
  for(int i=0;i<Ntelescopes;i++)
  {
    printf("Telescope%i: Angle=%lf\n",i+1,Det[i][0].Angle/rad);
    for(int j=0;j<layer[i];j++)
    {
      printf("%i%i %lf %lf %lf %lf %lf %lf %lf %lf %lf %i %i %i %i %s %s\n",i,j,Det[i][j].Dist,
      Det[i][j].DeadZoneF,Det[i][j].DeadZoneB,Det[i][j].Thick,Det[i][j].SizeX,Det[i][j].SizeY,
      Det[i][j].OffsetX,Det[i][j].OffsetY,Det[i][j].DeltaPhi,Det[i][j].NstripX,Det[i][j].NstripY,
      Det[i][j].Nring,Det[i][j].Nsector,Det[i][j].Shape,Det[i][j].Matt);
    }
  }
  printf("Resolution: Si=%lf MeV, CsI=%lf percent\n",ResolutionSi,ResolutionCsI);
  printf("************************************************************\n");

/***********************************************************************/

}

// ----------------------------------------------------------------------------
void ERTracker::CreateTelescopeGeometry(){
  /********************* ROOT Telescope Geometry: ************************/
  double ThickStr=0.0001;
  TGeoManager *geom=new TGeoManager("Reaction Chamber","geom"); 
  TGeoMaterial *matVacuum=new TGeoMaterial("Vacuum",0,0,0);
  TGeoMaterial *matSi=new TGeoMaterial("Si",24.,12,2.4);
  TGeoMedium *Vacuum=new TGeoMedium("Vacuum",1,matVacuum);
  TGeoMedium *Si=new TGeoMedium("Si",2,matSi);
  TGeoVolume *top=geom->MakeSphere("Top",Vacuum,0.,100.);
  geom->SetTopVolume(top);

  TGeoRotation *rot = new TGeoRotation();

  TGeoVolume *pDet[Ntelescopes][NlayMax][NdivMax+1];
  
  TGeoNode *trajectory;

  double strw,strXw,strYw,strSw,strRw,fbsh,fbshX,fbshY,fbshR,fbshS;
  double Xob,Yob,Zob,shftX,shftY,RinRing,RoutRing,PhiIn,PhiOut;
  int checkX,checkY,checkR,checkS;
      
  for(int it=0;it<Ntelescopes;it++)
  {
    for(int il=0;il<layer[it];il++)
    {
      if(!strcmp(Det[it][il].StripFB,"stripf")) fbsh=-ThickStr*fabs(Det[it][il].Dist)/2./Det[it][il].Dist;
      else if(!strcmp(Det[it][il].StripFB,"stripb")) fbsh=(ThickStr/2.+Det[it][il].Thick)*fabs(Det[it][il].Dist)/
        Det[it][il].Dist;
      else if(!strcmp(Det[it][il].StripFB,"nostrip")) 
        {fbsh=Det[it][il].Thick*fabs(Det[it][il].Dist)/2./Det[it][il].Dist;checkX=1;checkY=1,checkR=1,checkS=1;}
      else if(!strcmp(Det[it][il].StripFB,"stripYX")||!strcmp(Det[it][il].StripFB,"stripRS"))
      {
        fbshY=-ThickStr*fabs(Det[it][il].Dist)/2./Det[it][il].Dist;
        fbshR=-ThickStr*fabs(Det[it][il].Dist)/2./Det[it][il].Dist;
        fbshX=(ThickStr/2.+Det[it][il].Thick)*fabs(Det[it][il].Dist)/Det[it][il].Dist;
        fbshS=(ThickStr/2.+Det[it][il].Thick)*fabs(Det[it][il].Dist)/Det[it][il].Dist;
        checkY=1;checkX=0;
        checkR=1;checkS=0;
      }
      else if(!strcmp(Det[it][il].StripFB,"stripXY")||!strcmp(Det[it][il].StripFB,"stripSR"))
      {
        fbshX=-ThickStr*fabs(Det[it][il].Dist)/2./Det[it][il].Dist;
        fbshS=-ThickStr*fabs(Det[it][il].Dist)/2./Det[it][il].Dist;
        fbshY=(ThickStr/2.+Det[it][il].Thick)*fabs(Det[it][il].Dist)/Det[it][il].Dist;
        fbshR=(ThickStr/2.+Det[it][il].Thick)*fabs(Det[it][il].Dist)/Det[it][il].Dist;
        checkY=0;checkX=1;
        checkR=0;checkS=1;
      }
      rot->SetAngles(90.,Det[it][il].Angle/rad,0.);

      char N1[3];
      char N0[]="pDet";
      sprintf(N1,"%d",it);strcat(N0,N1);
      sprintf(N1,"%d",il);strcat(N0,N1);
      strcat(N0,"0");
      if(!strcmp(Det[it][il].Shape,"square"))
      {
        if(strcmp(Det[it][il].StripFB,"nostrip")) pDet[it][il][0]=geom->MakeBox(N0,Si,Det[it][il].SizeY/2.,Det[it][il].SizeX/2.,
          Det[it][il].Thick/2.);
        if(Det[it][il].NstripX!=1) {Ndiv=abs(Det[it][il].NstripX);strw=Det[it][il].SizeX/Det[it][il].NstripX;}
        if(Det[it][il].NstripY!=1) {Ndiv=abs(Det[it][il].NstripY);strw=Det[it][il].SizeY/Det[it][il].NstripY;}
        strXw=Det[it][il].SizeX/Det[it][il].NstripX;
        strYw=Det[it][il].SizeY/Det[it][il].NstripY;
        if(!strcmp(Det[it][il].StripFB,"nostrip"))
          Ndiv=fabs(Det[it][il].NstripX*Det[it][il].NstripY);
        if(!strcmp(Det[it][il].StripFB,"stripYX")||!strcmp(Det[it][il].StripFB,"stripXY"))
          Ndiv=fabs(Det[it][il].NstripX)+fabs(Det[it][il].NstripY);
      }
      else if(!strcmp(Det[it][il].Shape,"annular"))
      {
        if(strcmp(Det[it][il].StripFB,"nostrip")) pDet[it][il][0]=geom->MakeTube(N0,Si,Det[it][il].Rin,Det[it][il].Rout,
          Det[it][il].Thick/2.);
        if(Det[it][il].Nsector!=1) {Ndiv=abs(Det[it][il].Nsector);strw=360./Det[it][il].Nsector;}
        if(Det[it][il].Nring!=1) {Ndiv=abs(Det[it][il].Nring);strw=(Det[it][il].Rout-Det[it][il].Rin)/Det[it][il].Nring;}
        strSw=360./Det[it][il].Nsector;
        strRw=(Det[it][il].Rout-Det[it][il].Rin)/Det[it][il].Nring;
        if(!strcmp(Det[it][il].StripFB,"nostrip"))
          Ndiv=fabs(Det[it][il].Nring*Det[it][il].Nsector);
        if(!strcmp(Det[it][il].StripFB,"stripRS")||!strcmp(Det[it][il].StripFB,"stripSR"))
          Ndiv=fabs(Det[it][il].Nring)+fabs(Det[it][il].Nsector);
      }
      TGeoTranslation *trans = new TGeoTranslation
        (Det[it][il].OffsetY,-Det[it][il].OffsetX,Det[it][il].Dist+Det[it][il].Thick*fabs(Det[it][il].Dist)/2./
        Det[it][il].Dist);
      TGeoCombiTrans *pos1=new TGeoCombiTrans();
      for(int is=1;is<=Ndiv;is++)
      {
        char N2[]="pDet";
        TGeoCombiTrans *pos=new TGeoCombiTrans();
        sprintf(N1,"%d",it);strcat(N2,N1);
        sprintf(N1,"%d",il);strcat(N2,N1);
        sprintf(N1,"%d",is);strcat(N2,N1);
        double shft=strw*(double(is)-0.5-Ndiv/2.);
        TGeoTranslation *trans1=new TGeoTranslation();
        if(!strcmp(Det[it][il].Shape,"square"))
        {
          if(Det[it][il].NstripX!=1&&Det[it][il].NstripY==1&&
            (!strcmp(Det[it][il].StripFB,"stripf")||!strcmp(Det[it][il].StripFB,"stripb")))
          {
            trans1->SetTranslation(Det[it][il].OffsetY,-shft-Det[it][il].OffsetX,
              fbsh+Det[it][il].Dist);
            Xob=Det[it][il].SizeY/2.;
            Yob=fabs(strw)/2.;
            Zob=ThickStr/2.;
          }
          else if(Det[it][il].NstripY!=1&&Det[it][il].NstripX==1&&
            (!strcmp(Det[it][il].StripFB,"stripf")||!strcmp(Det[it][il].StripFB,"stripb")))
          {
            trans1->SetTranslation(shft+Det[it][il].OffsetY,-Det[it][il].OffsetX,
              fbsh+Det[it][il].Dist);
            Xob=fabs(strw)/2;
            Yob=Det[it][il].SizeX/2.;
            Zob=ThickStr/2.;
          }
          else if(!strcmp(Det[it][il].StripFB,"nostrip"))
          {
            if(checkX>abs(Det[it][il].NstripX)) {checkX=1;checkY++;}
            shftX=strXw*(double(checkX)-0.5-abs(Det[it][il].NstripX)/2.);
            shftY=strYw*(double(checkY)-0.5-abs(Det[it][il].NstripY)/2.);
            trans1->SetTranslation(shftY+Det[it][il].OffsetY,-shftX-Det[it][il].OffsetX,
              fbsh+Det[it][il].Dist);
            Xob=fabs(strYw)/2.;
            Yob=fabs(strXw)/2.;
            Zob=Det[it][il].Thick/2.;
            checkX++;
          }
          if(!strcmp(Det[it][il].StripFB,"stripYX"))
          {
            if(checkY>abs(Det[it][il].NstripY)) {checkX++;}
            shftX=strXw*(double(checkX)-0.5-abs(Det[it][il].NstripX)/2.);
            shftY=strYw*(double(checkY)-0.5-abs(Det[it][il].NstripY)/2.);
            if(checkY<=abs(Det[it][il].NstripY))
            {
              trans1->SetTranslation(shftY+Det[it][il].OffsetY,-Det[it][il].OffsetX,
                fbshY+Det[it][il].Dist);
              Xob=fabs(strYw)/2;
              Yob=Det[it][il].SizeX/2.;
              Zob=ThickStr/2.;
            }
            else
            {
              trans1->SetTranslation(Det[it][il].OffsetY,-shftX-Det[it][il].OffsetX,
                fbshX+Det[it][il].Dist);
              Xob=Det[it][il].SizeY/2.;
              Yob=fabs(strXw)/2.;
              Zob=ThickStr/2.;
            }
            checkY++;
          }
          else if(!strcmp(Det[it][il].StripFB,"stripXY"))
          {
            if(checkX>abs(Det[it][il].NstripX)) {checkY++;}
            shftX=strXw*(double(checkX)-0.5-abs(Det[it][il].NstripX)/2.);
            shftY=strYw*(double(checkY)-0.5-abs(Det[it][il].NstripY)/2.);
            if(checkX<=abs(Det[it][il].NstripX))
            {
              trans1->SetTranslation(Det[it][il].OffsetY,-shftX-Det[it][il].OffsetX,
                fbshX+Det[it][il].Dist);
              Xob=Det[it][il].SizeY/2.;
              Yob=fabs(strXw)/2.;
              Zob=ThickStr/2.;
            }
            else
            {
              trans1->SetTranslation(shftY+Det[it][il].OffsetY,-Det[it][il].OffsetX,
                fbshY+Det[it][il].Dist);
              Xob=fabs(strYw)/2;
              Yob=Det[it][il].SizeX/2.;
              Zob=ThickStr/2.;
            }
            checkX++;
          }
          *pos = *rot*(*trans1);
          pDet[it][il][is]=geom->MakeBox(N2,Si,Xob,Yob,Zob);
          top->AddNode(pDet[it][il][is],1,pos);
        } /* if(!strcmp(Det[it][il].Shape,"square")) */
        else if(!strcmp(Det[it][il].Shape,"annular"))
        {
          if(Det[it][il].Nsector!=1&&Det[it][il].Nring==1&&
            (!strcmp(Det[it][il].StripFB,"stripf")||!strcmp(Det[it][il].StripFB,"stripb")))
          {
            trans1->SetTranslation(Det[it][il].OffsetY,-Det[it][il].OffsetX,
              fbsh+Det[it][il].Dist);
            RinRing=Det[it][il].Rin;
            RoutRing=Det[it][il].Rout;
            Zob=ThickStr/2.;
            if(strw>0.)
            {
              PhiIn=Det[it][il].DeltaPhi+strw*(double(is)-1.)-90.;
              PhiOut=PhiIn+strw;
            }
            else
            {
              PhiOut=Det[it][il].DeltaPhi+360.+strw*(double(is)-1.)-90.;
              PhiIn=PhiOut+strw;
            }
          }
          else if(Det[it][il].Nring!=1&&Det[it][il].Nsector==1&&
            (!strcmp(Det[it][il].StripFB,"stripf")||!strcmp(Det[it][il].StripFB,"stripb")))
          {
            trans1->SetTranslation(Det[it][il].OffsetY,-Det[it][il].OffsetX,
              fbsh+Det[it][il].Dist);
            PhiIn=0.;
            PhiOut=360.;
            Zob=ThickStr/2.;
            if(strw>0.)
            {
              RinRing=Det[it][il].Rin+strw*(double(is)-1.);
              RoutRing=RinRing+strw;
            }
            else
            {
              RoutRing=Det[it][il].Rout+strw*(double(is)-1.);
              RinRing=RoutRing+strw;
            }
          }
          else if(!strcmp(Det[it][il].StripFB,"nostrip"))
          {
            trans1->SetTranslation(Det[it][il].OffsetY,-Det[it][il].OffsetX,
              fbsh+Det[it][il].Dist);
            if(checkS>abs(Det[it][il].Nsector)) {checkS=1;checkR++;}
            RinRing=Det[it][il].Rin+strRw*(double(checkR)-1.);
            RoutRing=RinRing+strRw;
            PhiIn=Det[it][il].DeltaPhi+strSw*(double(checkS)-1.)-90.;
            PhiOut=PhiIn+strSw;
            Zob=Det[it][il].Thick/2.;
            checkS++;
          }
          if(!strcmp(Det[it][il].StripFB,"stripRS"))
          {
            if(checkR>abs(Det[it][il].Nring)) {checkS++;}
            if(checkR<=abs(Det[it][il].Nring))
            {
              trans1->SetTranslation(Det[it][il].OffsetY,-Det[it][il].OffsetX,
                fbshR+Det[it][il].Dist);
              if(strRw>0.)
              {
                RinRing=Det[it][il].Rin+strRw*(double(checkR)-1.);
                RoutRing=RinRing+strRw;
              }
              else
              {
                RoutRing=Det[it][il].Rout+strRw*(double(checkR)-1.);
                RinRing=RoutRing+strRw;
              }
              PhiIn = 0.;
              PhiOut = 360.;
            }
            else
            {
              trans1->SetTranslation(Det[it][il].OffsetY,-Det[it][il].OffsetX,
                fbshS+Det[it][il].Dist);
              if(strSw>0.)
              {
                PhiIn=Det[it][il].DeltaPhi+strSw*(double(checkS)-1.)-90.;
                PhiOut=PhiIn+strSw;
              }
              else
              {
                PhiOut=Det[it][il].DeltaPhi+360.+strSw*(double(checkS)-1.)-90.;
                PhiIn=PhiOut+strSw;
              }
              RinRing = Det[it][il].Rin;
              RoutRing = Det[it][il].Rout;
            }
            Zob=ThickStr/2.;
            checkR++;
          }
          else if(!strcmp(Det[it][il].StripFB,"stripSR"))
          {
            if(checkS>abs(Det[it][il].Nsector)) {checkR++;}
            if(checkS<=abs(Det[it][il].Nsector))
            {
              trans1->SetTranslation(Det[it][il].OffsetY,-Det[it][il].OffsetX,
                fbshS+Det[it][il].Dist);
              if(strSw>0.)
              {
                PhiIn=Det[it][il].DeltaPhi+strSw*(double(checkS)-1.)-90.;
                PhiOut=PhiIn+strSw;
              }
              else
              {
                PhiOut=Det[it][il].DeltaPhi+360.+strSw*(double(checkS)-1.)-90.;
                PhiIn=PhiOut+strSw;
              }
              RinRing = Det[it][il].Rin;
              RoutRing = Det[it][il].Rout;
            }
            else
            {
              trans1->SetTranslation(Det[it][il].OffsetY,-Det[it][il].OffsetX,
                fbshR+Det[it][il].Dist);
              if(strRw>0.)
              {
                RinRing=Det[it][il].Rin+strRw*(double(checkR)-1.);
                RoutRing=RinRing+strRw;
              }
              else
              {
                RoutRing=Det[it][il].Rout+strRw*(double(checkR)-1.);
                RinRing=RoutRing+strRw;
              }
              PhiIn = 0.;
              PhiOut = 360.;
            }
            Zob=ThickStr/2.;
            checkS++;
          }
          *pos = *rot*(*trans1);
          pDet[it][il][is]=geom->MakeTubs(N2,Si,RinRing,RoutRing,Zob,PhiIn,PhiOut);
          top->AddNode(pDet[it][il][is],1,pos);
        } /* if(!strcmp(Det[it][il].Shape,"annular")) */
      } /* for(nt is=1;is<=Ndiv;is++) */
      *pos1 = *rot*(*trans);
      if(strcmp(Det[it][il].StripFB,"nostrip")) top->AddNode(pDet[it][il][0],1,pos1);
    } /* for(int il=0;il<layer[it];il++) */
  } /* (int it=0;it<Ntelescopes;it++) */
  geom->CloseGeometry();
/***********************************************************************/
}
//-----------------------------------------------------------------------------
ClassImp(ERTracker)
