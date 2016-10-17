// -------------------------------------------------------------------------
// -----                      ERMCEventHeader header file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

/** ERMCEventHeader.h
 *@author V.Schetinin <sch_vitaliy@mail.ru>
 **
 ** Data class for storing specific information about event
 **/

#ifndef ERMCEVENTHEADER_H_
#define ERMCEVENTHEADER_H_

#include "FairMCEventHeader.h"

class ERMCEventHeader : public FairMCEventHeader {
private:
	Double_t fTargetReactionPos;
	Double_t fDirectReactionPos;
	Float_t fNeuRadELoss;
	Float_t fNeuRadLY;
public:
	ERMCEventHeader();
	void SetTargetReactionPos(Double_t pos){fTargetReactionPos = pos;}
	void SetDirectReactionPos(Double_t pos){fDirectReactionPos = pos;}
	void SetNeuRadEloss(Float_t eloss) {fNeuRadELoss = eloss;}
  	void SetNeuRadLY(Float_t ly)       {fNeuRadLY = ly;}

	Double_t TargetReactionPos()const {return fTargetReactionPos;}
	Double_t DirectReactionPos()const {return fDirectReactionPos;}
	Float_t NeuRadEloss()const {return fNeuRadELoss;}
	Float_t NeuRadLY()const {return fNeuRadLY;}

	ClassDef(ERMCEventHeader,1)
};

#endif