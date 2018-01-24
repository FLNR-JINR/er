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
    fCsIMECount(-1), 
    fLaBrMECount(-1),
    fCsILC(new TArrayF(64)),
    fLaBrLC(new TArrayF(4)),
    fCsIDispA(new TArrayF(64)),
    fLaBrDispA(new TArrayF(4)),
    fCsIDispB(new TArrayF(64)),
    fLaBrDispB(new TArrayF(4))
{
}
//-------------------------------------------------------------------------



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
  std::cout << "*****************************************" << std::endl;
  std::cout << "          ERGadastDigiPar                " << std::endl;
  std::cout << "*****************************************" << std::endl;
  std::cout << "   ERGadastCsIMECount: " <<  fCsIMECount <<  std::endl;
  std::cout << "   ERGadastLaBrMECount: " <<  fLaBrMECount <<  std::endl;
  std::cout << "*****************************************" << std::endl;
  std::cout << "   ERGadastCsILC: " <<  std::endl;
  for (Int_t iElement = 0; iElement < 16; iElement++){
    std::cout << "     ";
    for (Int_t jElement = 0; jElement < 4; jElement++)
      std::cout <<(*fCsILC)[iElement*4 + jElement] << "\t";
     std::cout << std::endl;
  }
  std::cout << "*****************************************" << std::endl;
  std::cout << "   ERGadastCsIDispA: " <<  std::endl;
  for (Int_t iElement = 0; iElement < 16; iElement++){
    std::cout << "     ";
    for (Int_t jElement = 0; jElement < 4; jElement++)
      std::cout <<(*fCsIDispA)[iElement*4 + jElement] << "\t";
     std::cout << std::endl;
  }
  std::cout << "*****************************************" << std::endl;
  std::cout << "   ERGadastCsIDispB: " <<  std::endl;
  for (Int_t iElement = 0; iElement < 16; iElement++){
    std::cout << "     ";
    for (Int_t jElement = 0; jElement < 4; jElement++)
      std::cout <<(*fCsIDispB)[iElement*4 + jElement] << "\t";
     std::cout << std::endl;
  }
  std::cout << "*****************************************" << std::endl;
  std::cout << "   ERGadastLaBrLC: " <<  std::endl;
  for (Int_t iElement = 0; iElement < 4; iElement++){
    std::cout <<(*fLaBrLC)[iElement] << "\t";
  }
  std::cout << std::endl;
  std::cout << "*****************************************" << std::endl;
  std::cout << "   ERGadastLaBrDispA: " <<  std::endl;
  for (Int_t iElement = 0; iElement < 4; iElement++){
    std::cout <<(*fLaBrDispA)[iElement] << "\t";
  }
  std::cout << std::endl;
  std::cout << "*****************************************" << std::endl;
  std::cout << "   ERGadastLaBrDispB: " <<  std::endl;
  for (Int_t iElement = 0; iElement < 4; iElement++){
    std::cout <<(*fLaBrDispB)[iElement] << "\t";
  }
  std::cout << std::endl;
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
  
  if ( ! l->fill("ERGadastCsIMECount", &fCsIMECount) ) { return kFALSE; }
  if ( ! l->fill("ERGadastLaBrMECount", &fLaBrMECount) ) { return kFALSE; }
 
  fCsILC->Set(fCsIMECount);
  if ( ! l->fill("ERGadastCsILC", fCsILC) ) { return kFALSE; }

  fLaBrLC->Set(fLaBrMECount);
  if ( ! l->fill("ERGadastLaBrLC", fLaBrLC) ) { return kFALSE; }

  fCsIDispA->Set(fCsIMECount);
  if ( ! l->fill("ERGadastCsIDispA", fCsIDispA) ) { return kFALSE; }

  fLaBrDispA->Set(fLaBrMECount);
  if ( ! l->fill("ERGadastLaBrDispA", fLaBrDispA) ) { return kFALSE; }

  fCsIDispB->Set(fCsIMECount);
  if ( ! l->fill("ERGadastCsIDispB", fCsIDispB) ) { return kFALSE; }
  
  fLaBrDispB->Set(fLaBrMECount);
  if ( ! l->fill("ERGadastLaBrDispB", fLaBrDispB) ) { return kFALSE; }

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
