// -------------------------------------------------------------------------
// -----                        ERBeamDetParticle header file                -----
// -----                     Created   by              -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetParticle_H
#define ERBeamDetParticle_H

#include "TString.h"
#include "TNamed.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class ERBeamDetParticle : public TNamed{
private:
  TLorentzVector fFourMomentum;
  Int_t          fPID;
  Double_t       fProbability;
public:
  ERBeamDetParticle();
  ERBeamDetParticle(Int_t pid, TLorentzVector fourMomentum, Double_t probability);

  void AddParameters(Int_t pid, TLorentzVector fourMomentum, Double_t probability);

	ClassDef(ERBeamDetParticle, 1)
};

#endif
