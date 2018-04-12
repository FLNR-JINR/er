// -------------------------------------------------------------------------
// -----                      ERDecayMCEventHeader source file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------
#include "ERDecayMCEventHeader.h"


ERDecayMCEventHeader::ERDecayMCEventHeader(){

}

void ERDecayMCEventHeader::Clear(){
	fOutputsTracksNb = 0;
	fOutputTrackID.Set(fOutputsTracksNb);
	fOutputTrackID.Reset();
}

ClassImp(ERDecayMCEventHeader)