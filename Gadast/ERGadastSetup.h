// -------------------------------------------------------------------------
// -----                        ERGadastSetup header file               -----
// -----                  Created data  by Schetinin Vitaliy           -----
// -------------------------------------------------------------------------


#ifndef ERGadastSetup_H
#define ERGadastSetup_H


#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TGeoPhysicalNode.h"


#include "ERGadastDigiPar.h"
#include "ERSupport.h"

class ERGadastSetup {
public:
	static ERGadastSetup* Instance();
	static Bool_t Init();
	static Float_t CsILC(TVector3* pos);
    static Float_t LaBrLC(TVector3* pos);
    static Float_t CsIDispA(TVector3* pos);
    static Float_t LaBrDispA(TVector3* pos);
    static Float_t CsIDispB(TVector3* pos);
    static Float_t LaBrDispB(TVector3* pos);

	int GetMeshElement(TVector3* pos, SensetiveType detType);
private:
	ERGadastSetup();
	static ERGadastSetup* fInstance;
	static ERGadastDigiPar* fDigiPar;
	static TGeoPhysicalNode* fGadastNode;
};

#endif