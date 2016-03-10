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
    Int_t MeshElementsCount(){return fMeshElementsCount;}
  private:
    Int_t fMeshElementsCount;
    TArrayF* fMeshElements;

    ERGadastDigiPar(const ERGadastDigiPar&);
    ERGadastDigiPar& operator=(const ERGadastDigiPar&);

    ClassDef(ERGadastDigiPar,1);
};


#endif /**  ERGadastDigiPar_H   **/
