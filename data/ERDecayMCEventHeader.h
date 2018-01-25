// -------------------------------------------------------------------------
// -----                      ERDecayMCEventHeader header file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

/** ERDecayMCEventHeader.h
 *@author V.Schetinin <sch_vitaliy@mail.ru>
 **
 ** Data class for storing specific information about event
 **/

#ifndef ERDecayMCEventHeader_H_
#define ERDecayMCEventHeader_H_

#include "TLorentzVector.h"
#include "TClonesArray.h"

#include "FairMCEventHeader.h"

class ERDecayMCEventHeader : public FairMCEventHeader {
private:
	TVector3 fDecayPos;
	TLorentzVector fInputIon;
  TClonesArray* fOutputsParticles;
public:
	ERDecayMCEventHeader();
	void SetDecayPos(TVector3 pos){fDecayPos = pos;}
  void SetInputIon(TLorentzVector iion){fInputIon = iion;}
  void AddOutputParticle(TLorentzVector oparticle){
          new((*fOutputsParticles)[fOutputsParticles->GetEntriesFast()])TLorentzVector(oparticle);}
  
	ClassDef(ERDecayMCEventHeader,1)
};

#endif