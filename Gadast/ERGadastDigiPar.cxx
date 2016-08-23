#include "ERGadastDigiPar.h"

#include "FairParamList.h"
#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairGenericParAsciiFileIo.h"
#include "FairGenericParRootFileIo.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include<iostream>

#include "TString.h"
#include "TMath.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

// -----   Standard constructor   ------------------------------------------
ERGadastDigiPar::ERGadastDigiPar(const char* name,
    const char* title,
    const char* context)
  : FairParGenericSet(name, title, context),
    fMeshElementsCount(-1),
    fMeshElements(new TArrayF(64))
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ERGadastDigiPar::~ERGadastDigiPar()
{
  clear();
}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void ERGadastDigiPar::clear()
{
  status = kFALSE;
  resetInputVersions();
}
// -------------------------------------------------------------------------

// -----   Public method print ---------------------------------------
void ERGadastDigiPar::print()
{
  Int_t rowNofElements = (Int_t)TMath::Sqrt(fMeshElementsCount);
  
  std::cout << "*****************************************" << std::endl;
  std::cout << "          ERGadastDigiPar                " << std::endl;
  std::cout << "*****************************************" << std::endl;
  std::cout << "   ERGadastMeshElementsCount: " <<  fMeshElementsCount <<  std::endl;
  std::cout << "   ERGadastMeshElements: " <<  std::endl;
  for (Int_t iElement = 0; iElement < rowNofElements; iElement++){
    std::cout << "     ";
    for (Int_t jElement = 0; jElement < rowNofElements; jElement++)
      std::cout <<(*fMeshElements)[iElement*rowNofElements + jElement] << "\t";
     std::cout << std::endl;
  }
  std::cout << "*****************************************" << std::endl;
}
//------------------------------------------------------
void ERGadastDigiPar::putParams(FairParamList* l)
{ 
  cout << " I am in ERGadastDigiPar::putParams " << endl;
}
//------------------------------------------------------
Bool_t ERGadastDigiPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }
  
  if ( ! l->fill("ERGadastMeshElementsCount", &fMeshElementsCount) ) { return kFALSE; }
  
  fMeshElements->Set(fMeshElementsCount);
  if ( ! l->fill("ERGadastMeshElements", fMeshElements) ) { return kFALSE; }
  
  return kTRUE;
}
//------------------------------------------------------
Bool_t ERGadastDigiPar::init(FairParIo* input){
	std::cout << input->getFilename() << std::endl;
  if ( TString(input->getFilename()).Contains(".digi")){
    FairGenericParAsciiFileIo* p=new FairGenericParAsciiFileIo(((FairParAsciiFileIo*)input)->getFile());
	return p->init(this);
  }
  if ( TString(input->getFilename()).Contains(".root")){ 
    FairGenericParRootFileIo* p=new FairGenericParRootFileIo(((FairParRootFileIo*)input)->getParRootFile());
	return p->init(this);
  }
  return kFALSE;
}
//------------------------------------------------------

ClassImp(ERGadastDigiPar)
