// -------------------------------------------------------------------------
// -----                      ERDecay header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAYER_H
#define ERDECAYER_H

#include "TRandom3.h"
#include "TString.h"
#include "TGenPhaseSpace.h"
#include "TDatabasePDG.h"               //for TDatabasePDG

class ERDecay{
protected:
	TString fName;

	TRandom3 *fRnd;

	Bool_t fUniform;
	Float_t fUniformA;
	Float_t fUniformB;

	Bool_t fExponential;
	Float_t fExponentialStart;
	Float_t fExponentialTau;

	TString fVolumeName;
	Double_t fStep;

	Double_t fTargetMass;

	TString fInputIonName;
	TParticlePDG*   fInputIonPDG;

	Double_t fDecayPosZ;
	TLorentzVector fDecayPos;
	Bool_t fDecayFinish;

	void AddParticleToStack(Int_t pdg, TLorentzVector pos, TLorentzVector state);

public:
	ERDecay(TString name);
	~ERDecay();

	void SetDecayPosZ(Double_t pos) {fDecayPosZ = pos;}
	void SetInputIon(Int_t A, Int_t Z, Int_t Q);
	void SetUniformPos(Double_t a, Double_t b);
	void SetExponentialPos(Double_t start, Double_t tau);
	void SetDecayVolume(TString name){fVolumeName = name;}
	void SetStep(Double_t step){fStep = step;}
	void SetTargetMass(Double_t targetMass){fTargetMass = targetMass;}

	virtual Bool_t Stepping() = 0;

	virtual void BeginEvent();
	virtual void FinishEvent();
	virtual Bool_t Init();

	TString GetName() {return fName;}
	ClassDef(ERDecay,1)
};

#endif
