
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
  std::vector<FairIon*> fOutputIons;
  TLorentzVector fInputIonV;
  TString fOutputIonName;
  
  TString fFileName;
  
  Bool_t ReadFile();
  void SaveToEventHeader();
  
  std::vector<std::vector<TLorentzVector> > fDecays;
  Int_t fNOutputs;

  //TParticlePDG*   fOutputIonPDG;
  std::vector<TParticlePDG*> fOutputParticlesPDG;
  std::vector<TNamed*> fOutputs;
public:
	ERTextDecay(TString name);
	~ERTextDecay();

	Bool_t Stepping();
	Bool_t Init();

  void AddOutputIon(Int_t A, Int_t Z, Int_t Q);
  void AddOutputParticle(Int_t pdg);
  void SetFileName(TString name){fFileName = name;}

	ClassDef(ERTextDecay,1)
};

#endif
