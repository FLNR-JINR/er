/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadPoint.h"

#include "FairLogger.h"

// -----   Default constructor   -------------------------------------------
//TODO initialize all data membders!
ERNeuRadPoint::ERNeuRadPoint()
  : FairMCPoint(),
    fX_out(0.),  fY_out(0.),  fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERNeuRadPoint::ERNeuRadPoint(Int_t eventID,
                             Int_t trackID,
                             Int_t mot0trackID,

                             //Int_t fiberNb,
                             //Int_t pixelNb,
                             //Int_t moduleNb,
                             UInt_t pmtId,
                             UInt_t chId,

                             Double_t mass,

                             // IN, INloc, OUT, OUTloc
                             TVector3 posIn,
                             TVector3 posInLoc,
                             TVector3 posOut,
                             TVector3 posOutLoc,

                             TVector3 momIn,
                             TVector3 momOut,
                             Double_t timeIn,
                             Double_t timeOut,
                             Double_t trackLength,
                             Double_t eLoss,
                             Double_t lightYield,
                             Int_t pid,
                             Double_t charge)
  : FairMCPoint(trackID, -1., posIn, momIn, timeIn, 0., eLoss),
    fEventID(eventID),

    //fFiberNb(fiberNb),
    //fPixelNb(pixelNb),
    //fModuleNb(moduleNb),
    fPmtId(pmtId),
    fChId(chId),

    fXInLocal(posInLoc.X()),
    fYInLocal(posInLoc.Y()),
    fZInLocal(posInLoc.Z()),
    fX_out(posOut.X()),
    fY_out(posOut.Y()),
    fZ_out(posOut.Z()),
    fXOutLocal(posOutLoc.X()),
    fYOutLocal(posOutLoc.Y()),
    fZOutLocal(posOutLoc.Z()),

    fPx_out(momOut.X()),
    fPy_out(momOut.Y()),
    fPz_out(momOut.Z()),
    fLightYield(lightYield),
    fPID(pid),
    fCharge(charge),
    fTimeIn(timeIn),
    fTimeOut(timeOut),
    fTrackLength(trackLength)
{
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
ERNeuRadPoint::ERNeuRadPoint(const ERNeuRadPoint& right)
  : FairMCPoint(right),

    //fFiberNb(right.fFiberNb),
    //fPixelNb(right.fPixelNb),
    //fModuleNb(right.fModuleNb),
    fPmtId(right.fPmtId),
    fChId(right.fChId),

    fXInLocal(right.fXInLocal),
    fYInLocal(right.fYInLocal),
    fZInLocal(right.fZInLocal),
    fX_out(right.fX_out),
    fY_out(right.fY_out),
    fZ_out(right.fZ_out),
    fXOutLocal(right.fXOutLocal),
    fYOutLocal(right.fYOutLocal),
    fZOutLocal(right.fZOutLocal),
    
    fPx_out(right.fPx_out),
    fPy_out(right.fPy_out),
    fPz_out(right.fPz_out),
    fLightYield(right.fLightYield),
    fPID(right.fPID),
    fCharge(right.fCharge),
    fTimeIn(right.fTimeIn),
    fTimeOut(right.fTimeOut),
    fTrackLength(right.fTrackLength)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
ERNeuRadPoint::~ERNeuRadPoint()
{
}
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void ERNeuRadPoint::Print(const Option_t* opt /* = 0*/) const
{
  LOG(INFO) << "-I- ERNeuRadPoint: track " << fTrackID << " mother track = " << fMot0TrackID <<  FairLogger::endl;
  LOG(INFO) << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << FairLogger::endl;
  LOG(INFO) << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << FairLogger::endl;
  LOG(INFO) << "    Time " << fTime << " ns,  Length " << fLength << " cm" << FairLogger::endl;
  LOG(INFO) << "    Energy loss " << fELoss << " keV, Light yield " << fLightYield << " MeV "<<  FairLogger::endl;
  //LOG(INFO) << "    Fiber number " << fFiberNb << " Pixel number " << fPixelNb << " Module number " << fModuleNb << FairLogger::endl;
  LOG(INFO) << "    PMT ID " << fPmtId << " , channel ID " << fChId << FairLogger::endl;
}
// -------------------------------------------------------------------------

// -----   Point x coordinate from linear extrapolation   ------------------
Double_t ERNeuRadPoint::GetX(Double_t z) const {
  //  cout << fZ << " " << z << " " << fZ_out << endl;
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
// -------------------------------------------------------------------------

// -----   Point y coordinate from linear extrapolation   ------------------
Double_t ERNeuRadPoint::GetY(Double_t z) const {
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
// -------------------------------------------------------------------------

// -----   Public method IsUsable   ----------------------------------------
Bool_t ERNeuRadPoint::IsUsable() const {
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------

//-------------------------------------------------------------------------
Double_t ERNeuRadPoint::GetPIn() const {
  return TMath::Sqrt(fPx*fPx + fPy*fPy + fPz*fPz);
}
//-------------------------------------------------------------------------
Double_t ERNeuRadPoint::GetPOut() const {
  return TMath::Sqrt(fPx_out*fPx_out + fPy_out*fPy_out + fPz_out*fPz_out);
}
//-------------------------------------------------------------------------
Double_t ERNeuRadPoint::GetP(Double_t pointLen) const {
  return GetPIn() + (GetPOut() - GetPIn()) * pointLen / GetLength(); //TODO check!!! normalize momentum by point length?
}
//-------------------------------------------------------------------------
Double_t ERNeuRadPoint::GetTime(Double_t pointLen) const {
  return fTime + (fTimeOut - fTime) * pointLen / GetLength(); //TODO check!!! normalize time by point length?
}
//-------------------------------------------------------------------------
Double_t ERNeuRadPoint::GetLength() const {
  return TMath::Sqrt((fX_out-fX)*(fX_out-fX) +
                     (fY_out-fY)*(fY_out-fY) +
                     (fZ_out-fZ)*(fZ_out-fZ) );
}
//-------------------------------------------------------------------------

ClassImp(ERNeuRadPoint)
