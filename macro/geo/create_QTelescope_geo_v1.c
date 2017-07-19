void create_QTelescope_geo_v1()
{
  double first_wall_Z = 5;
  double sec_wall_Z = first_wall_Z + 1;
  double blocks_of_CSI_Z = sec_wall_Z + 1;
  // Create a global translation
  Float_t global_X = 0.;
  Float_t global_Y = 0.;
  Float_t global_Z = 0.;
  //Create gloabal Rotation
  TGeoRotation *fGlobalRotation = new TGeoRotation();
  fGlobalRotation->RotateX(0.);
  fGlobalRotation->RotateY(0.);
  fGlobalRotation->RotateZ(0.);
  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);


  TGeoRotation *f90deg_Z_Rotation = new TGeoRotation();
  f90deg_Z_Rotation->RotateX(0.);
  f90deg_Z_Rotation->RotateY(0.);
  f90deg_Z_Rotation->RotateZ(90.);

  TGeoManager*   gGeoMan = NULL;
// -------   Load media from media file   -----------------------------------
FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
FairGeoInterface* geoFace = geoLoad->getGeoInterface();
TString geoPath = gSystem->Getenv("VMCWORKDIR");
TString medFile = geoPath + "/geometry/media.geo";
geoFace->setMediaFile(medFile);
geoFace->readMedia();
gGeoMan = gGeoManager;

// -------   Geometry file name (output)   ----------------------------------
TString geoFileName = geoPath + "/geometry/QTelescope.v1.geo.root";
// --------------------------------------------------------------------------


// -----------------   Get and create the required media    -----------------
FairGeoMedia*   geoMedia = geoFace->getMedia();
FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

FairGeoMedium* msilicon      = geoMedia->getMedium("silicon");
if ( ! msilicon ) Fatal("Main", "FairMedium silicon not found");
geoBuild->createMedium(msilicon);
TGeoMedium* psilicon = gGeoMan->GetMedium("silicon");
if ( ! psilicon ) Fatal("Main", "Medium silicon not found");


//---CsI
FairGeoMedium* mCsI      = geoMedia->getMedium("CsI");
if ( ! mCsI ) Fatal("Main", "FairMedium Csi not found");
geoBuild->createMedium(mCsI);
TGeoMedium* pCsI = gGeoMan->GetMedium("CsI");
if ( ! pCsI ) Fatal("Main", "Medium CsI not found");

//------------------------- VOLUMES -----------------------------------------

// --------------   Create geometry and top volume  -------------------------
gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
gGeoMan->SetName("DETgeom");
TGeoVolume* top = new TGeoVolumeAssembly("TOP");
gGeoMan->SetTopVolume(top);
TGeoVolume* det = new TGeoVolumeAssembly("det");
// --------------------------------------------------------------------------

//------------------ Silicon box statin   -----------------------------------------
Double_t station_X = 9.92;   //cm
Double_t station_Y = 9.92;  //cm
Double_t station_Z = 0.03;  //cm
station_X /= 2.;
station_Y /= 2.;
station_Z /= 2.;
TGeoVolume *Sil_station = gGeoManager->MakeBox("Sil_station", psilicon, station_X, station_Y, station_Z);

//------------------ Silicon fiber   -----------------------------------------
Double_t fiber_X = 9.408;   //cm
Double_t fiber_Y = 0.147;   //cm
Double_t fiber_Z = 0.03;  //cm
fiber_X /= 2.;
fiber_Y /= 2.;
fiber_Z /= 2.;
TGeoVolume *fiber = gGeoManager->MakeBox("fiber", psilicon, fiber_X, fiber_Y, fiber_Z);

//------------------ CsI BOX   -----------------------------------------
Double_t box_CsI_X = 4.;   //cm
Double_t box_CsI_Y = 5.;   //cm
Double_t box_CsI_Z = 5.;  //cm
box_CsI_X /= 2.;
box_CsI_Y /= 2.;
box_CsI_Z /= 2.;
TGeoVolume *box_CsI = gGeoManager->MakeBox("box_CsI", pCsI, box_CsI_X, box_CsI_Y, box_CsI_Z);



//------------------ STRUCTURE  -----------------------------------------
//------------------ Add fibers to station  along x -----------------------------
Int_t fibers_in_det_X_Nb = 1;
Int_t fibers_in_det_Y_Nb = 64;

Double_t det_X = fiber_X * fibers_in_det_X_Nb;
Double_t det_Y = fiber_Y * fibers_in_det_Y_Nb;
Double_t det_Z = fiber_Z;
Int_t i_fiber = 0;

for (Int_t i_Y_fiber = 0; i_Y_fiber < fibers_in_det_Y_Nb; i_Y_fiber++){
  Double_t fiber_in_det_Y_trans = det_Y - fiber_Y*2*(i_Y_fiber)-fiber_Y;
  Double_t fiber_in_det_X_trans = 0.;
  Double_t fiber_in_det_Z_trans = 0.;
  Sil_station->AddNode( fiber, i_fiber, new TGeoCombiTrans(fiber_in_det_X_trans,
                                                        fiber_in_det_Y_trans,
                                                        fiber_in_det_Z_trans,
                                                        fZeroRotation));

}



det->AddNode(Sil_station,1,new TGeoCombiTrans(0,station_Y, sec_wall_Z + fiber_Z, f90deg_Z_Rotation));
det->AddNode(Sil_station,1,new TGeoCombiTrans(0,station_Y, first_wall_Z + fiber_Z, fZeroRotation));

det->AddNode(box_CsI,1,new TGeoCombiTrans(box_CsI_X,box_CsI_Y, blocks_of_CSI_Z + box_CsI_Z, fZeroRotation));
det->AddNode(box_CsI,1,new TGeoCombiTrans(-box_CsI_X,box_CsI_Y, blocks_of_CSI_Z + box_CsI_Z, fZeroRotation));
det->AddNode(box_CsI,1,new TGeoCombiTrans(box_CsI_X,box_CsI_Y + box_CsI_Y *2 , blocks_of_CSI_Z + box_CsI_Z, fZeroRotation));
det->AddNode(box_CsI,1,new TGeoCombiTrans(-box_CsI_X,box_CsI_Y + box_CsI_Y *2 , blocks_of_CSI_Z + box_CsI_Z, fZeroRotation));


top->AddNode(det, 1, new TGeoCombiTrans(global_X,global_Y,global_Z,fGlobalRotation));

// ---------------   Finish   -----------------------------------------------
gGeoMan->CloseGeometry();
gGeoMan->CheckOverlaps(0.001);
gGeoMan->PrintOverlaps();
gGeoMan->Test();

TFile* geoFile = new TFile(geoFileName, "RECREATE");
top->Write();
geoFile->Close();
// --------------------------------------------------------------------------
}
