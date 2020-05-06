#include "ERGadastSetup.h"

#include "TMath.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoNode.h"
#include <iostream>

#include "FairRun.h"
#include "FairRuntimeDb.h"

#include "ERDetectorList.h"

using namespace std;

ERGadastSetup* ERGadastSetup::fInstance = NULL;
ERGadastDigiPar* ERGadastSetup::fDigiPar;
TGeoPhysicalNode* ERGadastSetup::fGadastNode;
//----------------------------------------------------------------------------
ERGadastSetup* ERGadastSetup::Instance(){
    if (fInstance == NULL)
        return new ERGadastSetup();
    else
        return fInstance;
}
//----------------------------------------------------------------------------
ERGadastSetup::ERGadastSetup()
{

	FairRun* run = FairRun::Instance();
    if ( ! run ) Fatal("ERGadastSetup", "No analysis run");

    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    if ( ! rtdb ) Fatal("ERGadastSetup", "No runtime database");

    fDigiPar = (ERGadastDigiPar*)(rtdb->getContainer("ERGadastDigiPar"));
    if ( ! fDigiPar ) Fatal("ERGadastSetup`", "No ERNeuRadDigiPar in runtime");
}
//----------------------------------------------------------------------------
int ERGadastSetup::GetMeshElement(TVector3* pos, SensetiveType detType){
	TGeoNode* node = gGeoManager->FindNode(pos->X(), pos->Y(), pos->Z());
	TGeoMatrix* gm = node->GetMatrix();

	Double_t posMaster[3];
	posMaster[0] = pos->X(); posMaster[1] = pos->Y();  posMaster[2] = pos->Z();
	Double_t posLocal[3];
	gm->MasterToLocal(posMaster,posLocal);
	if(detType == CsI){
		Int_t xCount=4,yCount=4,zCount=4;
		Double_t dX = 72.8/(double)xCount;
		Double_t dY = 41.5/(double)yCount;
		Double_t dZ = 150.1/(double)zCount; // 4 ячейки длина криистала 150.1
		posLocal[0] += 72.8/2;
		posLocal[1] += 41.5/2;
		posLocal[2] += 30.; //так как начало координат в начале координат первого trd. сдвигаем ее на поверхность второго trd
		
		Int_t x = (Int_t)(posLocal[0]/dX);
		Int_t y = (Int_t)(posLocal[1]/dY);
		Int_t z = (Int_t)(posLocal[2]/dZ);

		Int_t MeshElement = z*xCount*yCount + y*xCount + x;
		return MeshElement;
	}
	if (detType == LaBr){
		Int_t zCount=4;
		Double_t dZ = 50.8/(double)zCount; // 4 ячейки длина криистала 150.1
		posLocal[2] += 50.8/2; //так как начало координат в центре tube
		
		Int_t z = (Int_t)(posLocal[2]/dZ);
		return z;
	}
}
//----------------------------------------------------------------------------
Bool_t ERGadastSetup::Init(){
	// --- Get cave (top node)
	TGeoManager* geo = gGeoManager;
	if (!gGeoManager){
		Fatal("ERGadastSetup","Can`t find gGeoManager!");
		return kFALSE;
	}
	cout << "Reading geometry from TGeoManager " << geo->GetName() << endl;
  	geo->CdTop();
  	TGeoNode* cave = geo->GetCurrentNode();
  	 // --- Get top Gaadast node
	TGeoNode* gadast = NULL;
	for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
		TString name = cave->GetDaughter(iNode)->GetName();
		if ( name.Contains("Gadast", TString::kIgnoreCase) ) {
			gadast = cave->GetDaughter(iNode);
  			geo->CdDown(iNode);
  			cout << "Gadast top node is " << gadast->GetName() << endl;
		  	break;
		}
	}

	if ( ! gadast ) {
		cout << "No top Gadast node found in geometry!" << endl;
		return kFALSE;
	}

  	// --- Create physical node for sts
    TString path = cave->GetName();
    path = path + "/" + gadast->GetName();
    fGadastNode = new TGeoPhysicalNode(path);
    
    return kTRUE;
}

//----------------------------------------------------------------------------
Float_t ERGadastSetup::CsILC(TVector3* pos){
	return fDigiPar->CsILC(fInstance->GetMeshElement(pos, CsI));
}
//----------------------------------------------------------------------------
Float_t ERGadastSetup::LaBrLC(TVector3* pos){
	return fDigiPar->LaBrLC(fInstance->GetMeshElement(pos, LaBr));
}
//----------------------------------------------------------------------------
Float_t ERGadastSetup::CsIDispA(TVector3* pos){
	return fDigiPar->CsIDispA(fInstance->GetMeshElement(pos, CsI));
}
//----------------------------------------------------------------------------
Float_t ERGadastSetup::LaBrDispA(TVector3* pos){
	return fDigiPar->LaBrDispA(fInstance->GetMeshElement(pos, LaBr));
}
//----------------------------------------------------------------------------
Float_t ERGadastSetup::CsIDispB(TVector3* pos){
	return fDigiPar->CsIDispB(fInstance->GetMeshElement(pos, CsI));
} 
//----------------------------------------------------------------------------
Float_t ERGadastSetup::LaBrDispB(TVector3* pos){
	return fDigiPar->LaBrDispB(fInstance->GetMeshElement(pos, LaBr));
} 
//----------------------------------------------------------------------------