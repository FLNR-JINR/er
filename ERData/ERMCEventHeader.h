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
public:
	ERMCEventHeader();
	void SetTargetReactionPos(Double_t pos){fTargetReactionPos = pos;}
	void SetDirectReactionPos(Double_t pos){fDirectReactionPos = pos;}
	Double_t TargetReactionPos(){return fTargetReactionPos;}
};

#endif