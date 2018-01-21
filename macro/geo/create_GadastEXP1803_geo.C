// ----- One cell CsI parameters ----------------------------------------------
Double_t cellOutX1 = 6.02, cellOutX2 = 7.28; 
Double_t cellOutY1 = 3.52, cellOutY2 = 4.15; 
Double_t cellOutZ  = 12.01;
Double_t cellInX1  = 7.28, cellInX2  = 3.82; 
Double_t cellInY1  = 4.15, cellInY2  = 3.82; 
Double_t cellInZ   = 3.;
// ----- One cell shell parameters --------------------------------------------
Double_t cellShellOutX1 = 6.03233594179232, cellShellOutX2 = 7.29301787351589; 
Double_t cellShellOutY1 = 3.53266350482021, cellShellOutY2 = 4.163004470682; 
Double_t cellShellOutZ  = 12.0165;
Double_t cellShellInX1  = 7.29301787351589, cellShellInX2  = 3.82750999888938; //3
Double_t cellShellInY1  = 4.163004470682,   cellShellInY2  = 3.83230464765268; 
Double_t cellShellInZ   = 3.0065;
// ----- Transtalstion parameters ---------------------------------------------
Double_t rotAngle = 30.;
Double_t magnetCenterZ = 30.;
Double_t distToMagnetCenter = 30.;

void create_GadastEXP1803_geo()
{
  // Create a global translation
  Float_t global_X = 0.;
  Float_t global_Y = 0.;
  Float_t global_Z = 0.;
  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  // Create a 90 degree rotation around Z axis
  TGeoRotation *f90RotationZ = new TGeoRotation();
  f90RotationZ->RotateX(0.);
  f90RotationZ->RotateY(0.);
  f90RotationZ->RotateZ(-90.);

  TGeoManager*   gGeoMan = NULL;
  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------
  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/partOfGadast.v1.geo.root";
  // --------------------------------------------------------------------------
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // ----- Create media for Gadast --------------------------------------------
  FairGeoMedium* mCsI      = geoMedia->getMedium("CsI");
  if ( ! mCsI ) Fatal("Main", "FairMedium CsI not found");
  geoBuild->createMedium(mCsI);
  TGeoMedium* pCsI = gGeoMan->GetMedium("CsI");
  if ( ! pCsI ) Fatal("Main", "Medium CsI not found");

  FairGeoMedium* mMylar      = geoMedia->getMedium("mylar");
  if ( ! mMylar ) Fatal("Main", "FairMedium mylar not found");
  geoBuild->createMedium(mMylar);
  TGeoMedium* pMylar = gGeoMan->GetMedium("mylar");
  if ( ! pMylar ) Fatal("Main", "Medium mylar not found");
  // ------ Create vacuum media -----------------------------------------------
  FairGeoMedium* vacuum      = geoMedia->getMedium("vacuum");
  if ( ! vacuum ) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(vacuum);
  TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
  if ( ! pMed0 ) Fatal("Main", "Medium vacuum not found");

  //------------------------- VOLUMES -----------------------------------------

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("PartOfGadastGeom");
  TGeoVolume* top   = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  TGeoVolume* csiModule = new TGeoVolumeAssembly("csiModule");
  TGeoVolume* gadast = new TGeoVolumeAssembly("PartOfGadast");
  // ------- CsI cell ---------------------------------------------------------
  TGeoTrd2 csiCellOut  = TGeoTrd2("csiCellOut", cellOutX1 / 2, cellOutX2 / 2, 
                                                cellOutY1 / 2, cellOutY2 / 2, 
                                                cellOutZ / 2);
  TGeoTrd2 csiCellIn   = TGeoTrd2("csiCellIn" , cellInX1 / 2,  cellInX2 / 2,  
                                                cellInY1 / 2,  cellInY2 / 2,  
                                                cellInZ / 2);
  TGeoCombiTrans* transCell = new TGeoCombiTrans("transCell", 0., 
                                                              0., 
                                                              (cellOutZ + cellInZ) / 2, 
                                                              fZeroRotation);
  transCell->RegisterYourself();
  TGeoCompositeShape *csiCellShape = new TGeoCompositeShape("csiCellShape", 
                                                            "(csiCellOut) + (csiCellIn:transCell)");
  TGeoVolume* csiCell = new TGeoVolume("onecell_cell", csiCellShape, pCsI);

  // ------ Cell shell --------------------------------------------------------
  TGeoTrd2 csiCellShellOut  = TGeoTrd2("csiCellShellOut", cellShellOutX1 / 2, cellShellOutX2 / 2, 
                                                          cellShellOutY1 / 2, cellShellOutY2 / 2, 
                                                          cellShellOutZ / 2);
  TGeoTrd2 csiCellShellIn   = TGeoTrd2("csiCellShellIn" , cellShellInX1 / 2,  cellShellInX2 / 2,  
                                                          cellShellInY1 / 2,  cellShellInY2 / 2,  
                                                          cellShellInZ / 2);
  TGeoCombiTrans* transCellShell = new TGeoCombiTrans("transCellShell", 0., 
                                                                        0., 
                                                                        (cellShellOutZ + cellShellInZ) / 2, 
                                                                        fZeroRotation);
  transCellShell->RegisterYourself();
  TGeoCompositeShape *csiCellShellShape = new TGeoCompositeShape("csiCellShellShape", 
                                                                 "(csiCellShellOut) + (csiCellShellIn:transCellShell)");
  TGeoVolume* csiCellShell = new TGeoVolume("csiCellShell", csiCellShellShape, pMylar);

  csiCellShell->AddNode(csiCell, 0, new TGeoCombiTrans(0, 0, 0, fZeroRotation));


  Double_t angleStep = 90 - (180. / TMath::Pi())*
                                    (TMath::Abs(TMath::ATan((cellShellOutZ) / 
                                    (TMath::Abs(cellShellOutY1 - cellShellOutY2) / 2))));
  Double_t transStepY = (cellShellOutY2 + cellShellOutY1) / 2.;
  Double_t transStepZ = 0.201635;
  Double_t angle  = - angleStep;
  Double_t transY = - transStepY / 2;
  Double_t transZ = - 1.40897636833871;

  for (Int_t csiInd = 6 / 2; csiInd > 0; csiInd--) {
    TGeoRotation *rotation = new TGeoRotation();
    rotation->RotateX(angle);
    TGeoCombiTrans *translate = new TGeoCombiTrans(0, -transY, transZ, rotation);
    csiModule->AddNode(csiCellShell, csiInd, translate);
    angle  -= angleStep;
    transY -= transStepY;
    transZ -= transStepZ; 
  }
  transZ = -1.40897636833871;
  transY =  transStepY / 2;
  angle  =  angleStep;
  for (Int_t csiInd = 6 / 2 + 1; csiInd <= 6; csiInd++) {
    TGeoRotation *rotation = new TGeoRotation();
    rotation->RotateX(angle);
    TGeoCombiTrans *translate = new TGeoCombiTrans(0, -transY, transZ, rotation);
    csiModule->AddNode(csiCellShell, csiInd, translate);
    angle  += angleStep;
    transY += transStepY;
    transZ -= transStepZ; 
  } 
  gadast->AddNode(csiModule, 1, new TGeoCombiTrans(0, 0, 0, f90RotationZ));
  Double_t dZ = distToMagnetCenter * TMath::Cos(rotAngle * TMath::Pi() / 180);
  Double_t dX = distToMagnetCenter * TMath::Sin(rotAngle * TMath::Pi() / 180);
  Double_t dAlpha = 180 / TMath::Pi() * TMath::ATan(dX / (magnetCenterZ + dZ));

  TGeoRotation *fCsIRotation = new TGeoRotation();
  fCsIRotation->RotateX(0.);
  fCsIRotation->RotateY(dAlpha);
  fCsIRotation->RotateZ(0.);

  top->AddNode(gadast, 1, new TGeoCombiTrans(dX, 
                                             0., 
                                             magnetCenterZ + dZ, 
                                             fCsIRotation));
 // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();

/*  cout << "Dx rot12 " << 4.50725238515248 - 1.50241746171749 << endl;
  cout << "Dy step12 " << 57.6447825558185 - (19.2325558894204) << endl;
  cout << "Dx rot23 " << 1.50241746171749 + 1.50241746171749 << endl;
  cout << "Dy step23 " << 19.2325558894204 + (19.2325558894204) << endl;
  cout << "Dz step " << -16.1061135260391 - (-14.0897636833871) << endl;

  cout << "calc angle " << 90 - (180 / TMath::Pi())*(TMath::Abs(TMath::ATan((cellShellOutZ) 
                                        / 
                                        (TMath::Abs((cellShellOutX1 - cellShellOutX2) / 2))))) << endl;
 */ // --------------------------------------------------------------------------
}