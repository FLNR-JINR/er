/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeSiPoint_H
#define ERQTelescopeSiPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"


class ERQTelescopeSiPoint : public FairMCPoint {
 public:
  ERQTelescopeSiPoint() = default;
  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param posIn    Ccoordinates at entrance of point [cm]
   *@param posOut   Coordinates at exit of point [cm]
   *@param momIn    Momentum of track at entrance [MeV]
   *@param momOut   Momentum of track at exit [MeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [MeV]
   **/
  ERQTelescopeSiPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss, Int_t X_StripNb, Int_t pdg);
  /** Destructor **/
  virtual ~ERQTelescopeSiPoint() = default;
  /** Accessors **/
  Int_t GetEventID()            const { return fEventID; }
  Int_t GetMot0TrackID()        const { return fMot0TrackID; }
  Double_t GetXIn()             const { return fX; }
  Double_t GetYIn()             const { return fY; }
  Double_t GetZIn()             const { return fZ; }
  Double_t GetXOut()            const { return fX_out; }
  Double_t GetYOut()            const { return fY_out; }
  Double_t GetZOut()            const { return fZ_out; }
  Double_t GetPxOut()           const { return fPx_out; }
  Double_t GetPyOut()           const { return fPy_out; }
  Double_t GetPzOut()           const { return fPz_out; }
  Double_t GetMass()            const { return fMass; }
  Int_t    GetPDG()             const { return fPDG; }
  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }
  /** Point coordinates at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;
  virtual Int_t GetStripNb()   {return fStrip;}
  /** Check for distance between in and out **/
  Bool_t IsUsable() const;
  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;
 protected:
  Int_t      fEventID = -1;
  Int_t      fMot0TrackID = -1;
  Double_t   fMass = -1.;
  Double32_t fX_out = 0.,  fY_out = 0.,  fZ_out = 0.;
  Double32_t fPx_out = 0., fPy_out = 0., fPz_out = 0.;
  Int_t      fStrip = -1;
  Int_t      fPDG = -1;
  ClassDef(ERQTelescopeSiPoint,1)
};
#endif

