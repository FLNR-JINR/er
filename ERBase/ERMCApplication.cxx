#include "ERMCApplication.h"

#include "FairLogger.h"

#include <iostream>
using namespace std;

ERMCApplication::ERMCApplication(const char* name, const char* title, TObjArray* ModList, const char* MatName):
	FairMCApplication(name, title,ModList,MatName),
	fDecayer(NULL)
{

}

/** default constructor
*/
ERMCApplication::ERMCApplication(){

}
/** default destructor
*/
ERMCApplication::~ERMCApplication(){

}

/** Define action at each step, dispatch the action to the corresponding detectors */
void ERMCApplication::Stepping(){
	FairMCApplication::Stepping();
	if (fDecayer){
		if (!fDecayer->Stepping())
			LOG(ERROR) << "Fatal problem in ER decayer!" << FairLogger::endl;
	}
}

/** Define actions at the beginning of the event */
void ERMCApplication::BeginEvent(){                                     // MC Application
	FairMCApplication::BeginEvent();
	if (fDecayer)
		fDecayer->BeginEvent();
}
    /** Define actions at the end of event */
void ERMCApplication::FinishEvent(){                                   // MC Application
	FairMCApplication::FinishEvent();
	if(fDecayer)
		fDecayer->FinishEvent();
}

ClassImp(ERMCApplication)