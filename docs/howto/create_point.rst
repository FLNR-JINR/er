Типовой класс поинт выглядит следующим образом:

ERDetPoint.h:

:: 

	// -------------------------------------------------------------------------
	// -----                      ERDetPoint header file                -----
	// -----                  Created data  developerName                  -----
	// -------------------------------------------------------------------------


	/**  ERDetPoint.h
	 **/


	#ifndef ERDetPoint_H
	#define ERDetPoint_H


	#include "TObject.h"
	#include "TVector3.h"

	#include "FairMCPoint.h"


	class ERDetPoint : public FairMCPoint 
	{

	 public:

	  /** Default constructor **/
	  ERDetPoint();


	  /** Constructor with arguments
	   *@param EventID  Index of Event
	   *@param trackID  Index of MCTrack
	   *@param mot0trackID Index of Mother MCTrack
	   *@param pid particle ID
	   *@param posIn    Ccoordinates at entrance of point [cm]
	   *@param posOut   Coordinates at exit of point [cm]
	   *@param momIn    Momentum of track at entrance [GeV]
	   *@param momOut   Momentum of track at exit [GeV]
	   *@param tof      Time since event start [ns]
	   *@param length   Track length since creation [cm]
	   *@param eLoss    Energy deposit [KeV]
	   **/
	  ERDetPoint(Int_t eventID, Int_t trackID,
			  Int_t mot0trackID,
			  Int_t pid,
			  TVector3 posIn,
			  TVector3 posOut, TVector3 momIn, TVector3 momOut,
			  Double_t tof, Double_t length, Double_t eLoss);


	  /** Copy constructor **/
	  ERDetPoint(const ERDetPoint&);


	  /** Destructor **/
	  virtual ~ERDetPoint();


	  ERDetPoint& operator=(const ERDetPoint&) { return *this; }


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
	  Int_t GetPID()            const { return fPid; }

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
	  Int_t fPid;
	  Double32_t fX_out,  fY_out,  fZ_out;
	  Double32_t fPx_out, fPy_out, fPz_out;
	  Int_t fStilbenNr;
	  Float_t fLightYield;

	  ClassDef(ERDetPoint,1)
	};
	#endif

ERDetPoint.cxx:

::

	// -------------------------------------------------------------------------
	// -----                      ERDetPoint source file                   -----
	// -------------------------------------------------------------------------

	#include "ERDetPoint.h"
	#include <iostream>
	using namespace std;
	// -----   Default constructor   -------------------------------------------
	ERDetPoint::ERDetPoint()
	  : FairMCPoint(),
	    fX_out(0.), fY_out(0.), fZ_out(0.),
	    fPx_out(0.), fPy_out(0.), fPz_out(0.),
	    fStilbenNr(-1)
	{
	}
	// -------------------------------------------------------------------------
	ERDetPoint::ERDetPoint(Int_t eventID, Int_t trackID,
			  Int_t mot0trackID,
			  Int_t pid,
			  TVector3 posIn,
			  TVector3 posOut, TVector3 momIn, TVector3 momOut,
			  Double_t tof, Double_t length, Double_t eLoss)
	  : FairMCPoint(trackID, -1., posIn, momIn, tof, length, eLoss),
	    fEventID(eventID),
	    fPid(pid),
	    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
	    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z())
	{
	}
	// -------------------------------------------------------------------------
	ERDetPoint::ERDetPoint(const ERDetPoint& right)
	  : FairMCPoint(right),
	    fPid(right.fPid),
	    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
	    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out)
	{
	}
	// -------------------------------------------------------------------------
	ERDetPoint::~ERDetPoint()
	{
	}
	// -------------------------------------------------------------------------
	void ERDetPoint::Print(const Option_t* opt /* = 0*/) const
	{
	  cout << "-I- ERDetPoint: track " << fTrackID << " mother track = " << fMot0TrackID << endl;
	  cout << "    particle ID " << fPid << endl;
	  cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
	  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
	  cout << "    Time " << fTime << " ns,  Length " << fLength << " cm" << endl;
	  cout << "    Energy loss " << fELoss << " keV "<< endl;
	}
	// -------------------------------------------------------------------------
	// -----   Point x coordinate from linear extrapolation   ------------------
	Double_t ERDetPoint::GetX(Double_t z) const
	{
	  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
	  Double_t dz = fZ_out - fZ;
	  return ( fX + (z-fZ) / dz * (fX_out-fX) );
	}
	// -------------------------------------------------------------------------

	// -----   Point y coordinate from linear extrapolation   ------------------
	Double_t ERDetPoint::GetY(Double_t z) const
	{
	  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
	  Double_t dz = fZ_out - fZ;
	  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
	  return ( fY + (z-fZ) / dz * (fY_out-fY) );
	}
	// -------------------------------------------------------------------------
	// -----   Public method IsUsable   ----------------------------------------
	Bool_t ERDetPoint::IsUsable() const
	{
	  Double_t dz = fZ_out - fZ;
	  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
	  return kTRUE;
	}
	// -------------------------------------------------------------------------
	ClassImp(ERDetPoint)