

#ifndef ERDETECTOR_CXX_
#define ERDETECTOR_CXX_

#include "FairDetector.h"

class ERDetector : public FairDetector
{

public:
    /**
      constructor
      Name :  Detector Name
      Active: kTRUE for active detectors  (ProcessHits() will be called)
              kFALSE for inactive detectors
    */
	ERDetector(const char* Name, Bool_t Active, Int_t DetId=0);

	/** Virtaul method Construct geometry
	 **
	 ** Constructs the ERNeuRad geometry
	**/
	virtual void ConstructGeometry();
};





#endif /* ERDETECTOR_CXX_ */

