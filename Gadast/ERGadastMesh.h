// -------------------------------------------------------------------------
// -----                        ERGadastMesh header file               -----
// -----                  Created data  by Schetinin Vitaliy           -----
// -------------------------------------------------------------------------


#ifndef ERGadastMesh_H
#define ERGadastMesh_H


#include "TLorentzVector.h"
#include "TGeoMatrix.h"

#include "ERDetectorList.h"

class ERGadastMesh {
public:
	ERGadastMesh();

	int GetMeshElement(TLorentzVector* pos, TGeoMatrix* gm, ERGadastDetectorType detType);
};

#endif