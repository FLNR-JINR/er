/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNeuRadPoint_H
#define ERNeuRadPoint_H

#include "FairMCPoint.h" // mother class

#include "TVector3.h"

/** @class ERNeuRadPoint
 ** @brief The data class for storing pieces of charged tracks in sensitive volumes in NeuRad
 ** @author V.Schetinin <sch_vitaliy@mail.ru>
 ** @version 1.0
**/
class ERNeuRadPoint : public FairMCPoint
{
public:

  /** Default constructor **/
  ERNeuRadPoint();

  /** Constructor with arguments
   *@param EventID Index of Event
   *@param trackID Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param fiberInModuleNb Number of fiber in module
   *@param posIn Coordinates at entrance of point [cm]
   *@param posOut Coordinates at exit of point [cm]
   *@param momIn Momentum of track at entrance [GeV]
   *@param momOut Momentum of track at exit [GeV]
   *@param tof Time since event start [ns]
   *@param length Track length since creation [cm]
   *@param eLoss Energy deposit [KeV]
   *@param lightYield Energy deposit [MeV]
   **/
  ERNeuRadPoint(Int_t eventID,
                Int_t trackID,
                Int_t mot0trackID,
                Int_t fiberNb,
                Int_t pixelNb,
                Int_t moduleNb,
                Double_t mass,
                TVector3 posIn,
                TVector3 posInLoc,
                TVector3 posOut,
                TVector3 momIn,
                TVector3 momOut,
                Double_t timeIn,
                Double_t timeOut,
                Double_t trackLength,
                Double_t eLoss,
                Double_t lightYield,
                Int_t pid,
                Double_t charge);

  /** Copy constructor **/
  ERNeuRadPoint(const ERNeuRadPoint&);

  /** Destructor **/
  virtual ~ERNeuRadPoint();

  ERNeuRadPoint& operator=(const ERNeuRadPoint&) { return *this; }

  /** Accessors **/
  Int_t GetEventID() const { return fEventID; }
  Int_t GetMot0TrackID()   const { return fMot0TrackID; }
  Double_t GetXIn()   const { return fX; }
  Double_t GetYIn()   const { return fY; }
  Double_t GetZIn()   const { return fZ; }
  Double_t GetXInLocal()   const { return fXlocal; }
  Double_t GetYInLocal()   const { return fYlocal; }
  Double_t GetZInLocal()   const { return fZlocal; }
  Double_t GetXOut()  const { return fX_out; }
  Double_t GetYOut()  const { return fY_out; }
  Double_t GetZOut()  const { return fZ_out; }
  Double_t GetPxIn()  const { return fPx; }
  Double_t GetPyIn()  const { return fPy; }
  Double_t GetPzIn()  const { return fPz; }
  Double_t GetPxOut() const { return fPx_out; }
  Double_t GetPyOut() const { return fPy_out; }
  Double_t GetPzOut() const { return fPz_out; }
  Double_t GetPIn()   const;
  Double_t GetPOut()  const;
  Double_t GetMass()  const { return fMass; }
  Double_t GetLightYield() const {return fLightYield;}
  Int_t GetModuleNb() {return fModuleNb;}
  Int_t GetPixelNb() {return fPixelNb;}
  Int_t GetFiberNb() const {return fFiberNb;}
  Int_t GetPID() const {return fPID;}
  Double_t GetCharge() const {return fCharge;}
  Double_t GetTime() const {return fTimeIn;}
  Double_t GetTimeIn() const {return fTimeIn;}
  Double_t GetTimeOut() const {return fTimeOut;}
  Double_t GetLength() const;

  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }

  /** Point coordinates at given z from linear extrapolation **/
  //@todo проверить эти функции
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;
  Double_t GetP(Double_t pointLen) const;
  Double_t GetTime(Double_t pointLen) const;

  /** Check for distance between in and out **/
  Bool_t IsUsable() const;

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /** Modifies **/
  void SetLightYield(Double_t ly) {fLightYield = ly;}
  void SetTimeIn(Double_t t) {fTimeIn = t;}
  void SetXIn(Double_t xin) {fX = xin;}
  void SetYIn(Double_t yin) {fY = yin;}
  void SetZIn(Double_t zin) {fZ = zin;}
  void SetXOut(Double_t xout) {fX_out = xout;}
  void SetYOut(Double_t yout) {fY_out = yout;}
  void SetZOut(Double_t zout) {fZ_out = zout;}

protected:

  Int_t fEventID;
  Int_t fMot0TrackID;
  Int_t fFiberNb;
  Int_t fPixelNb;
  Int_t fModuleNb;
  Double_t fMass;
  Double32_t fXlocal, fYlocal, fZlocal;
  Double32_t fX_out,  fY_out,  fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;
  Double_t fTimeIn;
  Double_t fTimeOut;
  Double_t fLightYield;
  Int_t fPID;
  Double_t fCharge;
  Double_t fTrackLength;

  ClassDef(ERNeuRadPoint, 1);
};

#endif // ERNeuRadPoint_H
