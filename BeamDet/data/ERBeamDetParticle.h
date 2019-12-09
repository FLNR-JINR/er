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
  TLorentzVector fTofState;
  TLorentzVector fTargetState;
  Int_t          fPID;
  Double_t		   fPx;
  Double_t		   fPy;
  Double_t       fPz;
  Double_t       fP;
  Double_t       fProbability;
public:
  ERBeamDetParticle();
  ERBeamDetParticle(Int_t pid, TLorentzVector tofState, TLorentzVector targetState, Double_t probability);

  void AddParameters(Int_t pid, TLorentzVector tofState, TLorentzVector targetState, Double_t probability);
  TLorentzVector GetLVTarget() const {return fTargetState;}

	ClassDef(ERBeamDetParticle, 1)
};

#endif
