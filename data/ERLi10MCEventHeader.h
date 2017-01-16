// -------------------------------------------------------------------------
// -----                      ERLi10MCEventHeader header file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

/** ERLi10MCEventHeader.h
 *@author V.Schetinin <sch_vitaliy@mail.ru>
 **
 ** Data class for storing specific information about event
 **/

#ifndef ERLi10MCEventHeader_H
#define ERLi10MCEventHeader_H

#include "FairMCEventHeader.h"

class ERLi10MCEventHeader : public FairMCEventHeader {
private:
	Float_t fReactionPos;
	Float_t fReactionTime;
public:
	ERLi10MCEventHeader();
	void SetReactionPos(Double_t pos){fReactionPos = pos;}
	void SetReactionTime(Double_t time) {fReactionTime = time;}

	Float_t ReactionPos() const {return fReactionPos;}
	Float_t ReactionTime() const {return fReactionTime;}

	ClassDef(ERLi10MCEventHeader,1)
};

#endif
