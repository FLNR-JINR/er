/*
 * ERNXyterCalibrator.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#include "ERNXyterCalibrator.h"

#include "TFile.h"

ERNXyterCalibrator& ERNXyterCalibrator::Instance(void) {
	static ERNXyterCalibrator instance;
	return instance;
}

ERNXyterCalibrator::ERNXyterCalibrator() {
	fConstructed = kFALSE;

}

ERNXyterCalibrator::~ERNXyterCalibrator() {
	this->Deconstruct();
}

void ERNXyterCalibrator::Deconstruct(void) {
	if (fConstructed) {
		//for (Int_t i = 0; i < 64; i++) {
		//    delete fGraph[i];                       // This causes double-free error. Why!?
		//}
		fConstructed = kFALSE;
	}
}

UInt_t ERNXyterCalibrator::ImportGraphs(TString graphFile) {
	//TODO check - check that there is no memory already allocated.
	//TODO If already allocated - delete! Next allocatation is invoked in GenerateLUTs().
	if (fConstructed) {
		this->Deconstruct();
	}

	TFile fr(graphFile.Data());
	if (fr.IsZombie()) {
	   TObject::Error("ERNXyterCalibrator::ImportGraphs", "Error opening file %s", graphFile.Data());
	   return 1;
	}

	TList *gList = fr.GetListOfKeys();
	const Int_t nEntries = gList->GetEntries();
	if (nEntries != 64) {
		Error("ERNXyterCalibrator::ImportGraphs", "Something wrong, only %d graphs found instead of 64.", nEntries);
		return 1; // FAIL
	}

	TString gName;
	for (Int_t i=0; i<nEntries; i++) {
		gName.Form("gcal%d", i);
		fGraph[i] = (TGraph*)fr.Get(gName.Data());
		if (fGraph[i] == 0) {
			Error("ERNXyterCalibrator::ImportGraphs", "Graph %s was not found in file %s", gName.Data(), fr.GetName());
			fr.Close();
			return 1; // FAIL
		}
	}

	fr.Close();

//	printf("!!!!!!!!!!!!  %s !!!!!!!!!!!!!!!", fGraph[4]->GetName() );

//	this->GenerateLUTs();

//	this->ExportLUTs("LUTs.root");

	return 0; // OK
}

//unsigned int ERNXyterCalibrator::ExportLUTs(TString filename) {
//}

Float_t ERNXyterCalibrator::GetCalibratedVal(UInt_t ch, Float_t val) {
//	if (!fConstructed) this->GenerateDummyLUTs();

	if (ch>=64) return 0.; // We basically ignore channels 64-128

	// Bin number as input!
//    return fLUT[ch]->GetBinContent((UInt_t)val+200);
	if (fGraph[ch] == NULL) return (Float_t)val;

	return (Float_t)fGraph[ch]->Eval(val);
}

//void ERNXyterCalibrator::GenerateDummyLUTs(void) {
//}
//
//void ERNXyterCalibrator::GenerateLUTs(void) {
//}
