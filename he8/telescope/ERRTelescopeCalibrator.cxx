#include "ERRTelescopeCalibrator.h"

#include<iostream>
using namespace std;



#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERRTelescopeCalibrator::ERRTelescopeCalibrator()
  : FairTask("Convert Raw event to analyse event"),
  fInEvent(NULL),
  fOutEvent(NULL),
  fPath(""),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeCalibrator::ERRTelescopeCalibrator(Int_t verbose)
  : FairTask("Convert Raw event to analyse event", verbose),
  fInEvent(NULL),
  fOutEvent(NULL),
  fPath(""),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeCalibrator::~ERRTelescopeCalibrator()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeCalibrator::SetParContainers()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERRTelescopeCalibrator::Init()
{
  if (fPath == "") Fatal("Init", "No parameters file in ERRTelescopeCalibrator");

  FILE *F2 = fopen(fPath.Data(),"r");
  if(F2==NULL) 
    Fatal("Init", TString("RDsrdCalibrator: file ") + fPath + TString(" is not found"));
  else
  {
    double a,b,t;
    char comments[128];
    int it,il,is;
    fscanf(F2,"%s %s %s %s %s %s %s\n",comments,comments,comments,comments,comments,comments,comments);
    while(!feof(F2))
    {
      fscanf(F2,"%i %i %i %lf %lf %lf %s\n",&it,&il,&is,&a,&b,&t,comments);
      CLB[it][il][is][0] = b;
      CLB[it][il][is][1] = a;
      CLB[it][il][is][2] = t;
    }
  }

  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  fInEvent = (ERRTelescopeEvent*)ioman->GetObject("RTelescopeEvent.");
  if (!fInEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fOutEvent = new ERRTelescopeCalEvent();
  ioman->Register("RTelescopeCalEvent.","Analyze",fOutEvent, kTRUE);

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERRTelescopeCalibrator::Exec(Option_t* opt)
{
  //std::cout << std::endl;
  //std::cout << "####### ERRTelescopeCalibrator EVENT " << fEvent++ << " #####" << std::endl;

  Reset();

  double EP;
  
  for(int imu=0;imu<=fInEvent->mD11;imu++)  /* D11 16 sectors */
  {
    EP=CLB[0][0][fInEvent->nD11[imu]][0]*(fInEvent->neD11[imu]+Ran.Uniform(-0.5,0.5))+CLB[0][0][fInEvent->nD11[imu]][1];
    if(EP>CLB[0][0][fInEvent->nD11[imu]][2])
    {
      fOutEvent->mC11++;
      fOutEvent->eC11[fOutEvent->mC11]=EP;
      fOutEvent->nC11[fOutEvent->mC11]=fInEvent->nD11[imu];
    }
  }
  for(int imu=0;imu<=fInEvent->mD12;imu++)  /* D12 16 rings */
  {
    EP=CLB[0][1][fInEvent->nD12[imu]][0]*(fInEvent->neD12[imu]+Ran.Uniform(-0.5,0.5))+CLB[0][1][fInEvent->nD12[imu]][1];
    if(EP>CLB[0][1][fInEvent->nD12[imu]][2])
    {
      fOutEvent->mC12++;
      fOutEvent->eC12[fOutEvent->mC12]=EP;
      fOutEvent->nC12[fOutEvent->mC12]=fInEvent->nD12[imu];
    }
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeCalibrator::Reset()
{
  fOutEvent->Reset();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeCalibrator::Finish()
{   
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERRTelescopeCalibrator)
