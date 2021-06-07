void create_thick_frame() {
 	TFile* geo_file = new TFile("thick_frame.root", "RECREATE");
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
    TString mediumName = "FR4";
    FairGeoMedium* mFR4 = geoMedia->getMedium(mediumName);
    if (!mFR4) Fatal("create_passive_component", "FairMedium %s not found", mediumName.Data());
    geoBuild->createMedium(mFR4);
    TGeoMedium* pFR4 = geoM->GetMedium(mediumName);
    if (!pFR4) Fatal("create_passive_component", "Medium %s not found", mediumName.Data());
    // thickFrame FR4
    float width = 0.6;
    float ext_X = 6.; //cm
    float ext_Y = 6.; //cm
    float ext_Z = 0.3; //cm
    ext_X /= 2.;
    ext_Y /= 2.;
    ext_Z /= 2.;
    width /= 2.;
    // --------------------------------------------------------------------------
    TGeoBBox boxThickExt = TGeoBBox("boxThickExt", ext_X+width, ext_Y+width, ext_Z);
    TGeoBBox boxThickInt = TGeoBBox("boxThickInt", ext_X, ext_Y, ext_Z);
    TGeoCompositeShape *c_Thickframe = new TGeoCompositeShape("c_Thickframe", "(boxThickExt) - (boxThickInt)");
    TGeoVolume* thickFrame = new TGeoVolume("thickFrame", c_Thickframe, pFR4);
    //---------------------------------------------------------------------------
    geoM->SetTopVolume(thickFrame);
    geoM->CloseGeometry();
    geoM->GetTopVolume()->Write();
    geo_file->Close();
 }