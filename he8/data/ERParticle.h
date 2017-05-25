// -------------------------------------------------------------------------
// -----                      ERParticle header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERParticle_H
#define ERParticle_H

#include "TNamed.h"
#include "TTree.h"
#include "TString.h"
#include "TLorentzVector.h"

class ERParticle: public TNamed {

public:
	ERParticle();
	virtual ~ERParticle();
	char* NameOfNucleus;
	double Mass;
	double Excitation;
	int AtNumber;
	int AtMass;
	void ParticleID(char* name, char* path);
	double ReturnMass(char* NON,char* WayMass);
	char* ReactionInput(char* WayReaction);
  	TLorentzVector Part;

	ClassDef(ERParticle,1);
};

#endif
