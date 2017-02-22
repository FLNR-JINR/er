
#ifndef ERTextDecay_H
#define ERTextDecay_H

#include <vector>

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

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

    Double_t fDecayPos;
  	Bool_t fDecayFinish;

  	FairIon* fInputIon;
  	FairIon* fOutputIon;
public:
	ERTextDecay(TString name);
	~ERTextDecay();

	Bool_t Stepping();
	Bool_t Init();

	void BeginEvent();
	void FinishEvent();

	void SetDecayPos(Double_t pos) {fDecayPos = pos;}
	void SetInputIon(Int_t A, Int_t Z, Int_t Q);
	void SetOutputIon(Int_t A, Int_t Z, Int_t Q);
	void AddOutputParticle(Int_t pdg);

	ClassDef(ERTextDecay,1)
};

#endif
