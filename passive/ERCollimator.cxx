#include "ERCollimator.h"

#include "TString.h"
#include "TGeoMatrix.h"

ERCollimator::ERCollimator()
{
	fPositionRotation = new TGeoCombiTrans("mCollimator", 0., 0., 0., new TGeoRotation("rotNoRot", 0., 0., 0.));
}

ERCollimator::~ERCollimator()
{
}

void ERCollimator::ConstructGeometry()
{
	TString fileName = GetGeometryFileName();
	if (fileName == "") {
		LOG(FATAL) << "Collimator geometry file name is not set." << FairLogger::endl;
	} else if(fileName.EndsWith(".root")) {
		LOG(INFO) << "Constructing collimator geometry from ROOT file " << fileName.Data() << FairLogger::endl;
		ConstructRootGeometry();
	} else if(fileName.EndsWith(".gdml")) {
		LOG(INFO) << "Constructing collimator geometry from GDML file " << fileName.Data() << FairLogger::endl;
		ConstructGDMLGeometry(fPositionRotation);
	} else {
		LOG(FATAL) << "Collimator geometry file name is not correct." << FairLogger::endl;
	}
}

ClassImp(ERCollimator)
