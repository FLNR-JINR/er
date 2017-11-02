void create_NeuRad_Wupper_Proto_geo()
{
	TString erPath = gSystem->Getenv("VMCWORKDIR");

	// Output paths
	TString outGeoFilenameRoot = erPath + "/geometry/NeuRad_Wupper_Proto.geo.root";
	TString outGeoFilenameGdml = erPath + "/geometry/NeuRad_Wupper_Proto.gdml";

	// Input paths
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

	TString mediumName;

	mediumName = "BC408";
	FairGeoMedium* mBC408 = geoMedia->getMedium(mediumName);
	if (!mBC408) Fatal("create_NeuRad_Wupper_Proto_geo", "FairMedium %s not found", mediumName.Data());
	geoBuild->createMedium(mBC408);
	TGeoMedium* pBC408 = geoM->GetMedium(mediumName);
	if (!pBC408) Fatal("create_NeuRad_Wupper_Proto_geo", "Medium %s not found", mediumName.Data());

	mediumName = "polypropylene";
	FairGeoMedium* mPolypropylene = geoMedia->getMedium(mediumName);
	if (!mPolypropylene) Fatal("create_NeuRad_Wupper_Proto_geo", "FairMedium %s not found", mediumName.Data());
	geoBuild->createMedium(mPolypropylene);
	TGeoMedium* pPolypropylene = geoM->GetMedium(mediumName);
	if (!pPolypropylene) Fatal("create_NeuRad_Wupper_Proto_geo", "Medium %s not found", mediumName.Data());
	
	mediumName = "Tefflon";
	FairGeoMedium* mTefflon = geoMedia->getMedium(mediumName);
	if (!mTefflon) Fatal("create_NeuRad_Wupper_Proto_geo", "FairMedium %s not found", mediumName.Data());
	geoBuild->createMedium(mTefflon);
	TGeoMedium* pTefflon = geoM->GetMedium(mediumName);
	if (!pTefflon) Fatal("create_NeuRad_Wupper_Proto_geo", "Medium %s not found", mediumName.Data());
	
	mediumName = "aluminium";
	FairGeoMedium* mAluminium = geoMedia->getMedium(mediumName);
	if (!mAluminium) Fatal("create_NeuRad_Wupper_Proto_geo", "FairMedium %s not found", mediumName.Data());
	geoBuild->createMedium(mAluminium);
	TGeoMedium* pAluminium = geoM->GetMedium(mediumName);
	if (!pAluminium) Fatal("create_NeuRad_Wupper_Proto_geo", "Medium %s not found", mediumName.Data());
	
	mediumName = "vacuum";
	FairGeoMedium* mVacuum = geoMedia->getMedium(mediumName);
	if (!mVacuum) Fatal("create_NeuRad_Wupper_Proto_geo", "FairMedium %s not found", mediumName.Data());
	geoBuild->createMedium(mVacuum);
	TGeoMedium* pVacuum = geoM->GetMedium(mediumName);
	if (!pVacuum) Fatal("create_NeuRad_Wupper_Proto_geo", "Medium %s not found", mediumName.Data());

	// General dimensions
	Double_t fiberXsize = 0.3; // cm. 0.3 cm. This is actually not the size of the fiber but the fize of the fiber with the cladding around it
	Double_t fiberYsize = 0.3; // cm. 0.3 cm. This is actually not the size of the fiber but the fize of the fiber with the cladding around it
	Double_t fiberZsize = 25.; // cm. 25 cm
	Double_t cladding_thickness = 0.005; // cm. 0.005 cm. non-sensitive layer between fibers (cladidng and painting)
	Double_t subm_wrap_thickness = 0.01; // cm. 0.01 cm. teflon wrapping around submodule (8x8 fibers assembly)
	Double_t housing_thickness = 0.01; // cm. 0.01 cm. paper wrapping (housing) around module
	Double_t ersatz_pmt_lenZ = 0.24; // cm. 0.24 cm. simulation of the overall pmt material
	Double_t cover_lenZ = 0.1; // cm. 0.1 cm. simulation of the plastic cover at the opposite (w.r.t PMT) end 

	// Shapes
	TGeoBBox* fiberShape = new TGeoBBox("fiberShape", fiberXsize/2.-cladding_thickness,
	                                                  fiberYsize/2.-cladding_thickness,
	                                                  fiberZsize/2.);
	TGeoBBox* claddingShape = new TGeoBBox("claddingShape", fiberXsize/2.,
	                                                        fiberYsize/2.,
	                                                        fiberZsize/2.);
	TGeoBBox* pixelShape = new TGeoBBox("pixelShape", (2.*fiberXsize)/2.,
	                                                  (2.*fiberYsize)/2.,
	                                                  fiberZsize/2.);
	TGeoBBox* submoduleShape = new TGeoBBox("submoduleShape", (8.*fiberXsize+2.*subm_wrap_thickness)/2.,
	                                                          (8.*fiberYsize+2.*subm_wrap_thickness)/2.,
	                                                          fiberZsize/2.);
	/*TGeoBBox* moduleShape = new TGeoBBox("moduleShape", 2.*(8.*fiberXsize+2.*subm_wrap_thickness)/2.,
	                                                    2.*(8.*fiberYsize+2.*subm_wrap_thickness)/2.,
	                                                    fiberZsize/2.);*/

	// Volumes
	TGeoVolume* fiberVol = new TGeoVolume("fiberVol", fiberShape, pBC408);
	TGeoVolume* claddingVol = new TGeoVolume("claddingVol", claddingShape, pBC408);
	TGeoVolume* pixelVol = new TGeoVolume("pixelVol", pixelShape, pVacuum);
	TGeoVolume* submoduleVol = new TGeoVolume("submoduleVol", submoduleShape, pTefflon);
	TGeoVolume* moduleVol = new TGeoVolumeAssembly("moduleVol"/*, moduleShape, pTefflon*/);

	// Matrices
	TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
	rotNoRot->RegisterYourself();

	// Structure
	// Fiber in cladding
	claddingVol->AddNode(fiberVol, 1);

	TString matrixName;

	// 2*2 claddings in pixel
	for (UInt_t iX=0; iX<2; iX++) {
		for (UInt_t iY=0; iY<2; iY++) {
			matrixName.Form("mCladdingInPixel_%d", iX*2+iY);
			pixelVol->AddNode(claddingVol, iX*2+iY, new TGeoCombiTrans(matrixName,
			    -fiberXsize/2.+fiberXsize*iX, -fiberYsize/2.+fiberYsize*iY, 0., rotNoRot));
		}
	}
	
	// 4*4 pixels in submodule
	Double_t pixelXsize = 2.*fiberXsize;
	Double_t pixelYsize = 2.*fiberYsize;

	for (UInt_t iX=0; iX<4; iX++) {
		for (UInt_t iY=0; iY<4; iY++) {
			matrixName.Form("mPixelInSubmodule_%d", iX*4+iY);
			submoduleVol->AddNode(pixelVol, iX*4+iY, new TGeoCombiTrans(matrixName,
			    -3.*pixelXsize/2.+pixelXsize*iX, -3.*pixelYsize/2.+pixelYsize*iY, 0., rotNoRot));
		}
	}

	// 2*2 submodules in module
	Double_t submoduleXsize = 8.*fiberXsize + 2.*subm_wrap_thickness;
	Double_t submoduleYsize = 8.*fiberYsize + 2.*subm_wrap_thickness;

	for (UInt_t iX=0; iX<2; iX++) {
		for (UInt_t iY=0; iY<2; iY++) {
			matrixName.Form("mSubmoduleInModule_%d", iX*2+iY);
			moduleVol->AddNode(submoduleVol, iX*2+iY, new TGeoCombiTrans(matrixName,
			    -submoduleXsize/2.+submoduleXsize*iX, -submoduleYsize/2.+submoduleYsize*iY, 0., rotNoRot));
		}
	}

	Double_t moduleXsize = 2.*submoduleXsize + 2.*housing_thickness;
	Double_t moduleYsize = 2.*submoduleYsize + 2.*housing_thickness;

	// Housing, Ersatz-PMT, Cover
	// Shapes
	TGeoBBox* housingOp1Shape = new TGeoBBox("housingOp1Shape", moduleXsize/2., moduleYsize/2., fiberZsize/2.);
	TGeoBBox* housingOp2Shape = new TGeoBBox("housingOp2Shape", moduleXsize/2.-housing_thickness, moduleYsize/2.-housing_thickness, fiberZsize/2.);
	TGeoCompositeShape* housingShape = new TGeoCompositeShape("housingShape","housingOp1Shape-housingOp2Shape");
	TGeoBBox* ersatzPmtShape = new TGeoBBox("ersatzPmtShape", moduleXsize/2., moduleYsize/2., ersatz_pmt_lenZ/2.);
	TGeoBBox* coverShape = new TGeoBBox("coverShape", moduleXsize/2., moduleYsize/2., cover_lenZ/2.);
	//Volumes
	TGeoVolume* housingVol = new TGeoVolume("housingVol", housingShape, pPolypropylene);
	TGeoVolume* ersatzPmtVol = new TGeoVolume("ersatzPmtVol", ersatzPmtShape, pAluminium);
	TGeoVolume* coverVol = new TGeoVolume("coverVol", coverShape, pPolypropylene);

	// World ------------------------------------
	TGeoVolume* topVol = new TGeoVolumeAssembly("TOP");

	// Structure
	topVol->AddNode(moduleVol, 1);
	topVol->AddNode(housingVol, 1);
	topVol->AddNode(ersatzPmtVol, 1, new TGeoCombiTrans(0., 0., (fiberZsize+ersatz_pmt_lenZ)/2., rotNoRot));
	topVol->AddNode(coverVol, 1, new TGeoCombiTrans(0., 0., -(fiberZsize+cover_lenZ)/2., rotNoRot));

	// Finalize
	geoM->SetTopVolume(topVol);
	geoM->CloseGeometry();
	geoM->CheckOverlaps();
	geoM->CheckGeometry();
	geoM->CheckGeometryFull();

	// Export
	geoM->Export(outGeoFilenameGdml);

	TFile* outGeoFileRoot = new TFile(outGeoFilenameRoot, "RECREATE");
	geoM->GetTopVolume()->Write();
	outGeoFileRoot->Close();

	// Draw
	TBrowser* bro = new TBrowser("bro", "bro");
	geoM->GetTopVolume()->Draw("ogl");
}
