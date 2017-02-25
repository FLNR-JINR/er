// -------------------------------------------------------------------------
// -----                      ERDecayMCEventHeader source file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------
#include "ERDecayMCEventHeader.h"


ERDecayMCEventHeader::ERDecayMCEventHeader():
fOutputsParticles(new TClonesArray("TLorentzVector", 10))
{

}

ClassImp(ERDecayMCEventHeader)