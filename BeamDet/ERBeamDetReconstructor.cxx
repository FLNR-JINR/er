#include "ERBeamDetReconstructor.h"

#include<iostream>
using namespace std;

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairRun.h"

#include "ERSupport.h"

// ----------------------------------------------------------------------------
ERBeamDetReconstructor::ERBeamDetReconstructor()
  : FairTask("Convert cal event to reco event"),
  fInEvent(NULL),
  fInCalEvent(NULL),
  fOutEvent(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------
ERBeamDetReconstructor::ERBeamDetReconstructor(Int_t verbose)
  : FairTask("Convert cal event to reco event", verbose),
  fInEvent(NULL),
  fInCalEvent(NULL),
  fOutEvent(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------
ERBeamDetReconstructor::~ERBeamDetReconstructor()
{
}
// ----------------------------------------------------------------------------
void ERBeamDetReconstructor::SetParContainers()
{
}
// ----------------------------------------------------------------------------
InitStatus ERBeamDetReconstructor::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  fInEvent = (ERBeamDetEvent*)ioman->GetObject("BeamDetEvent.");
  if (!fInEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fInCalEvent = (ERBeamDetCalEvent*)ioman->GetObject("BeamDetCalEvent.");
  if (!fInEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fOutEvent = new ERBeamDetRecoEvent();
  ioman->Register("BeamDetRecoEvent.","Reco",fOutEvent, kTRUE);

  FairRun* run = FairRun::Instance();
  header = (ERHe8EventHeader*)run->GetEventHeader();

  TString filePath  = gSystem->Getenv("VMCWORKDIR") + TString("/input/StableNuclei.dat");
  char* WayMass= const_cast<char*>(filePath.Data());
  fOutEvent->projectile.ParticleID(header->projname,WayMass);
  fOutEvent->target.ParticleID(header->tarname,WayMass);
  fOutEvent->target.Part.SetPxPyPzE(0.,0.,0.,fOutEvent->target.Mass);
  ReadDeDx();
  DefineBeamEnergy();
  ElossTOFaMWPCaTarget();

  return kSUCCESS;
}
// -----   Public method Exec   --------------------------------------------
void ERBeamDetReconstructor::Exec(Option_t* opt)
{
  std::cout << "####### ERBeamDetReconstructor EVENT " << fEvent++ << " #####" << std::endl;
  Reset();
  MWPC();
  Tof();
}
//----------------------------------------------------------------------------
void ERBeamDetReconstructor::Tof(){
  TRandom Rnd;
  char ShowTrack[10];
  double tof_offset = 87.98;
  //      double tof_offset = 84.;
  double dt_F3,dt_F4,t_F3,t_F4;
  header->mtrack = i_flag_MW;
  //RawD.mtrack = 1;i_flag_MW = 1;

    header->mbeam = 0;
    // ****************** measurement of TOF spread around tof_0, calculated from the magnetic field in the 2nd dipole ************************
    //        fInCalEvent->tF3l = fInCalEvent->tF3l - fInCalEvent->tF4r+ParD.CLB[3][0][0][1];
    //        fInCalEvent->tF3r = fInCalEvent->tF3r - fInCalEvent->tF4r+ParD.CLB[3][1][0][1];
    //        fInCalEvent->tF4l = fInCalEvent->tF4l - fInCalEvent->tF4r+ParD.CLB[4][1][0][1];
    //        fInCalEvent->tF4r = fInCalEvent->tF4r - fInCalEvent->tF4r+ParD.CLB[4][0][0][1];
    // ****************************************************************************************************************************************
          
            dt_F3 = (fInCalEvent->tF3l-fInCalEvent->tF3r);
            dt_F4 = (fInCalEvent->tF4l-fInCalEvent->tF4r);

    //        if(fabs(dt_F4)<=header->UpMat.tF4_dlt&&fabs(dt_F3)<=header->UpMat.tF3_dlt&&
    //        fabs(fInCalEvent->tF3l)<=header->UpMat.tF3l_rng&&fabs(fInCalEvent->tF3r)<=header->UpMat.tF3r_rng&&
    //        fabs(fInCalEvent->tF4l)<=header->UpMat.tF4l_rng&&fabs(fInCalEvent->tF4r)<=header->UpMat.tF4r_rng)
    //        if(fabs(dt_F4)<=header->UpMat.tF4_dlt&&fabs(dt_F3)<=header->UpMat.tF3_dlt)
    //        {
    //printf("*********************************\n");
    //          t_F3 = (Rnd.Gaus(fInCalEvent->tF3l,header->UpMat.TofRes/2.35)+Rnd.Gaus(fInCalEvent->tF3r,header->UpMat.TofRes/2.35))/2;
    //          t_F4 = (Rnd.Gaus(fInCalEvent->tF4l,header->UpMat.TofRes/2.35)+Rnd.Gaus(fInCalEvent->tF4r,header->UpMat.TofRes/2.35))/2.;
          t_F4 = Rnd.Gaus(fInCalEvent->tF4l,header->UpMat.TofRes/2.35);
          t_F3 = Rnd.Gaus(fInCalEvent->tF3r,header->UpMat.TofRes/2.35);
    //          t_F4 = Rnd.Gaus((fInCalEvent->tF4l+fInCalEvent->tF4r)/2,header->UpMat.TofRes/2.35);
    // ****************** measurement of TOF spread around tof_0, calculated from the magnetic field in the 2nd dipole ************************
    //          fInCalEvent->tofb = t_F4 - t_F3 + tof_0;
    // ********************************* measurement of absolute TOF value tof_offset = dT1-L0*(dT1-dT0)/(L1-L0)*******************************
          fOutEvent->tofb = t_F4 - t_F3 + tof_offset;
    //if(fInCalEvent->tofb<130.) 
    //{printf("ntF3l=%i,ntF3r=%i,ntF4l=%i,ntF4r=%i\n",RawD.ntF3l,RawD.ntF3r,RawD.ntF4l,RawD.ntF4r);
    //printf("tF3l=%lf,tF3r=%lf,tF4l=%lf,tF4r=%lf,  TOF=%lf\n",fInCalEvent->tF3l,fInCalEvent->tF3r,fInCalEvent->tF4l,fInCalEvent->tF4r,fInCalEvent->tofb);
    //printf("t_F3=%lf,t_F4=%lf,(fInCalEvent->tF3l+fInCalEvent->tF3r)/2=%lf,(fInCalEvent->tF4l+fInCalEvent->tF4r)/2=%lf,  TOF=%lf\n",t_F3,t_F4,(fInCalEvent->tF3l+fInCalEvent->tF3r)/2,
    //(fInCalEvent->tF4l+fInCalEvent->tF4r)/2,(fInCalEvent->tF4l+fInCalEvent->tF4r-fInCalEvent->tF3l-fInCalEvent->tF3r)/2+ tof_offset);}
    // ****************************************************************************************************************************************
          if(fInCalEvent->aF4r+fInCalEvent->aF4l>500.&&fOutEvent->tofb<150.&&fOutEvent->tofb>60.)
          {

            beta_b = header->UpMat.PlasticDist/fOutEvent->tofb/slight;

            if(beta_b>0.&&beta_b<=1.)
            {
              header->mbeam = 1;
              strcpy(ShowTrack,"invisible");
              gamma_b = 1./sqrt(1.-beta_b*beta_b);
              p_beam  = beta_b*fOutEvent->projectile.Mass*gamma_b;
              fOutEvent->projectile.Part.SetPxPyPzE(p_beam*sin(Vbeam.Theta())*cos(Vbeam.Phi()),
                p_beam*sin(Vbeam.Theta())*sin(Vbeam.Phi()),p_beam*cos(Vbeam.Theta()),
                p_beam/beta_b);

              Tb = UpstreamEnergyLoss(&(header->UpMat),&(fOutEvent->projectile),header->ReIN.TOFis,header->ReIN.TRACKINGis,ShowTrack);
              if(fOutEvent->Tb>0.1&&!strcmp(header->UpMat.HeatScreenAns,"yes")) 
                Tb = EiEo(header->UpMat.beam_TARwin,Tb,header->UpMat.HeatScreenThick/cos(Vbeam.Theta()));
              if(Tb>0.1)
                Tb = EiEo(header->UpMat.beam_TARwin,Tb,header->UpMat.FoilThick/cos(Vbeam.Theta()));
              else 
                Tb = 0.;
              float range = header->UpMat.TarThick*header->UpMat.TarPress*TempNorm/header->UpMat.TarTemp/cos(Vbeam.Theta())/2.;
              if(Tb>0.1) Tb = EiEo(header->UpMat.beam_target,Tb,range);
              else Tb = 0.;
              p_beam = sqrt(pow(Tb+fOutEvent->projectile.Mass,2)-pow(fOutEvent->projectile.Mass,2));
              fOutEvent->projectile.Part.SetPxPyPzE(p_beam*sin(Vbeam.Theta())*
                cos(Vbeam.Phi()),p_beam*sin(Vbeam.Theta())*sin(Vbeam.Phi()),
                p_beam*cos(Vbeam.Theta()),Tb+fOutEvent->projectile.Mass);
              fOutEvent->CM0.Part = fOutEvent->projectile.Part + fOutEvent->target.Part;
              fOutEvent->projectile.Part.Boost(-fOutEvent->CM0.Part.BoostVector());
              fOutEvent->target.Part.Boost(-fOutEvent->CM0.Part.BoostVector());
              t_cm0 = fOutEvent->projectile.Part.E()+fOutEvent->target.Part.E()-fOutEvent->projectile.Mass-fOutEvent->target.Mass;
              t_cm = t_cm0 + Qreaction;
              fOutEvent->projectile.Part.Boost(fOutEvent->CM0.Part.BoostVector());
              fOutEvent->target.Part.Boost(fOutEvent->CM0.Part.BoostVector());
              if(t_cm>0.) good_mbeam++;
              else header->mbeam = 0;
              fOutEvent->Tb = fOutEvent->projectile.Part.E()-fOutEvent->projectile.Mass;
              fOutEvent->tcm = t_cm;              
            } /* if(beta_b>0.&&beta_b<=1.) */  
          } /* if(fInCalEvent->aF4r+fInCalEvent->aF4l>500.) */
          //        } /* if(fabs(dt_F4)<=header->UpMat.tF4_dlt&&fabs(dt_F3)<=header->UpMat.tF3_dlt&& */
      if(i_flag_MW==1&&sqrt(pow(fOutEvent->xbt,2)+pow(fOutEvent->ybt,2))<header->UpMat.TarEntrHoleRad)
        header->mtrack = 1;
      
      fOutEvent->Tb = Tb;
}
//----------------------------------------------------------------------------
void ERBeamDetReconstructor::MWPC(){
  i_flag_MW = 0;
  double tarcoord[3];
  if(header->ReIN.TRACKINGis)
      {
        for(int iMW=0;iMW<header->UpMat.MWNwires;iMW++)
        {
          fOutEvent->nMW11[iMW] = 0;fOutEvent->nMW12[iMW] = 0;
          fOutEvent->nMW21[iMW] = 0;fOutEvent->nMW22[iMW] = 0;
        }
        fOutEvent->mMW12 = fInEvent->nx1;
        fOutEvent->mMW11 = fInEvent->ny1;
        fOutEvent->mMW22 = fInEvent->nx2;
        fOutEvent->mMW21 = fInEvent->ny2;
        
        if(fOutEvent->mMW11>=1&&fOutEvent->mMW12>=1) good_mw1++;
        if(fOutEvent->mMW21>=1&&fOutEvent->mMW22>=1) good_mw2++;
        if(fOutEvent->mMW11>=1&&fOutEvent->mMW12>=1&&fOutEvent->mMW21>=1&&fOutEvent->mMW22>=1) good_mw++;

        for(int iMW=1;iMW<=fOutEvent->mMW11;iMW++) fOutEvent->nMW11[iMW-1] = fInEvent->y1[iMW-1];
        for(int iMW=1;iMW<=fOutEvent->mMW12;iMW++) fOutEvent->nMW12[iMW-1] = fInEvent->x1[iMW-1];
        for(int iMW=1;iMW<=fOutEvent->mMW21;iMW++) fOutEvent->nMW21[iMW-1] = fInEvent->y2[iMW-1];
        for(int iMW=1;iMW<=fOutEvent->mMW22;iMW++) fOutEvent->nMW22[iMW-1] = fInEvent->x2[iMW-1];

        fOutEvent->mcMW11 = mcluMW(fOutEvent->mMW11,fOutEvent->nMW11);
        fOutEvent->mcMW12 = mcluMW(fOutEvent->mMW12,fOutEvent->nMW12);
        fOutEvent->mcMW21 = mcluMW(fOutEvent->mMW21,fOutEvent->nMW21);
        fOutEvent->mcMW22 = mcluMW(fOutEvent->mMW22,fOutEvent->nMW22);
        
        int i_flag_MW1 = 0;
        int i_flag_MW2 = 0;

        if(fOutEvent->mcMW11==1&&fOutEvent->mcMW12==1) {i_flag_MW1 = 1;goodclu_mw1++;}
        else if(fOutEvent->mcMW11>1||fOutEvent->mcMW12>1) badclu_mw1++;
        if(fOutEvent->mcMW21==1&&fOutEvent->mcMW22==1) {i_flag_MW2 = 1;goodclu_mw2++;}
        else if(fOutEvent->mcMW21>1||fOutEvent->mcMW22>1) badclu_mw2++;
        if(i_flag_MW1&&i_flag_MW2) {i_flag_MW = 1;goodclu_mw++;}
        else badclu_mw++;

        if(i_flag_MW)
        {
          
          char MWid[]="MWfar";
          char XY[]="X";
          fOutEvent->xmw1=coordMW(&(header->UpMat),fOutEvent,MWid,XY);
          strcpy(XY,"Y");
          fOutEvent->ymw1=coordMW(&(header->UpMat),fOutEvent,MWid,XY);
          strcpy(MWid,"MWclo");
          strcpy(XY,"X");
          fOutEvent->xmw2=coordMW(&(header->UpMat),fOutEvent,MWid,XY);
          strcpy(XY,"Y");
          fOutEvent->ymw2=coordMW(&(header->UpMat),fOutEvent,MWid,XY);
          /*
          fOutEvent->ymw1 += (fOutEvent->ymw2-fOutEvent->ymw1)*(header->UpMat.MWXYdist)/(header->UpMat.MWclosDist-header->UpMat.MWfarDist);
          fOutEvent->ymw2 = fOutEvent->ymw1 + (fOutEvent->ymw2-fOutEvent->ymw1)*
            (header->UpMat.MWclosDist-header->UpMat.MWfarDist+header->UpMat.MWXYdist/2+header->UpMat.MWgasThick/2)/(header->UpMat.MWclosDist-header->UpMat.MWfarDist);
          fOutEvent->xmw2 = fOutEvent->xmw1 + (fOutEvent->xmw2-fOutEvent->xmw1)*
            (header->UpMat.MWclosDist-header->UpMat.MWfarDist-header->UpMat.MWXYdist/2+header->UpMat.MWgasThick/2)/(header->UpMat.MWclosDist-header->UpMat.MWfarDist);
          VmwCl.SetXYZ(fOutEvent->xmw2,fOutEvent->ymw2,header->UpMat.MWclosDist);
          VmwFa.SetXYZ(fOutEvent->xmw1,fOutEvent->ymw1,header->UpMat.MWfarDist);
          Vbeam = (VmwCl - VmwFa);
          fOutEvent->thb = Vbeam.Theta()/rad;
          fOutEvent->phib = Vbeam.Phi()/rad;
          Vbeam.GetXYZ(tarcoord);
          MdistX = fOutEvent->xmw2;
          MdistY = fOutEvent->ymw2;
          MdistZ = header->UpMat.MWclosDist+header->UpMat.MWgasThick/2;
          */
          if(fabs(fOutEvent->xmw1)<=header->UpMat.MWstep*header->UpMat.MWNwires/2.&&
          fabs(fOutEvent->ymw1)<=header->UpMat.MWstep*header->UpMat.MWNwires/2.&&
          fabs(fOutEvent->xmw2)<=header->UpMat.MWstep*header->UpMat.MWNwires/2.&&
          fabs(fOutEvent->ymw2)<=header->UpMat.MWstep*header->UpMat.MWNwires/2.)
          {
            double Zdist;
            if(abs(header->UpMat.MWclosXNum)==1) Zdist = header->UpMat.MWclosDist + header->UpMat.MWXYdist/2.;
            else if(abs(header->UpMat.MWclosXNum)==2) Zdist = header->UpMat.MWclosDist - header->UpMat.MWXYdist/2.;

            VmwCl.SetXYZ(fOutEvent->xmw2,fOutEvent->ymw2,header->UpMat.MWclosDist-header->UpMat.MWfarDist);
            VmwFa.SetXYZ(fOutEvent->xmw1,fOutEvent->ymw1,0.);
            Vbeam = VmwCl - VmwFa;
            fOutEvent->projectile.Part.SetTheta(Vbeam.Theta());
            fOutEvent->projectile.Part.SetPhi(Vbeam.Phi());
            fOutEvent->thb = Vbeam.Theta()/rad;
            fOutEvent->phib = Vbeam.Phi()/rad;

            fOutEvent->xbt = fOutEvent->xmw2 - Zdist*tan(fOutEvent->projectile.Part.Theta())*cos(fOutEvent->projectile.Part.Phi());
            fOutEvent->ybt = fOutEvent->ymw2 - Zdist*tan(fOutEvent->projectile.Part.Theta())*sin(fOutEvent->projectile.Part.Phi());
            fOutEvent->zbt = header->UpMat.TarZshift;
            
            if(sqrt(fOutEvent->xbt*fOutEvent->xbt +fOutEvent->ybt*fOutEvent->ybt)<header->UpMat.TarRadius) {header->mtrack=1;good_mtrack++;}
          }
        }
      }
}
//----------------------------------------------------------------------------
double ERBeamDetReconstructor::coordMW(UpstreamMatter* pT,ERBeamDetRecoEvent* pR,char* MWid,char* XY){
  double co = -1000.;
  double offset;
  double Sn;
  int iMW;
  int mMW;
  int nMW[16];
  char Xchoice[]="X";
  char Ychoice[]="Y";
  TRandom Rnd;
  if(!strcmp(MWid,"MWfar"))
  {
    if(!strcmp(XY,Xchoice))
    {
      iMW = pT->MWfarXNum/abs(pT->MWfarXNum);
      offset = pT->MWfarXshift;
      mMW = pR->mMW12;
      for(int k=0;k<16;k++) {nMW[k] = pR->nMW12[k];}
    }
    else if(!strcmp(XY,Ychoice))
    {
      iMW = pT->MWfarYNum/abs(pT->MWfarYNum);
      offset = pT->MWfarYshift;
      mMW = pR->mMW11;
      for(int k=0;k<16;k++) {nMW[k] = pR->nMW11[k];}
    }
    else
    {printf("hMW: it's not neither X nor Y\n");return co;}    
  }
  else if(!strcmp(MWid,"MWclo"))
  {
    if(!strcmp(XY,Xchoice))
    {
      iMW = pT->MWclosXNum/abs(pT->MWclosXNum);
      offset = pT->MWclosXshift;
      mMW = pR->mMW22;
      for(int k=0;k<16;k++) {nMW[k] = pR->nMW22[k];}
    }
    else if(!strcmp(XY,Ychoice))
    {
      iMW = pT->MWclosYNum/abs(pT->MWclosYNum);
      offset = pT->MWclosYshift;
      mMW = pR->mMW21;
      for(int k=0;k<16;k++) {nMW[k] = pR->nMW21[k];}
    }
    else
    {printf("hMW: it's not neither X nor Y\n");return co;}    
  }
  else
  {printf("hMW: bad number of MWPC\n");return co;}
  
  if(mMW<1) {return co;}
  if(mMW==1) {Sn = (double)nMW[mMW-1];}
  if(mMW>1)
  {
    int n = 0;
    for(int k=1;k<=mMW;k++)
    {n += nMW[k-1];}
    Sn = (double)n/(double)mMW;
  }
  Sn += (rand() %10000)/10000.-0.5;
  co = pT->MWstep*iMW*(Sn-(double)(pT->MWNwires+1)/2.)+offset;
  return co;
}
//----------------------------------------------------------------------------
int ERBeamDetReconstructor::mcluMW(int mMW,int* nMW){
  int i;
  if(mMW<=1) {i = mMW;}
  else
  {
    i = 1;
    for(int j=2;j<=mMW;j++)
    {
      if(nMW[j]-nMW[j-1]>1) {i++;}
    }
  }
  return i;
}
//----------------------------------------------------------------------------
void ERBeamDetReconstructor::ElossTOFaMWPCaTarget(){
  /*
  Есть два способа определения энергии конкретного
  налетающего иона по времени пролета. Более прогрессивный, но пока не
  работавший - с абсолютной калибровкой времени и пересчетом энергии из
  скорости. Более древний и реально использующийся - В дипольном магните
  на входе и выходе стоят щели, которые пропускают частицы с определенным
  отношением импульса к заряду. Величина, характеризующая это отношение -
  магнитная жесткость B*rho - где B вертикальная проекция магнитного поля,
  а rho - радиус кривизны траектории в горизонтальной плоскости. Связь
  между B*rho и энергией дается "формулой Степанцова" (Stepantsov.cpp).
  Производятся имзерения времени прорлета ионов без калибровки разности
  времен. Получается пик с некоторым центром. Положение центра пика
  сопоставляется энергии, вычисленной через B*rho/ энергия каждого
  конкретного иона считается через его ТОF - в зависимости от положения
  измерннного TOF относительно центра пика. Так вот, положение центра
  пика измеренных некалиброванных TOF- называется tof_0.
  */
  char ShowTrack[10];
  double range = header->UpMat.PlasticThick2;

  Tb = EiEo(header->UpMat.beam_TOF,fOutEvent->projectile.Part.E()-fOutEvent->projectile.Mass,header->UpMat.PlasticThick2);
  cout << "energey" <<Tb << endl;
  p_beam = sqrt(pow(Tb+fOutEvent->projectile.Mass,2)-pow(fOutEvent->projectile.Mass,2));
  fOutEvent->projectile.Part.SetPxPyPzE(0.,0.,p_beam,Tb+fOutEvent->projectile.Mass);
  if(header->ReIN.TOFis)
  {
    tof_0 = header->UpMat.PlasticDist/sqrt(1-pow(fOutEvent->projectile.Mass/fOutEvent->projectile.Part.E(),2))/slight;
    printf("\n");
    printf("TOF measured between two plastics on the base of %lf cm is %lf ns\n",header->UpMat.PlasticDist,tof_0);
  }
  strcpy(ShowTrack,"visible");
  Tb = UpstreamEnergyLoss(&(header->UpMat),&(fOutEvent->projectile),header->ReIN.TOFis,header->ReIN.TRACKINGis,ShowTrack);
  cout << "energey" <<Tb << endl;
  p_beam = sqrt(pow(Tb+fOutEvent->projectile.Mass,2)-pow(fOutEvent->projectile.Mass,2));
  fOutEvent->projectile.Part.SetPxPyPzE(0.,0.,p_beam,Tb+fOutEvent->projectile.Mass);

  //target
  if(!strcmp(header->UpMat.HeatScreenAns,"yes")) Tb = EiEo(header->UpMat.beam_TARwin,Tb,header->UpMat.HeatScreenThick);
  if(Tb>0.1) Tb = EiEo(header->UpMat.beam_TARwin,Tb,header->UpMat.FoilThick);
  if(Tb>0.1) Tb = EiEo(header->UpMat.beam_target,Tb,header->UpMat.TarThick*header->UpMat.TarPress*TempNorm/header->UpMat.TarTemp/2.);
  p_beam = sqrt(pow(Tb+fOutEvent->projectile.Mass,2)-pow(fOutEvent->projectile.Mass,2));
  fOutEvent->projectile.Part.SetPxPyPzE(0.,0.,p_beam,Tb+fOutEvent->projectile.Mass);

  fOutEvent->CM0.Part = fOutEvent->projectile.Part + fOutEvent->target.Part;
  fOutEvent->projectile.Part.Boost(-fOutEvent->CM0.Part.BoostVector());
  fOutEvent->target.Part.Boost(-fOutEvent->CM0.Part.BoostVector());

  t_cm0 = fOutEvent->projectile.Part.E()+fOutEvent->target.Part.E()-fOutEvent->projectile.Mass-fOutEvent->target.Mass;
  t_cm = t_cm0 + Qreaction;

  fOutEvent->projectile.Part.Boost(fOutEvent->CM0.Part.BoostVector());
  fOutEvent->target.Part.Boost(fOutEvent->CM0.Part.BoostVector());

  printf("\n");
  printf("TARGET: %s H=%lf cm (T=%lf K, P=%lf bar) with %s entance window H=%lf cm\n",
    fOutEvent->target.NameOfNucleus,header->UpMat.TarThick,header->UpMat.TarTemp,header->UpMat.TarPress,header->UpMat.TarFoilMatter,header->UpMat.FoilThick);
  printf("In the center of the target: %lf MeV (%lfA MeV)\n",
    fOutEvent->projectile.Part.E()-fOutEvent->projectile.Mass,(fOutEvent->projectile.Part.E()-fOutEvent->projectile.Mass)/fOutEvent->projectile.AtMass);
  printf("Ecm+Q = %lf, Ecm = %lf MeV\n",t_cm,t_cm0);
  printf("************************************************************\n");
  Tp1 = fOutEvent->projectile.Part.E()-fOutEvent->projectile.Mass;
  Tp2 = (fOutEvent->projectile.Part.E()-fOutEvent->projectile.Mass)/fOutEvent->projectile.AtMass;
  Tp3 = t_cm;
}
//-----------------------------------------------------------------------------
void ERBeamDetReconstructor::Reset()
{
  fOutEvent->Reset();
}
//-----------------------------------------------------------------------------
double ERBeamDetReconstructor::UpstreamEnergyLoss(UpstreamMatter* pU,ERParticle* pP,bool Cond1, bool Cond2,char* Show){
  char Matter[32]="visible";
  double Tb,range;
  if(Cond1)
  {
    range = pU->PlasticThick2;
    Tb = EiEo(pU->beam_TOF,pP->Part.E()-pP->Mass,range);  /*Energy after 2nd plastic*/
    if(!strcmp(Show,Matter))
    {
      printf("TOF PLASTIC2 (%s), Thickness=%lf cm\n",pU->PlasticMatter2,pU->PlasticThick2);
      printf("%lf MeV (%lfA MeV)\n",Tb,Tb/pP->AtMass);
    }
  }
  if(Cond2)
  {
    for(int i=1;i<3;i++)
    {
      range = pU->MWwinThick;
      Tb = EiEo(pU->beam_MWwin,Tb,range); /*Energy after entrance MWPC window*/

      range = pU->MWgasThick;
      Tb = EiEo(pU->beam_MWgas,Tb,range); /*Energy after MWPC gas*/

      range = pU->MWcathThick;
      Tb = EiEo(pU->beam_MWcathod,Tb,range);  /*Energy after MWPC cathodes*/

      range = pU->MWwinThick;
      Tb = EiEo(pU->beam_MWwin,Tb,range); /*Energy after exit MWPC window*/

      if(!strcmp(Show,Matter))
      {
        printf("MWPC%i: 2 %s windows (%lf cm), %s cathodes (%lf cm), gas %s (%lf cm)\n",
          i,pU->MWwinMatter,pU->MWwinThick,pU->MWcathMatter,
          pU->MWcathThick,pU->MWgasMatter,pU->MWgasThick);
        printf("%lf MeV (%lfA MeV)\n",Tb,Tb/pP->AtMass);
      }
    }
  }
  return Tb;
}
// ----------------------------------------------------------------------------
void ERBeamDetReconstructor::Finish()
{   
}
// ----------------------------------------------------------------------------
void ERBeamDetReconstructor::DefineBeamEnergy(){
  cout << "Define beam energy" << endl;
  char RightEnUn1[]="MeV";
  char RightEnUn2[]="MeV/n";
  char RightEnUn3[]="A";
  char Dipole[] = "D2";
  char RightSpreadUn2[]="%";
  char* plett = strchr(header->ReIN.AboutBeam,'=');
  plett++;
  Tb = atof(plett);
  if(!strcmp(header->ReIN.EnergyUn,RightEnUn1)) Tb = Tb;
  else if(!strcmp(header->ReIN.EnergyUn,RightEnUn2)) Tb *= fOutEvent->projectile.AtMass;
  else if(!strcmp(header->ReIN.EnergyUn,RightEnUn3)) Tb = Stepantsov(Dipole,fOutEvent->projectile.AtNumber,fOutEvent->projectile.Mass,Tb);
  else printf("Main: wrong energy unit\n");
  Tboutput = Tb;

  plett = strchr(header->ReIN.AboutSlit,'=');
  plett++;
  BeamSpread = atof(plett);
  if(!strcmp(header->ReIN.SlitUn,RightEnUn1)) BeamSpread = BeamSpread;
  else if(!strcmp(header->ReIN.SlitUn,RightSpreadUn2)) BeamSpread *= Tb/100.; 
  else printf("Main: wrong energy spread unit\n");

  fOutEvent->projectile.Part.SetPxPyPzE(0.,0.,sqrt(pow(Tb+fOutEvent->projectile.Mass,2)-pow(fOutEvent->projectile.Mass,2)),Tb+fOutEvent->projectile.Mass);
  
  //Qreaction = 0.;
  //for(int ip=0;ip<header->NofDetPart+header->NofUnObsPart;ip++) Qreaction -= ejectile[0][ip][0].Mass;
  //Qreaction +=fOutEvent->projectile.Mass + fOutEvent->target.Mass;
}
//-----------------------------------------------------------------------------
double ERBeamDetReconstructor::Stepantsov(char* D,int Z,double A,double I){
  double a0_2 = 20.19773;
  double a1_2 = 265.01317;
  double a2_2 = 54.27665;
  double a0_3 = 17.46898;
  double a1_3 = 272.31081;
  double a2_3 = 50.04487;
  double P,B,Bro,Discr,T;
  double R = 3.;
  double a0,a1,a2;
  char Dipole1[]="D1";
  char Dipole2[]="D2";
  
  if(!strcmp(D,Dipole1))
  {a0 = a0_2;
  a1 = a1_2;
  a2 = a2_2;}
  else if(!strcmp(D,Dipole2))
  {a0 = a0_3;
  a1 = a1_3;
  a2 = a2_3;}
  else
  {printf("Stepantsov: one should write D1 or D2 only!!!\n"); T = 0.;}
  Discr = sqrt(a1*a1-4.*(a0-I)*a2);
  if(Discr>0.) {B = (-a1 + Discr)/2./a2;}
  else {B = 0.;}
  Bro = B*R;
  P = Bro*Z/3.3356*1000.;
  T = sqrt(P*P + A*A) - A;
  return T;
}
//-----------------------------------------------------------------------------
void ERBeamDetReconstructor::ReadDeDx(){
  char Matter[128];
  TString filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/eloss/");

  /********************* For the TOF plastic *****************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,fOutEvent->projectile.NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,header->UpMat.PlasticMatter1);
  ReadRint(Matter,header->UpMat.beam_TOF);
  /********************* For the MWPC windows ****************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,fOutEvent->projectile.NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,header->UpMat.MWwinMatter);
  ReadRint(Matter,header->UpMat.beam_MWwin);
  /************************ For the MWPC gas *****************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,fOutEvent->projectile.NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,header->UpMat.MWgasMatter);
  ReadRint(Matter,header->UpMat.beam_MWgas);
  /********************* For the MWPC cathodes ***************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,fOutEvent->projectile.NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,header->UpMat.MWcathMatter);
  ReadRint(Matter,header->UpMat.beam_MWcathod);
  /********************* For the heat screen **************************/
  //  strcpy(Matter,"../../../include/eloss/");
  //  strcat(Matter,fOutEvent->projectile.NameOfNucleus);
  //  strcat(Matter,"_");
  //  strcat(Matter,header->UpMat.TarFoilMatter);
  //  ReadRint(Matter,header->UpMat.beam_heatscreen);
  /********************* For the target windows **************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,fOutEvent->projectile.NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,header->UpMat.TarFoilMatter);
  ReadRint(Matter,header->UpMat.beam_TARwin);
  /********************* For the target material *************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,fOutEvent->projectile.NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,fOutEvent->target.NameOfNucleus);
  ReadRint(Matter,header->UpMat.beam_target);
  /*************************************************************************/
}
ClassImp(ERBeamDetReconstructor)
