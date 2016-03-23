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

#include "TVector3.h"

#include "FairMCEventHeader.h"

class ERMCEventHeader : public FairMCEventHeader {
private:
	TVector3 fTargetReactionPos;
	TVector3 fDirectReactionPos;
public:
	ERMCEventHeader();
};

#endif