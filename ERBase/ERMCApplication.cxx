#include "ERMCApplication.h"

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
	if (fDecayer)
		fDecayer->Stepping();
}


ClassImp(ERMCApplication)