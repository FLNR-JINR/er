#include "ERCollimator.h"
#include "TString.h"

ERCollimator::ERCollimator(){

}

ERCollimator::~ERCollimator(){

}

void ERCollimator::ConstructGeometry(){
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    std::cout << "Constructing Target geometry from ROOT file " << fileName.Data() << std::endl;
    ConstructRootGeometry();
  } else {
    std::cerr << "Target geometry file name is not set" << std::endl;
    exit(1);
  }
}

ClassImp(ERCollimator)