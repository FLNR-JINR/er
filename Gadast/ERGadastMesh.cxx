#include "ERGadastMesh.h"

#include "TMath.h"
#include <iostream>

using namespace std;

ERGadastMesh::ERGadastMesh()
{

}

int ERGadastMesh::GetMeshElement(TLorentzVector* pos, TGeoMatrix* gm, ERGadastDetectorType detType){
	Double_t posMaster[3];
	posMaster[0] = pos->X(); posMaster[1] = pos->Y();  posMaster[2] = pos->Z();
	Double_t posLocal[3];
	gm->MasterToLocal(posMaster,posLocal);
	if(detType == CsI){
		Int_t xCount=10, yCount=10, zCount = 10;
		Double_t dX = 72.8/(double)xCount;
		Double_t dY = 41.5/(double)yCount;
		Double_t dZ = 150.1/(double)zCount; // 4 ячейки длина криистала 150.1
		posLocal[0] += 72.8/2;
		posLocal[1] += 41.5/2;
		posLocal[2] += 30.; //так как начало координат в начале координат первого trd. сдвигаем ее на поверхность второго trd
		
		Int_t x = (Int_t)(posLocal[0]/dX);
		Int_t y = (Int_t)(posLocal[1]/dY);
		Int_t z = (Int_t)(posLocal[2]/dZ);
		//cerr << "local pos " << endl;
		//cerr << posLocal[0] << " " << posLocal[1] << " " << posLocal[2] << endl;
		//cerr << "mesh element" << endl;
		//cerr << x << " " << y << " " << z << endl;
		Int_t MeshElement = z*xCount*yCount + y*xCount + x;
		return MeshElement;
	}
	if (detType == LaBr){
		Int_t zCount=10;
		Double_t dZ = 50.8/(double)zCount; // 4 ячейки длина криистала 150.1
		posLocal[2] += 50.8/2; //так как начало координат в центре tube
		
		Int_t z = (Int_t)(posLocal[2]/dZ);
		return z;
	}
}