#ifndef ERN15B11DETECTOR_H
#define ERN15B11DETECTOR_H

#include <FairDetector.h> // mother class

// STD
#include <map>

// ROOT
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TArrayI.h>

// Project
#include "ERMCTrack.h" // for ExpertTrackingStatus

class ERPoint;

struct stcNodeSig {
	Short_t iLayer;
	Short_t iDetContainer;
	Short_t iDetGasPart;
	Short_t iSemi;
};

class ERN15B11Detector : public FairDetector
{
public:
	ERN15B11Detector();
	ERN15B11Detector(const char* Name, Bool_t Active, Int_t DetId = 0);
	virtual ~ERN15B11Detector();

	virtual void ConstructGeometry();
	virtual Bool_t CheckIfSensitive(std::string name);

	virtual Bool_t ProcessHits(FairVolume* v = 0);
	virtual void EndOfEvent();
	virtual void Register();
	virtual TClonesArray* GetCollection(Int_t iColl) const;
	virtual void Reset();

	//void AddSensitiveVolumeName(std::string p_name);

private:

	/**
	 * Parse full node path
	 */
	Bool_t GetCurNodeSig(const char* nodePath) const;


	std::map<Int_t, TClonesArray*> mOutputPoints;


	void StartNewPoint(void);
	void FinishNewPoint(TClonesArray* p_collection);
	ERPoint* AddPoint(TClonesArray* p_collection);

	static struct stcNodeSig mCurNodeSig;
	static Int_t mCurNodeUID;


private:

	//TODO check!
    /* current step information*/
    Int_t fEventID;                   ///< event index
    Int_t fTrackID;                   ///< track index
    Int_t fMot0TrackID;               ///< 0th mother track index
    Double_t fMass;                   ///< mass
    TLorentzVector fPosIn;            ///< point start  position
    TLorentzVector fPosOut;           ///< point finish  position
    TLorentzVector fCurPosIn;         ///< current step position
    TVector3 fPosInLocal;             ///< point position in sensetive volume CS
    TLorentzVector fMomIn;            ///< point start momentum
    TLorentzVector fMomOut;           ///< point start momentum
    TLorentzVector fCurMomIn;         ///< current step momentum
    Double32_t     fTimeIn;           ///< point start time
    Double32_t     fTimeOut;          ///< point finish time
    Double32_t     fTrackLength;      ///< track length from his origin
    Double32_t     fELoss;            ///< energy loss
    Int_t          fVolNb;          ///< number of fiber in pixel
    ExpertTrackingStatus fTrackStatus; ///< curren track stutus (transport, stop, disappeared, ...)
    TArrayI  fProcessesID;            ///< VMC prcess IDs in step


	ClassDef(ERN15B11Detector, 1);
};

#endif // ERN15B11DETECTOR_H
