/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeCsIPoint.h"

#include "FairLogger.h"
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIPoint::ERQTelescopeCsIPoint()
  : FairMCPoint(),
    fX_out(0.), fY_out(0.), fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.),
    fWallNb(-1), fBlockNb(-1)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIPoint::ERQTelescopeCsIPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss,Int_t wallNb ,Int_t blockNb)
  : FairMCPoint(trackID, -1., posIn, momIn, tof, length, eLoss),
    fEventID(eventID),
    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z()),
    fWallNb(wallNb), fBlockNb(blockNb)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIPoint::ERQTelescopeCsIPoint(const ERQTelescopeCsIPoint& right)
  : FairMCPoint(right),
    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out),
    fWallNb(right.fWallNb), fBlockNb(right.fBlockNb)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIPoint::~ERQTelescopeCsIPoint() {
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeCsIPoint::Print(const Option_t* opt /* = 0*/) const {
  LOG(DEBUG) << "-I- ERQTelescopeCsIPoint: track " << fTrackID << " mother track = " << fMot0TrackID << FairLogger::endl;
  LOG(DEBUG) << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << FairLogger::endl;
  LOG(DEBUG) << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << FairLogger::endl;
  LOG(DEBUG) << "    Time " << fTime << " ns,  Length " << fLength << " cm" << FairLogger::endl;
  LOG(DEBUG) << "    Energy loss " << fELoss << " keV "<< FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
Double_t ERQTelescopeCsIPoint::GetX(Double_t z) const {
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
//-------------------------------------------------------------------------------------------------
Double_t ERQTelescopeCsIPoint::GetY(Double_t z) const {
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
//-------------------------------------------------------------------------------------------------
Bool_t ERQTelescopeCsIPoint::IsUsable() const {
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeCsIPoint)
