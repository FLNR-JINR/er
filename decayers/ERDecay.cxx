#include "ERDecay.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"

#include "FairRunSim.h"
#include<iostream>

using namespace std;

//#include "ERTarget.h"
#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERDecay::ERDecay(TString name):
fName(name)
{
}

ERDecay::~ERDecay(){

}

ClassImp(ERDecay)
