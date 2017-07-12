
#ifndef ERTextDecay_H
#define ERTextDecay_H

#include <vector>

#include "TRandom3.h"
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "TString.h"

#include "FairIon.h"

#include "ERDecay.h"

class ERTextDecay : public ERDecay{
private:
	TString fInputIonName;
	TString fOutputIonName;
	TRandom3 *fRnd;

	TParticlePDG*   fInputIonPDG;
	TParticlePDG*   fOutputIonPDG;
  std::vector<TParticlePDG*> fOutputParticlesPDG;

  Double_t fDecayPosZ;
  TLorentzVector fDecayPos;
  Bool_t fDecayFinish;

  FairIon* fInputIon;
  FairIon* fOutputIon;
  TLorentzVector fInputIonV;
  
  TString fFileName;
  
  Bool_t ReadFile();
  void SaveToEventHeader();
  
  std::vector<std::vector<TLorentzVector> > fDecays;
  Int_t fNOutputs;
  
  Bool_t fUniform;
  Float_t fUniformA;
  Float_t fUniformB;
  
  Bool_t fExponential;
  Float_t fExponentialStart;
  Float_t fExponentialTau;

  TString fVolumeName;
public:
	ERTextDecay(TString name);
	~ERTextDecay();

	Bool_t Stepping();
	Bool_t Init();

	void BeginEvent();
	void FinishEvent();

	void SetDecayPosZ(Double_t pos) {fDecayPosZ = pos;}
	void SetInputIon(Int_t A, Int_t Z, Int_t Q);
	void SetOutputIon(Int_t A, Int_t Z, Int_t Q);
	void AddOutputParticle(Int_t pdg);
  void SetFileName(TString name){fFileName = name;}
  void SetUniformPos(Double_t a, Double_t b);
  void SetExponentialPos(Double_t start, Double_t tau);
  void SetDecayVolume(TString name){fVolumeName = name;}
	ClassDef(ERTextDecay,1)
};

#endif
