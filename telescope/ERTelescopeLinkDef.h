#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

//--------- setup ----------------/

#pragma link C++ class ERRTelescopeSetup+;

#pragma link C++ class ERQTelescopeSetup+;
#pragma link C++ class ERQTelescopeGeoComponentDoubleSi+;
#pragma link C++ class ERQTelescopeGeoNonUniformSingleSi+;
#pragma link C++ class ERQTelescopeGeoComponentSingleSi+;
#pragma link C++ class ERQTelescopeGeoComponentCsI+;

//--------- simulation ----------/

#pragma link C++ class ERRTelescope+;
// #pragma link C++ class ERRTelescopeDigitizer+;
#pragma link C++ class ERRTelescopeCsIPoint+;
#pragma link C++ class ERRTelescopeSiPoint+;
#pragma link C++ class ERRTelescopeCsIDigi+;
#pragma link C++ class ERRTelescopeSiDigi+;

#pragma link C++ class ERQTelescope+;
#pragma link C++ class ERQTelescopeDigitizer+;
#pragma link C++ class ERQTelescopeSiPoint+;
#pragma link C++ class ERQTelescopeSiDigi+;
#pragma link C++ class ERQTelescopeCsIPoint+;
#pragma link C++ class ERQTelescopeCsIDigi+;

//--------- reco ---------------/

#pragma link C++ class ERQTelescopeTrackFinder+;
#pragma link C++ class ERQTelescopeTrack+;

#pragma link C++ class ERQTelescopePID+;
#pragma link C++ class ERQTelescopeParticle+;

#endif