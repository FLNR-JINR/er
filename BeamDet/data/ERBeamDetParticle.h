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
  Int_t          fPID = 0;
  float          fTimeOnTarget = -1.;
  float          fProbability = -1.;
public:
  ERBeamDetParticle() = default;
  ERBeamDetParticle(Int_t pid, TLorentzVector tofState, TLorentzVector targetState, 
                    float time_on_target, float probability);
  void AddParameters(Int_t pid, TLorentzVector tofState, TLorentzVector targetState, float probability);
  TLorentzVector GetLVTarget() const {return fTargetState;}
  TLorentzVector lv_between_tofs() const {return fTofState;}
  float time_on_target() const {return fTimeOnTarget;}
	ClassDef(ERBeamDetParticle, 1)
};

#endif
