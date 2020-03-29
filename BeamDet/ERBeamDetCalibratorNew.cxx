#include "ERBeamDetCalibratorNew.h"

#include <iostream>

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERBeamDetCalibratorNew::ERBeamDetCalibratorNew()
  : FairTask("Convert Raw event to analyse event"),
  fInEvent(NULL),
  fPath(""),
  fBeamDetToFDigi(NULL), 
  fBeamDetMWPCDigi(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetCalibratorNew::ERBeamDetCalibratorNew(Int_t verbose)
  : FairTask("Convert Raw event to analyse event", verbose),
  fInEvent(NULL),
  fPath(""),
  fBeamDetToFDigi(NULL), 
  fBeamDetMWPCDigi(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetCalibratorNew::~ERBeamDetCalibratorNew()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetCalibratorNew::SetParContainers()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERBeamDetCalibratorNew::Init()
{
  if (fPath == "") Fatal("Init", "No parameters file in ERBeamDetCalibratorNew");

  FILE *F2 = fopen(fPath.Data(),"r");
  if(F2==NULL) {
    //Fatal("Init", TString("ERBeamDetCalibratorNew: file ") + fPath + TString(" is not found"));
    Fatal("Init", "ERBeamDetCalibratorNew: file %s is not found", fPath.Data());
  } else {
    double a,b,t;
    char comments[128];
    int it,il,is;
    int res;
    res = fscanf(F2,"%s %s %s %s %s %s %s\n",comments,comments,comments,comments,comments,comments,comments);
    while(!feof(F2))
    {
      int res2;
      res2 = fscanf(F2,"%i %i %i %lf %lf %lf %s\n",&it,&il,&is,&a,&b,&t,comments);
      CLB[it][il][is][0] = b;
      CLB[it][il][is][1] = a;
      CLB[it][il][is][2] = t;
    }
  }

  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  fInEvent = (ERBeamDetEvent*)ioman->GetObject("BeamDetEvent.");
  if (!fInEvent)
      Fatal("Init", "Can`t find branch in input file!");

  // Register output array fRTelescopeHits
  fBeamDetToFDigi = new TClonesArray("ERBeamDetTOFDigi",1000);
  fBeamDetMWPCDigi = new TClonesArray("ERBeamDetMWPCDigi",1000);

  ioman->Register("BeamDetToFDigi", "BeamDetToF Digi", fBeamDetToFDigi, kTRUE);
  ioman->Register("BeamDetMWPCDigi", "BeamDetMWPC Digi", fBeamDetMWPCDigi, kTRUE);

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetCalibratorNew::Exec(Option_t* opt)
{
  Reset();

  if (fInEvent->nx1 != 1 || fInEvent->ny1 != 1 || fInEvent->nx2 != 1 || fInEvent->ny2 != 1){
    FairRun* run = FairRun::Instance();
    run->MarkFill(kFALSE);
    return;
  }

  AddMWPCDigi(-1., -1.,0,0, fInEvent->x1[0]);
  AddMWPCDigi(-1., -1.,0,1, fInEvent->y1[0]);
  AddMWPCDigi(-1., -1.,1,0, fInEvent->x2[0]);
  AddMWPCDigi(-1., -1.,1,1, fInEvent->y2[0]);

  Double_t time, edep;
  time = (fInEvent->ntF3r+Ran.Uniform(-0.5,0.5))*CLB[2][1][0][0];
  edep = (fInEvent->naF3r+Ran.Uniform(-0.5,0.5));
  AddToFDigi(edep,time,0);

  time = (fInEvent->ntF4l+Ran.Uniform(-0.5,0.5))*CLB[3][1][0][0];
  edep = (fInEvent->naF4l+Ran.Uniform(-0.5,0.5));
  AddToFDigi(edep,time,1);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERBeamDetCalibratorNew::Reset()
{
  if (fBeamDetMWPCDigi) {
    fBeamDetMWPCDigi->Delete();
  }
  if (fBeamDetToFDigi) {
    fBeamDetToFDigi->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetCalibratorNew::Finish()
{   
}
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetMWPCDigi* ERBeamDetCalibratorNew::AddMWPCDigi(Float_t edep, Double_t time, Int_t mwpcNb, Int_t planeNb, Int_t wireNb)
{
  ERBeamDetMWPCDigi *digi = new((*fBeamDetMWPCDigi)[fBeamDetMWPCDigi->GetEntriesFast()])
              ERBeamDetMWPCDigi(edep, time, mwpcNb, planeNb, wireNb);
  return digi;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ERBeamDetTOFDigi* ERBeamDetCalibratorNew::AddToFDigi(Float_t edep, Double_t time, Int_t ToFNb)
{
  ERBeamDetTOFDigi *digi = new((*fBeamDetToFDigi)[fBeamDetToFDigi->GetEntriesFast()])
              ERBeamDetTOFDigi(edep, time, ToFNb);
  return digi;
}
//-----------------------------------------------------------------------------

ClassImp(ERBeamDetCalibratorNew)
