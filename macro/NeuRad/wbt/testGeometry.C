void testGeometry()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");
  TString geoFilename = erPath + "/geometry/NeuRad_Wupper_Proto.geo.root";
  TGeoManager* geoMan = new TGeoManager("geoMan", "geoMan");
  cout << "Reading input geometry file: " << geoFilename << endl;
  TFile* geoFile = new TFile(geoFilename, "READ");
  TGeoVolumeAssembly* rootTop = (TGeoVolumeAssembly*)geoFile->Get("TOP");
  geoMan->SetTopVolume(rootTop);
  geoMan->CloseGeometry();


  TGeoNode* topNode = geoMan->GetTopNode();
  TGeoNode* neuradNode = topNode->GetDaughter(0);

  cout << "TOP:    " << topNode->GetName() << endl;
  cout << "NeuRad: " << neuradNode->GetName() << endl;

  TGeoNode* moduleNode = NULL;

  Int_t modulesCounter = 0;
  for (Int_t i=0; i<neuradNode->GetNdaughters(); i++) {
    TGeoNode* childNode = neuradNode->GetDaughter(i);
    TString childNodeName(childNode->GetName());
    if (childNodeName.Contains("module", TString::kIgnoreCase)) {
      modulesCounter++;
      moduleNode = childNode;
    }
  }
  cout << "        " << modulesCounter << " modules in " << neuradNode->GetName() << endl;

  if (moduleNode == NULL) {
    cerr << "moduleNode is NULL" << endl;
    return;
  }

  cout << "Module: " << moduleNode->GetName() << endl;

  TGeoNode* submoduleNode = NULL;

  Int_t submodulesCounter = 0;
  for (Int_t iSubmodule=0; iSubmodule<moduleNode->GetNdaughters(); iSubmodule++) {
    TGeoNode* childNode = moduleNode->GetDaughter(iSubmodule);
    TString childNodeName(childNode->GetName());
    if (childNodeName.Contains("submodule", TString::kIgnoreCase)) {
      submodulesCounter++;
      submoduleNode = childNode;
      cout << "        " << childNodeName << endl;
      TGeoMatrix* curM = childNode->GetMatrix();
      const Double_t* curTrans = curM->GetTranslation();
      cout << "        x=" << curTrans[0] << "\ty=" << curTrans[1] << "\tz=" << curTrans[2] << endl;
    }
  }

  cout << "        " << submodulesCounter << " submodules in " << moduleNode->GetName() << endl;

  if (submoduleNode == NULL) {
    cerr << "submoduleNode is NULL" << endl;
    return;
  }

  cout << "Submodule: " << submoduleNode->GetName() << endl;

  TGeoNode* pixelNode = NULL;

  Int_t pixelsCounter = 0;
  for (Int_t iPixel=0; iPixel<submoduleNode->GetNdaughters(); iPixel++) {
    TGeoNode* childNode = submoduleNode->GetDaughter(iPixel);
    TString childNodeName(childNode->GetName());
    if (childNodeName.Contains("pixel", TString::kIgnoreCase)) {
      pixelsCounter++;
      pixelNode = childNode;
      cout << "        " << childNodeName << endl;
      TGeoMatrix* curM = childNode->GetMatrix();
      const Double_t* curTrans = curM->GetTranslation();
      cout << "        x=" << curTrans[0] << "\ty=" << curTrans[1] << "\tz=" << curTrans[2] << endl;
    }
  }

  // etc...

}
