void create_passive_component() {
	TFile* geo_file = new TFile("passive_component.root", "RECREATE");

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
    TString mediumName = "lead";
    FairGeoMedium* mLead = geoMedia->getMedium(mediumName);
    if (!mLead) Fatal("create_passive_component", "FairMedium %s not found", mediumName.Data());
    geoBuild->createMedium(mLead);
    TGeoMedium* pLead = geoM->GetMedium(mediumName);
    if (!pLead) Fatal("create_passive_component", "Medium %s not found", mediumName.Data());
    TGeoVolume *passiveElement=geoM->MakeBox("PassiveElement",pLead,1.,1.,0.1);
    TGeoVolume *interVolume=geoM->MakeBox("InterVolume",pLead,.1,.1,0.001);
    TGeoRotation *zeroRotation = new TGeoRotation();
    zeroRotation->RotateX(0.);
    zeroRotation->RotateY(0.);
    zeroRotation->RotateZ(0.);
    passiveElement->AddNode(interVolume, 1, new TGeoCombiTrans(0, 0, 0, zeroRotation));
    geoM->SetTopVolume(passiveElement);
    geoM->CloseGeometry();
    geoM->GetTopVolume()->Write();
    geo_file->Close();
}   