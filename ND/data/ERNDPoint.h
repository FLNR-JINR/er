// -------------------------------------------------------------------------
// -----                      ERNDPoint header file                -----
// -----                  Created data  developerName                  -----
// -------------------------------------------------------------------------


/**  ERNDPoint.h
 **/


#ifndef ERNDPoint_H
#define ERNDPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"


class ERNDPoint : public FairMCPoint 
{

 public:

  /** Default constructor **/
  ERNDPoint();


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
  ERNDPoint(
    Int_t eventID,
    Int_t trackID,
    Int_t mot0trackID,
    Int_t pdg,
    TVector3 posIn,
    TVector3 posOut,
    TVector3 momIn,
    TVector3 momOut,
    Double_t tof,
    Double_t length,
    Double_t eLoss,
    Int_t stilbenNr,
    Float_t lightYield,
    Int_t parentTrackId,
    Int_t parentPdgId
  );


  /** Copy constructor **/
  ERNDPoint(const ERNDPoint&);


  /** Destructor **/
  virtual ~ERNDPoint();


  ERNDPoint& operator=(const ERNDPoint&) { return *this; }


  /** Accessors **/
  Int_t GetParentTrackID()      const { return fParentTrackId; }
  Int_t GetParentPDGID()        const { return fParentPdgId; }
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
  Double_t GetPDG()             const { return fPdg; }
  Double_t GetMass()            const { return fMass; }

  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }

  Int_t StilbenNr() const {return fStilbenNr;}
  Float_t LightYield() const {return fLightYield;}

  /** Point coordinates at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;


  /** Check for distance between in and out **/
  Bool_t IsUsable() const;

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

 protected:
  
  Int_t fEventID;
  Int_t fMot0TrackID;
  Double_t fMass;
  Double32_t fX_out,  fY_out,  fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;
  Int_t fStilbenNr;
  Float_t fLightYield;
  Int_t fPdg;
  Int_t fParentTrackId = -1;                      ///< id of parrent particle track.
  Int_t fParentPdgId = -1;                        ///< pdg of parrent particle.
  
  ClassDef(ERNDPoint,1)
};
#endif
