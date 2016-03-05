// -------------------------------------------------------------------------
// -----                      ERGadastPoint header file                -----
// -----                  Created data  Vitaliy Schetinin              -----
// -------------------------------------------------------------------------



#ifndef ERGadastPoint_H
#define ERGadastPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"

enum ERGadastPointType{LaBr, CsI};

class ERGadastPoint : public FairMCPoint 
{

 public:
 
  /** Default constructor **/
  ERGadastPoint();


  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param posIn    Ccoordinates at entrance of point [cm]
   *@param posOut   Coordinates at exit of point [cm]
   *@param momIn    Momentum of track at entrance [GeV]
   *@param momOut   Momentum of track at exit [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [KeV]
   **/
  ERGadastPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss,
      ERGadastPointType type, Int_t pdg);


  /** Copy constructor **/
  ERGadastPoint(const ERGadastPoint&);


  /** Destructor **/
  virtual ~ERGadastPoint();


  ERGadastPoint& operator=(const ERGadastPoint&) { return *this; }


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
  ERGadastPointType GetType()   const { return fType; }

  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }


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
  ERGadastPointType fType;
  Int_t fPDG;

  ClassDef(ERGadastPoint,1)
};
#endif
