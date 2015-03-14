// -------------------------------------------------------------------------
// -----                      ExpertNeuRadPoint source file            -----
// -------------------------------------------------------------------------

#include "ExpertNeuRadPoint.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
ExpertNeuRadPoint::ExpertNeuRadPoint()
  : FairMCPoint(),
    fX_out(0.), fY_out(0.), fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.)
{
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
ExpertNeuRadPoint::ExpertNeuRadPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
      Int_t moduleInBundleNb,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss, Double_t lightYield)
  : FairMCPoint(trackID, -1., posIn, momIn, tof, length, eLoss),
    fModuleInBundleNb(moduleInBundleNb),
    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z()),
    fLightYield(lightYield)
{
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
ExpertNeuRadPoint::ExpertNeuRadPoint(const ExpertNeuRadPoint& right)
  : FairMCPoint(right),
    fModuleInBundleNb(right.fModuleInBundleNb),
    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out),
    fLightYield(right.fLightYield)
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ExpertNeuRadPoint::~ExpertNeuRadPoint()
{
}
// -------------------------------------------------------------------------



// -----   Public method Print   -------------------------------------------
void ExpertNeuRadPoint::Print(const Option_t* opt) const
{
  cout << "-I- ExpertNeuRadPoint: LAND Point for track " << fTrackID
       << " in detector " << fDetectorID
       //<< " Sector: " << fSector
     //  << " PaddleTyp: " << fPaddleTyp
     //  << " PaddleNb: " << fPaddleNb 
	 << endl; 
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength 
       << " cm,  Energy loss " << fELoss*1.0e06 << " keV" << endl;
}
// -------------------------------------------------------------------------



// -----   Point x coordinate from linear extrapolation   ------------------
Double_t ExpertNeuRadPoint::GetX(Double_t z) const
{
  //  cout << fZ << " " << z << " " << fZ_out << endl;
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
// -------------------------------------------------------------------------



// -----   Point y coordinate from linear extrapolation   ------------------
Double_t ExpertNeuRadPoint::GetY(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
// -------------------------------------------------------------------------



// -----   Public method IsUsable   ----------------------------------------
Bool_t ExpertNeuRadPoint::IsUsable() const
{
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------



ClassImp(ExpertNeuRadPoint)
