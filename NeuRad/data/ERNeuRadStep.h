// -------------------------------------------------------------------------
// -----                      ERNeuRadStep header file             -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------

/**  ERNeuRadStep.h
 **/

#ifndef ERNeuRadStep_H
#define ERNeuRadStep_H

#include "TObject.h"
#include "TVector3.h"
#include "TArrayI.h"

#include "FairMultiLinkedData.h"

#include "ERMCTrack.h"

class ERNeuRadStep : public FairMultiLinkedData
{

public:

  /** Default constructor **/
  ERNeuRadStep();

  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param fiberNb number of fiber in module
   *@param pos    Coordinates at entrance of point [cm]
   *@param mom    Momentum of track first step[GeV]
   *@param tof      Time since event start [ns]
   *@param length Track length since creation [cm]
   *@param pid   
   **/
  ERNeuRadStep(Int_t eventID, Int_t stepNr,Int_t trackID,
		  Int_t mot0trackID,
      Int_t fiberNb,
      Int_t pixelNb,
      Int_t moduleNb,
		  TVector3 pos, 
      TVector3 mom, 
		  Double_t tof, 
      Double_t length, 
      Int_t pid,
      Double_t mass,
      ExpertTrackingStatus trackStatus,
      Double_t eLoss,
      Double_t charge,
      TArrayI  processID);

  /** Copy constructor **/
  ERNeuRadStep(const ERNeuRadStep&);

  /** Destructor **/
  virtual ~ERNeuRadStep();
  
  ERNeuRadStep& operator=(const ERNeuRadStep&) { return *this; }
  
  /** Accessors **/
  Int_t GetFiberNb() const {return fFiberNb;}
  Int_t GetPixelNb() const {return fPixelNb;}
  Int_t GetModuleNb() const {return fModuleNb;}
  Double_t GetX() const {return fX;}
  Double_t GetY() const {return fY;}
  Double_t GetZ() const {return fZ;}
  void Print();
  
  static ExpertTrackingStatus GetTrackStatus();

protected:

  Int_t fEventID;
  Int_t fStepNr;
  Int_t fTrackID;
  Int_t fMot0TrackID;
  Int_t fFiberNb;
  Int_t fPixelNb;
  Int_t fModuleNb;
  Double32_t fX,  fY,  fZ;
  Double32_t fPx, fPy, fPz;
  Double32_t fTOF;
  Double32_t fLength;
  Int_t fPID;
  Double_t fMass;
  ExpertTrackingStatus fTrackStatus;
  Double_t fEloss;
  Double_t fCharge;
  TArrayI fProcessID;
  

  ClassDef(ERNeuRadStep,1)
};

#endif //ERNeuRadStep_H
