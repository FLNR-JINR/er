// -------------------------------------------------------------------------
// -----                  ERGadastDigiPar header file                 -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERGadastDigiPar_H
#define ERGadastDigiPar_H


#include "FairParGenericSet.h"

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayF.h"
#include "TArrayI.h"

class FairParIo;
class FairParamList;


class ERGadastDigiPar : public FairParGenericSet
{

  public:

    /** Standard constructor **/
    ERGadastDigiPar(const char* name    = "ERGadastDigiPar",
                            const char* title   = "ERGadast Digitization Parameters",
                            const char* context = "Default");


    /** Destructor **/
    virtual ~ERGadastDigiPar();


    /** Initialisation from input device**/
    virtual Bool_t init(FairParIo* input);


    /** Output to file **/
    //virtual Int_t write(FairParIo* output);

    virtual void print();
    
    /** Reset all parameters **/
    virtual void clear();

    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    
    /** Accessories  **/
    Int_t CsIMECout() const {return fCsIMECount;}
    Int_t LaBrMECout() const {return fLaBrMECount;}
    Float_t CsILC(Int_t me) const {return (*fCsILC)[me];}
    Float_t LaBrLC(Int_t me) const {return (*fLaBrLC)[me];}
    Float_t CsIDispA(Int_t me) const {return (*fCsIDispA)[me];}
    Float_t LaBrDispA(Int_t me) const {return (*fLaBrDispA)[me];}
    Float_t CsIDispB(Int_t me) const {return (*fCsIDispB)[me];}
    Float_t LaBrDispB(Int_t me) const {return (*fLaBrDispB)[me];}
  private:
    //количество элементов в сетке
    Int_t fCsIMECount; 
    Int_t fLaBrMECount;

    TArrayF* fCsILC;
    TArrayF* fLaBrLC;

    TArrayF* fCsIDispA;
    TArrayF* fLaBrDispA;

    TArrayF* fCsIDispB;
    TArrayF* fLaBrDispB;

    ERGadastDigiPar(const ERGadastDigiPar&);
    ERGadastDigiPar& operator=(const ERGadastDigiPar&);

    ClassDef(ERGadastDigiPar,1);
};


#endif /**  ERGadastDigiPar_H   **/
