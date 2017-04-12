#include "ERTofCalibrator.h"

#include<iostream>
using namespace std;



#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERTofCalibrator::ERTofCalibrator()
  : FairTask("Convert Raw event to analyse event"),
  fInEvent(NULL),
  fOutEvent(NULL),
  fPath(""),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTofCalibrator::ERTofCalibrator(Int_t verbose)
  : FairTask("Convert Raw event to analyse event", verbose),
  fInEvent(NULL),
  fOutEvent(NULL),
  fPath(""),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTofCalibrator::~ERTofCalibrator()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTofCalibrator::SetParContainers()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERTofCalibrator::Init()
{
  if (fPath == "") Fatal("Init", "No parameters file in ERTofCalibrator");

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
  fInEvent = (ERTofEvent*)ioman->GetObject("TofEvent.");
  if (!fInEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fOutEvent = new ERTofCalEvent();
  ioman->Register("TofCalEvent.","Analyze",fOutEvent, kTRUE);

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERTofCalibrator::Exec(Option_t* opt)
{
  //std::cout << std::endl;
  //std::cout << "####### ERTofCalibrator EVENT " << fEvent++ << " #####" << std::endl;
  fOutEvent->tF3l = (fInEvent->ntF3l+Ran.Uniform(-0.5,0.5))*CLB[2][0][0][0];
  fOutEvent->tF3r = (fInEvent->ntF3r+Ran.Uniform(-0.5,0.5))*CLB[2][1][0][0];
  fOutEvent->tF4r = (fInEvent->ntF4r+Ran.Uniform(-0.5,0.5))*CLB[3][0][0][0];
  fOutEvent->tF4l = (fInEvent->ntF4l+Ran.Uniform(-0.5,0.5))*CLB[3][1][0][0];
  fOutEvent->aF3l = (fInEvent->naF3l+Ran.Uniform(-0.5,0.5));
  fOutEvent->aF3r = (fInEvent->naF3r+Ran.Uniform(-0.5,0.5));
  fOutEvent->aF4r = (fInEvent->naF4r+Ran.Uniform(-0.5,0.5));
  fOutEvent->aF4l = (fInEvent->naF4l+Ran.Uniform(-0.5,0.5));
  Reset();

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERTofCalibrator::Reset()
{
  fOutEvent->Reset();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTofCalibrator::Finish()
{   
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERTofCalibrator)
