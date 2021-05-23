void create_passive_component() {

    // Double_t xOffset = 0.06;
    // Double_t yOffset = 0.074;
    // Double_t zOffset = -0.1;

    Double_t xOffset = 0.;
    Double_t yOffset = -0.24;
    Double_t zOffset = 0.;

    TString erPath = gSystem->Getenv("VMCWORKDIR");
    TString geoFileName = erPath + "/geometry/housingFrames.root";    
    TString medFile = erPath + "/geometry/media.geo";
    // Materials and media
    FairGeoLoader* geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    FairGeoMedia* geoMedia = geoFace->getMedia();
    FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

    // --------------------------------------------------------------------------
    // Geometry manager
    TGeoManager* geoM = (TGeoManager*)gROOT->FindObject("FAIRGeom");    
    geoM->SetName("DETgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    geoM->SetTopVolume(top);
    TGeoVolume* frames = new TGeoVolumeAssembly("frames");

    // read Materials
    // --------------------------------------------------------------------------
    TString mediumName = "Keramik";
    FairGeoMedium* mKeramik = geoMedia->getMedium(mediumName);
    if (!mKeramik) Fatal("create_passive_component", "FairMedium %s not found", mediumName.Data());
    geoBuild->createMedium(mKeramik);
    TGeoMedium* pKeramik = geoM->GetMedium(mediumName);
    if (!pKeramik) Fatal("create_passive_component", "Medium %s not found", mediumName.Data());

    mediumName = "FR4";
    FairGeoMedium* mFR4 = geoMedia->getMedium(mediumName);
    if (!mFR4) Fatal("create_passive_component", "FairMedium %s not found", mediumName.Data());
    geoBuild->createMedium(mFR4);
    TGeoMedium* pFR4 = geoM->GetMedium(mediumName);
    if (!pFR4) Fatal("create_passive_component", "Medium %s not found", mediumName.Data());

    // --------------   Create geometry and top volume  -------------------------

    // thinFrame keramic
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

    // thickFrame FR4
    width = 0.6;

    ext_X = 6.; //cm
    ext_Y = 6.; //cm
    ext_Z = 0.3; //cm

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

    Double_t trans_1_X = 2.08;
    Double_t trans_1_Y = 4.63; 
    Double_t trans_1_Z = 17.3;

    Double_t trans_2_X = 2.35;
    Double_t trans_2_Y = 4.85; 
    Double_t trans_2_Z = 18.8;

    TGeoRotation *fZeroRotation = new TGeoRotation();
    fZeroRotation->RotateX(0.);
    fZeroRotation->RotateY(0.);
    fZeroRotation->RotateZ(0.);

    // --------------------------------------------------------------------------
    frames->AddNode(thinFrame, 0, new TGeoCombiTrans(trans_1_X + xOffset,trans_1_Y + yOffset, trans_1_Z + zOffset, fZeroRotation));
    frames->AddNode(thinFrame, 0, new TGeoCombiTrans(trans_1_Y + xOffset,-trans_1_X + yOffset, trans_1_Z + zOffset, fZeroRotation));
    frames->AddNode(thinFrame, 0, new TGeoCombiTrans(-trans_1_X + xOffset,-trans_1_Y + yOffset, trans_1_Z + zOffset, fZeroRotation));
    frames->AddNode(thinFrame, 0, new TGeoCombiTrans(-trans_1_Y + xOffset,trans_1_X + yOffset, trans_1_Z + zOffset, fZeroRotation));

    frames->AddNode(thickFrame, 0, new TGeoCombiTrans(trans_2_X + xOffset,trans_2_Y + yOffset, trans_2_Z + zOffset, fZeroRotation));
    frames->AddNode(thickFrame, 0, new TGeoCombiTrans(trans_2_Y + xOffset,-trans_2_X + yOffset, trans_2_Z + zOffset, fZeroRotation));
    frames->AddNode(thickFrame, 0, new TGeoCombiTrans(-trans_2_X + xOffset,-trans_2_Y + yOffset, trans_2_Z + zOffset, fZeroRotation));
    frames->AddNode(thickFrame, 0, new TGeoCombiTrans(-trans_2_Y + xOffset,trans_2_X + yOffset, trans_2_Z + zOffset, fZeroRotation));

    frames->AddNode(thickFrame, 0, new TGeoCombiTrans(trans_2_X + xOffset,trans_2_Y + yOffset, trans_2_Z+1. + zOffset, fZeroRotation));
    frames->AddNode(thickFrame, 0, new TGeoCombiTrans(trans_2_Y + xOffset,-trans_2_X + yOffset, trans_2_Z+1. + zOffset, fZeroRotation));
    frames->AddNode(thickFrame, 0, new TGeoCombiTrans(-trans_2_X + xOffset,-trans_2_Y + yOffset, trans_2_Z+1. + zOffset, fZeroRotation));
    frames->AddNode(thickFrame, 0, new TGeoCombiTrans(-trans_2_Y + xOffset,trans_2_X + yOffset, trans_2_Z+1. + zOffset, fZeroRotation));

    top->AddNode(frames,0,new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
    // --------------------------------------------------------------------------
    geoM->CloseGeometry();
    geoM->CheckOverlaps(0.001);
    geoM->PrintOverlaps();
    geoM->Test();

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
    // --------------------------------------------------------------------------
}   
