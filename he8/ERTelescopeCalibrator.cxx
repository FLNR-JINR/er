#include "ERTelescopeCalibrator.h"

#include<iostream>
using namespace std;



#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERTelescopeCalibrator::ERTelescopeCalibrator()
  : FairTask("Convert Raw event to analyse event"),
  fInEvent(NULL),
  fOutEvent(NULL),
  fPath(""),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTelescopeCalibrator::ERTelescopeCalibrator(Int_t verbose)
  : FairTask("Convert Raw event to analyse event", verbose),
  fInEvent(NULL),
  fOutEvent(NULL),
  fPath(""),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTelescopeCalibrator::~ERTelescopeCalibrator()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTelescopeCalibrator::SetParContainers()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERTelescopeCalibrator::Init()
{
  if (fPath == "") Fatal("Init", "No parameters file in ERTelescopeCalibrator");

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
  fInEvent = (ERTelescopeEvent*)ioman->GetObject("TelescopeEvent.");
  if (!fInEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fOutEvent = new ERTelescopeCalEvent();
  ioman->Register("TelescopeCalEvent.","Analyze",fOutEvent, kTRUE);

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERTelescopeCalibrator::Exec(Option_t* opt)
{
  //std::cout << std::endl;
  //std::cout << "####### ERTelescopeCalibrator EVENT " << fEvent++ << " #####" << std::endl;

  Reset();

  double EP;
  
for(int imu=0;imu<=fInEvent->mD21;imu++)  /* D21 X16 */
  {
    EP=CLB[1][0][fInEvent->nD21[imu]][0]*(fInEvent->neD21[imu]+Ran.Uniform(-0.5,0.5))+CLB[1][0][fInEvent->nD21[imu]][1];
    if(EP>CLB[1][0][fInEvent->nD21[imu]][2])
    {
      fOutEvent->mC21++;
      fOutEvent->eC21[fOutEvent->mC21]=EP;
      fOutEvent->nC21[fOutEvent->mC21]=fInEvent->nD21[imu];
      for(int imut=0;imut<=fInEvent->mDT21;imut++)
      {
        if(fOutEvent->nC21[fOutEvent->mC21]==fInEvent->nDT21[imut])
        {
          fOutEvent->mCT21++;
          fOutEvent->nCT21[fOutEvent->mCT21] = fInEvent->nDT21[imut];
          fOutEvent->tC21[fOutEvent->mCT21] = double(fInEvent->ntD21[imut]);
        }
      }
    }
  }
  for(int imu=0;imu<=fInEvent->mD22;imu++)  /* D22 Y16 */
  {
    EP=CLB[1][1][fInEvent->nD22[imu]][0]*(fInEvent->neD22[imu]+Ran.Uniform(-0.5,0.5))+CLB[1][1][fInEvent->nD22[imu]][1];
    if(EP>CLB[1][1][fInEvent->nD22[imu]][2])
    {
      fOutEvent->mC22++;
      fOutEvent->eC22[fOutEvent->mC22]=EP;
      fOutEvent->nC22[fOutEvent->mC22]=fInEvent->nD22[imu];
      for(int imut=0;imut<=fInEvent->mDT22;imut++)
      {
        if(fOutEvent->nC22[fOutEvent->mC22]==fInEvent->nDT22[imut])
        {
          fOutEvent->mCT22++;
          fOutEvent->nCT22[fOutEvent->mCT22] = fInEvent->nDT22[imut];
          fOutEvent->tC22[fOutEvent->mCT22] = double(fInEvent->ntD22[imut]);
        }
      }
    }
  }
  for(int imu=0;imu<=fInEvent->mD23;imu++)  /* D23 Y16 */
  {
    EP=CLB[1][2][fInEvent->nD23[imu]][0]*(fInEvent->neD23[imu]+Ran.Uniform(-0.5,0.5))+CLB[1][2][fInEvent->nD23[imu]][1];
    if(EP>CLB[1][2][fInEvent->nD23[imu]][2])
    {
      fOutEvent->mC23++;
      fOutEvent->eC23[fOutEvent->mC23]=EP;
      fOutEvent->nC23[fOutEvent->mC23]=fInEvent->nD23[imu];
    }
  }
  for(int imu=0;imu<=fInEvent->mD24;imu++)  /* D24 Y16 */
  {
    EP=CLB[1][3][fInEvent->nD24[imu]][0]*(fInEvent->neD24[imu]+Ran.Uniform(-0.5,0.5))+CLB[1][3][fInEvent->nD24[imu]][1];
    if(EP>CLB[1][3][fInEvent->nD24[imu]][2])
    {
      fOutEvent->mC24++;
      fOutEvent->eC24[fOutEvent->mC24]=EP;
      fOutEvent->nC24[fOutEvent->mC24]=fInEvent->nD24[imu];
    }
  }
  for(int imu=0;imu<=fInEvent->mD25;imu++)  /* D25 Y16 */
  {
    EP=CLB[1][4][fInEvent->nD25[imu]][0]*(fInEvent->neD25[imu]+Ran.Uniform(-0.5,0.5))+CLB[1][4][fInEvent->nD25[imu]][1];
    if(EP>CLB[1][4][fInEvent->nD25[imu]][2])
    {
      fOutEvent->mC25++;
      fOutEvent->eC25[fOutEvent->mC25]=EP;
      fOutEvent->nC25[fOutEvent->mC25]=fInEvent->nD25[imu];
    }
  }
  for(int imu=0;imu<=fInEvent->mD26;imu++)  /* D26 Y16 */
  {
    EP=CLB[1][5][fInEvent->nD26[imu]][0]*(fInEvent->neD26[imu]+Ran.Uniform(-0.5,0.5))+CLB[1][5][fInEvent->nD26[imu]][1];
    if(EP>CLB[1][5][fInEvent->nD26[imu]][2])
    {
      fOutEvent->mC26++;
      fOutEvent->eC26[fOutEvent->mC26]=EP;
      fOutEvent->nC26[fOutEvent->mC26]=fInEvent->nD26[imu];
    }
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERTelescopeCalibrator::Reset()
{
  fOutEvent->Reset();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTelescopeCalibrator::Finish()
{   
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERTelescopeCalibrator)
