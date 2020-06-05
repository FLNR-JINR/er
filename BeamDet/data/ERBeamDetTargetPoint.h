// -------------------------------------------------------------------------
// -----                      ERBeamDetTargetPoint header file                -----
// -----                  Created data  developerName                  -----
// -------------------------------------------------------------------------


/**  ERBeamDetTargetPoint.h
 **/


#ifndef ERBeamDetTargetPoint_H
#define ERBeamDetTargetPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"

class ERBeamDetTargetPoint : public FairMCPoint
{

 public:

  /** Default constructor **/
  ERBeamDetTargetPoint();

  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param pid particle ID
   *@param posIn    Ccoordinates at entrance of point [cm]
   *@param posOut   Coordinates at exit of point [cm]
   *@param momIn    Momentum of track at entrance [MeV]
   *@param momOut   Momentum of track at exit [MeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [MeV]
   **/
  ERBeamDetTargetPoint(Int_t eventID, 
                  Int_t trackID,
                  Int_t mot0trackID,
                  Int_t pid,
                  TVector3 posIn,
                  TVector3 posOut, TVector3 momIn, TVector3 momOut,
                  Double_t tof, Double_t length, Double_t eLoss, Double_t lightYield);

  /** Destructor **/
  virtual ~ERBeamDetTargetPoint();

  ERBeamDetTargetPoint& operator=(const ERBeamDetTargetPoint&) { return *this; }

  /** Accessors **/
  Int_t    GetEventID()     const { return fEventID; }
  Int_t    GetMot0TrackID() const { return fMot0TrackID; }
  Double_t GetXIn() const { return fX; }
  Double_t GetYIn() const { return fY; }
  Double_t GetZIn() const { return fZ; }
  Double_t GetXOut() const { return fX_out; }
  Double_t GetYOut() const { return fY_out; }
  Double_t GetZOut() const { return fZ_out; }
  Double_t GetPxIn() const { return fPx_in; }
  Double_t GetPyIn() const { return fPy_in; }
  Double_t GetPzIn() const { return fPz_in; }
  Double_t GetPxOut() const { return fPx_out; }
  Double_t GetPyOut() const { return fPy_out; }
  Double_t GetPzOut() const { return fPz_out; }
  Int_t    GetPID()  const { return fPid; }

  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }

  Float_t LightYield() const {return fLightYield;}

  /** Point coordinates at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;

  /** Check for distance between in and out **/
  Bool_t IsUsable() const;

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

 protected:
  Int_t       fEventID;
  Int_t       fMot0TrackID;
  Int_t       fPid;
  Double32_t  fX_out,  fY_out,  fZ_out;
  Double32_t  fX_in,  fY_in,  fZ_in;
  Double32_t  fPx_out, fPy_out, fPz_out;
  Double32_t  fPx_in, fPy_in, fPz_in;
  Double_t    fLightYield;

  ClassDef(ERBeamDetTargetPoint,1)
};

#endif
