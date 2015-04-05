// -------------------------------------------------------------------------
// -----                      ExpertNeuRadStep header file             -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


/**  ExpertNeuRadStep.h
 **/

#ifndef ExpertNeuRadStep_H
#define ExpertNeuRadStep_H


#include "TObject.h"
#include "TVector3.h"
#include "TArrayI.h"

enum ExpertTrackingStatus{
    Entering,
    Exiting,
    Inside,
    Stop
};

class ExpertNeuRadStep : public TObject 
{
 public:

  /** Default constructor **/
  ExpertNeuRadStep();

  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param fiberInBundleNb number of fiber in bundle
   *@param pos    Coordinates at entrance of point [cm]
   *@param mom    Momentum of track first step[GeV]
   *@param tof      Time since event start [ns]
   *@param length Track length since creation [cm]
   *@param pid   
   **/
  ExpertNeuRadStep(Int_t eventID, Int_t stepNr,Int_t trackID,
		  Int_t mot0trackID,
      Int_t fiberInBundleNb,
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
  ExpertNeuRadStep(const ExpertNeuRadStep&);


  /** Destructor **/
  virtual ~ExpertNeuRadStep();
  
  ExpertNeuRadStep& operator=(const ExpertNeuRadStep&) { return *this; }
  
  /** Accessors **/
  Double_t GetFiberInBundleNb() const {return fFiberInBundleNb;}
  
  void Print();
  
  static ExpertTrackingStatus GetTrackStatus();
  
 protected:
  
  Int_t fEventID;
  Int_t fStepNr;
  Int_t fTrackID;
  Int_t fMot0TrackID;
  Int_t fFiberInBundleNb;
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
  

  ClassDef(ExpertNeuRadStep,1)
};
#endif
