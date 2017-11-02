// All dimensions in cm
// Я добавляю у размеров цилиндров по одному сантиметру и выдвигаю их из бокса соответственно
// на 0.5 см чтобы не было совпадающих граней и хорошо рисовалось в openGL.
// А тоненький цилиндр делаю ещё чуть длиннее, чтобы хорошо рисовался стык цилиндров.

void create_collimator_geo()
{
	TString erPath = gSystem->Getenv("VMCWORKDIR");

	// Output paths
	TString outGeoFilenameRoot = erPath + "/geometry/collimator.geo.root";
	TString outGeoFilenameGdml = erPath + "/geometry/collimator.gdml";

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
	mediumName = "lead";
	FairGeoMedium* mFe = geoMedia->getMedium(mediumName);
	if (!mFe) Fatal("create_collimator_geo", "FairMedium %s not found", mediumName.Data());
	geoBuild->createMedium(mFe);
	TGeoMedium* pFe = geoM->GetMedium(mediumName);
	if (!pFe) Fatal("create_collimator_geo", "Medium %s not found", mediumName.Data());

	// Collimator -------------------------------
	// Operands shapes
	TGeoBBox* boxShape = new TGeoBBox("boxShape", 4.5/2., 9./2., 9./2.);
	TGeoTube* tube1Shape = new TGeoTube("tube1Shape", 0., 0.6, 3./2.+1./2.);
	TGeoTube* tube2Shape = new TGeoTube("tube2Shape", 0., 0.2, 6./2.+1./2.+1./2.);

	// Operands matrices
	TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
	rotNoRot->RegisterYourself();
	TGeoCombiTrans* M1 = new TGeoCombiTrans("M1", 0., 0., -3.-1./2., rotNoRot);
	TGeoCombiTrans* M2 = new TGeoCombiTrans("M2", 0., 0., 1.5+1./2., rotNoRot);
	M1->RegisterYourself();
	M2->RegisterYourself();

	// Third operand - rotated cylinder - false hole
	Double_t alpha = TMath::ATan(3./30.)*180./TMath::Pi();
	TGeoRotation* rotAlpha = new TGeoRotation("rotAlpha", 0., 0., 0.);
	rotAlpha->RotateY(alpha);
	rotAlpha->RegisterYourself();
	Double_t shift_X = 0.15;
	Double_t shift_Z = 1.5+1.5;
	Double_t tube3len = TMath::Sqrt(3.*3.+0.3*0.3);
	TGeoCombiTrans* M3 = new TGeoCombiTrans("M3", shift_X, 0., shift_Z, rotAlpha);
	M3->RegisterYourself();
	TGeoTube* tube3Shape = new TGeoTube("tube3Shape", 0., 0.2, tube3len/2.+0.2); // +0.2 to be on the safe side

	// Shape
	TGeoCompositeShape* collimatorShape = new TGeoCompositeShape("collimatorShape",
		"boxShape-tube1Shape:M1-tube2Shape:M2-tube3Shape:M3");

	// Volume
	TGeoVolume* collimatorVol = new TGeoVolume("collimatorVol", collimatorShape, pFe);

	// World ------------------------------------
	TGeoVolume* topVol = new TGeoVolumeAssembly("TOP");

	// Structure
	topVol->AddNode(collimatorVol, 1);

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
