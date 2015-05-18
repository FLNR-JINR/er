// -------------------------------------------------------------------------
// -----                       ERNeuRadDigi source file                -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERNeuRadDigi.h"
#include "FairLogger.h"

// -----   Default constructor   -------------------------------------------
ERNeuRadDigi::ERNeuRadDigi()
  :fDigi_nr(-1),
  fFrontTDC(0.),
  fBackTDC(0.),
  fTDC(0.),
  fFrontQDC(0.),
  fBackQDC(0.),
  fQDC(0.),
  fFiber_nr(-1)
{
}



ERNeuRadDigi::ERNeuRadDigi(Int_t digi_nr, Double_t frontTDC, Double_t backTDC,
                                      Double_t TDC, Double_t frontQDC, Double_t backQDC, Double_t QDC,
                                      Int_t fiber_nr)
  :fDigi_nr(digi_nr),
  fFrontTDC(frontTDC),
  fBackTDC(backTDC),
  fTDC(TDC),
  fFrontQDC(frontQDC),
  fBackQDC(backQDC),
  fQDC(QDC),
  fFiber_nr(fiber_nr)
{
}

ERNeuRadDigi::ERNeuRadDigi(const ERNeuRadDigi& right)
  :fDigi_nr(right.fDigi_nr),
  fFrontTDC(right.fFrontTDC),
  fBackTDC(right.fBackTDC),
  fTDC(right.fTDC),
  fFrontQDC(right.fFrontQDC),
  fBackQDC(right.fBackQDC),
  fQDC(right.fQDC),
  fFiber_nr(right.fFiber_nr)
{
}



// -----   Destructor   ----------------------------------------------------
ERNeuRadDigi::~ERNeuRadDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERNeuRadDigi::Print(const Option_t* opt /* = 0 */) const
{
  LOG(INFO) << "-I- ERNeuRadDigi:  " << FairLogger::endl;
  LOG(INFO) << "    Fiber : " << fFiber_nr << FairLogger::endl;
  LOG(INFO) << "    FrontTDC: " << fFrontTDC << " BackTDC " << fBackTDC << FairLogger::endl;
  LOG(INFO) << "    FrontQDC: " << fFrontQDC << " BackQDC " << fBackQDC << FairLogger::endl;
}
// -------------------------------------------------------------------------



ClassImp(ERNeuRadDigi)
