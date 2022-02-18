// -------------------------------------------------------------------------
// -----                      ERGadastLaBrPoint source file                -----
// -------------------------------------------------------------------------

#include "ERGadastLaBrPoint.h"
#include<iostream>
// -----   Default constructor   -------------------------------------------
ERGadastLaBrPoint::ERGadastLaBrPoint()
  : FairMCPoint(),
    fX_out(0.), fY_out(0.), fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.)
{
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
ERGadastLaBrPoint::ERGadastLaBrPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss, Int_t pdg,  Int_t cell)
  : FairMCPoint(trackID, -1., posIn, momIn, tof, length, eLoss),
    fEventID(eventID),
    fMot0TrackID(mot0trackID),
    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z()),
    fPDG(pdg), fCell(cell)
{
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
ERGadastLaBrPoint::ERGadastLaBrPoint(const ERGadastLaBrPoint& right)
  : FairMCPoint(right),
    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out),
    fPDG(right.fPDG), fCell(right.fCell)
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ERGadastLaBrPoint::~ERGadastLaBrPoint()
{
}
// -------------------------------------------------------------------------



// -----   Public method Print   -------------------------------------------
void ERGadastLaBrPoint::Print(const Option_t* opt /* = 0*/) const
{
  std::cout << "-I- ERGadastLaBrPoint: track " << fTrackID << " mother track = " << fMot0TrackID << std::endl;
  std::cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << std::endl;
  std::cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << std::endl;
  std::cout << "    Time " << fTime << " ns,  Length " << fLength << " cm" << std::endl;
  std::cout << "    Energy loss " << fELoss << " keV "<< std::endl;
}
// -------------------------------------------------------------------------



// -----   Point x coordinate from linear extrapolation   ------------------
Double_t ERGadastLaBrPoint::GetX(Double_t z) const
{
  //  cout << fZ << " " << z << " " << fZ_out << endl;
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
// -------------------------------------------------------------------------



// -----   Point y coordinate from linear extrapolation   ------------------
Double_t ERGadastLaBrPoint::GetY(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
// -------------------------------------------------------------------------



// -----   Public method IsUsable   ----------------------------------------
Bool_t ERGadastLaBrPoint::IsUsable() const
{
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------



ClassImp(ERGadastLaBrPoint)
