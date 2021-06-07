void create_thin_frame() {
 	TFile* geo_file = new TFile("thin_frame.root", "RECREATE");
    TString erPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = erPath + "/geometry/media.geo";
    // Materials and media
    FairGeoLoader* geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    FairGeoMedia* geoMedia = geoFace->getMedia();
    FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
    // Geometry manager
    TGeoManager* geoM = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    // Materials
    TString mediumName = "Keramik";
    FairGeoMedium* mKeramik = geoMedia->getMedium(mediumName);
    if (!mKeramik) Fatal("create_passive_component", "FairMedium %s not found", mediumName.Data());
    geoBuild->createMedium(mKeramik);
    TGeoMedium* pKeramik = geoM->GetMedium(mediumName);
    if (!pKeramik) Fatal("create_passive_component", "Medium %s not found", mediumName.Data());
    // thinFrame 
    Double_t ext_X,ext_Y,ext_Z; // box of steel
    Double_t width = 0.855;
    ext_X = 5.; //cm
    ext_Y = 5.; //cm
    ext_Z = 0.066; //cm
    ext_X /= 2.;
    ext_Y /= 2.;
    ext_Z /= 2.;
    width /= 2.;
    // --------------------------------------------------------------------------
    TGeoBBox boxThinExt = TGeoBBox("boxThinExt", ext_X+width, ext_Y+width, ext_Z);
    TGeoBBox boxThinInt = TGeoBBox("boxThinInt", ext_X, ext_Y, ext_Z);
    TGeoCompositeShape *c_Thinframe = new TGeoCompositeShape("c_Thinframe", "(boxThinExt) - (boxThinInt)");
    TGeoVolume* thinFrame = new TGeoVolume("thinFrame", c_Thinframe, pKeramik);
    //---------------------------------------------------------------------------
    geoM->SetTopVolume(thinFrame);
    geoM->CloseGeometry();
    geoM->GetTopVolume()->Write();
    geo_file->Close();
 }