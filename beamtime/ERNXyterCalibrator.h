/*
 * ERNXyterCalibrator.h
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#ifndef BEAMTIME_ERNXYTERCALIBRATOR_H_
#define BEAMTIME_ERNXYTERCALIBRATOR_H_

#include "TString.h"
#include "TGraph.h"
#include "TH1D.h"
//#include "Rtypes.h"
//#include "TObject.h"

class ERNXyterCalibrator : public TObject {

private: //Singleton standart
	ERNXyterCalibrator();
	virtual ~ERNXyterCalibrator();
	ClassDef(ERNXyterCalibrator,1);

public:	//Singleton standard
	static ERNXyterCalibrator& Instance(void);

public: // methods
    UInt_t ImportGraphs(TString graphFile);

    // Input value - (ped - rawVal)
    Float_t GetCalibratedVal(UInt_t ch, Float_t val);

private: // methods
//    void GenerateDummyLUTs(void);
//    void GenerateLUTs(void);
    void Deconstruct(void);

private: // data members
    TGraph* fGraph[64];

    Bool_t fConstructed; // Defines whether the histograms have been allocated

};

#endif /* BEAMTIME_ERNXYTERCALIBRATOR_H_ */
