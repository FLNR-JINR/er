#include "ERRecoMCApplication.h"

#include<iostream>

#include <iostream>
using namespace std;

ERRecoMCApplication::ERRecoMCApplication(const char* name, const char* title, TObjArray* ModList, const char* MatName):
	FairMCApplication(name, title,ModList,MatName)
{

}

/** default constructor
*/
ERRecoMCApplication::ERRecoMCApplication(){

}
/** default destructor
*/
ERRecoMCApplication::~ERRecoMCApplication(){

}

ClassImp(ERRecoMCApplication)