/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERPoint.h"

#include <cmath>
#include<iostream>

#include "FairLogger.h"

// -----   Default constructor   -------------------------------------------
ERPoint::ERPoint()
  : FairMCPoint(),
    fX_out(0.), fY_out(0.), fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.)
{
}
// -------------------------------------------------------------------------
// -----   Standard constructor   ------------------------------------------
ERPoint::ERPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
      Int_t volNb,
		  Double_t mass,
		  TVector3 posIn, TVector3 posInLoc,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t timeIn, Double_t timeOut, Double_t trackLength, Double_t eLoss, Double_t lightYield, Int_t pid, Double_t charge)
  : FairMCPoint(trackID, -1., posIn, momIn, timeIn, 0., eLoss),
    fEventID(eventID),
    fVolNb(volNb),
    fMot0TrackID(mot0trackID),
    fXlocal(posInLoc.X()),fYlocal(posInLoc.Y()), fZlocal(posInLoc.Z()), 
    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z()),
    fLightYield(lightYield), fPID(pid), fCharge(charge),fTimeIn(timeIn), fTimeOut(timeOut),
    fTrackLength(trackLength)
{
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
ERPoint::ERPoint(const ERPoint& right)
  : FairMCPoint(right),
    fVolNb(right.fVolNb),
    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out),
    fLightYield(right.fLightYield), fPID(right.fPID), fCharge(right.fCharge),
    fTimeIn(right.fTimeIn),fTimeOut(right.fTimeOut), fTrackLength(right.fTrackLength)
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ERPoint::~ERPoint()
{
}
// -------------------------------------------------------------------------



// -----   Public method Print   -------------------------------------------
void ERPoint::Print(const Option_t* opt /* = 0*/) const
{
  LOG(INFO) << "-I- ERPoint: track " << fTrackID << " mother track = " << fMot0TrackID <<  FairLogger::endl;
  LOG(INFO) << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << FairLogger::endl;
  LOG(INFO) << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << FairLogger::endl;
  LOG(INFO) << "    Time " << fTime << " ns,  Length " << fLength << " cm" << FairLogger::endl;
  LOG(INFO) << "    Energy loss " << fELoss << " keV, Light yield " << fLightYield << " MeV "<<  FairLogger::endl;
  LOG(INFO) << "    Vol number " << fVolNb << FairLogger::endl;
}
// -------------------------------------------------------------------------



// -----   Point x coordinate from linear extrapolation   ------------------
Double_t ERPoint::GetX(Double_t z) const
{
  //  cout << fZ << " " << z << " " << fZ_out << endl;
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
// -------------------------------------------------------------------------



// -----   Point y coordinate from linear extrapolation   ------------------
Double_t ERPoint::GetY(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
// -------------------------------------------------------------------------



// -----   Public method IsUsable   ----------------------------------------
Bool_t ERPoint::IsUsable() const
{
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------

//-------------------------------------------------------------------------
Double_t ERPoint::GetPIn()   const{
  return sqrt(fPx*fPx + fPy*fPy + fPz*fPz);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
Double_t ERPoint::GetPOut()  const{
  return sqrt(fPx_out*fPx_out + fPy_out*fPy_out + fPz_out*fPz_out);
}
//-------------------------------------------------------------------------
Double_t ERPoint::GetP(Double_t pointLen) const{
  return GetPIn() + (GetPOut() - GetPIn())*pointLen/GetLength();
}
//-------------------------------------------------------------------------
Double_t ERPoint::GetTime(Double_t pointLen) const{
  return fTime + (fTimeOut - fTime)*pointLen/GetLength();
}
//-------------------------------------------------------------------------
Double_t ERPoint::GetLength() const{
  return sqrt((fX_out-fX)*(fX_out-fX) + (fY_out-fY)*(fY_out-fY) + (fZ_out-fZ)*(fZ_out-fZ) );
}
//-------------------------------------------------------------------------
ClassImp(ERPoint)
