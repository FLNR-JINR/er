/**************************************************************

Dead zone includes all 'normal' volumes inside.

sensor x16 different types - sector (1) x16 same - dead zone (1) - top
sector (2) x16 same - dead zone (2) - top
crystal - crystalArray x16 same - top

**************************************************************/
#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_RTelescope_geo_v3()
{
	// Create geometry manager
	//TGeoManager* gGeoManager = new TGeoManager("geom", "geom");

	FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/geometry/media.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    TGeoManager* gGeoMan = gGeoManager;
    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/geometry/RTelescope.v3.geo.root";

	// Common stuff
    FairGeoMedia*   geoMedia = geoFace->getMedia();
    FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

    FairGeoMedium* mpSi      = geoMedia->getMedium("silicon");
    if ( ! mpSi ) Fatal("Main", "FairMedium silicon not found");
    geoBuild->createMedium(mpSi);
    TGeoMedium* pSi = gGeoMan->GetMedium("silicon");
    if ( ! pSi ) Fatal("Main", "Medium silicon not found");

    FairGeoMedium* mpAir      = geoMedia->getMedium("vacuum");
    if ( ! mpAir ) Fatal("Main", "FairMedium vacuum not found");
    geoBuild->createMedium(mpAir);
    TGeoMedium* pAir= gGeoMan->GetMedium("vacuum");
    if ( ! pAir ) Fatal("Main", "Medium vacuum not found");

    FairGeoMedium* mpCsI      = geoMedia->getMedium("CsI");
    if ( ! mpCsI ) Fatal("Main", "FairMedium Csi not found");
    geoBuild->createMedium(mpCsI);
    TGeoMedium* pCsI = gGeoMan->GetMedium("CsI");
    if ( ! pCsI ) Fatal("Main", "Medium CsI not found");

	TGeoTranslation* zeroTrans = new TGeoTranslation("zeroTrans", 0., 0., 0.);
	TGeoRotation* zeroRot = new TGeoRotation("zeroRot", 0., 0., 0.);
	TGeoCombiTrans* zeroCombitrans = new TGeoCombiTrans(*zeroTrans, *zeroRot);

	// ================================================================
	// All dimension in cm
	Double_t R_min = 1.6;
	Double_t R_max = 4.2;
	Double_t deltaR = (R_max-R_min)/32.;

	// ================================================================
	// Ring1

	Double_t thickness1 = 0.0285;
	Double_t thickness_dead_before1 = 0.0001;
	Double_t thickness_dead_after1 = 0.00015;
	Double_t addendR_dead_inner1 = 0.05;
	Double_t addendR_dead_outer1 = 0.05;
	Float_t rsp_min = 2000.;
    Float_t rsp_max = rsp_min + thickness1;
    Float_t thsp_min = TMath::ATan(R_min/rsp_min)*TMath::RadToDeg();
    Float_t thsp_max = TMath::ATan(R_max/rsp_max)*TMath::RadToDeg();

	// Sensors
	TString sensorName;
	TGeoVolume* sensor_ring1R1[32];
	TGeoVolume* sensor_ring1R2[32];
	Float_t thsp_min1;
    Float_t thsp_max1;

	for (UInt_t iSensor=0; iSensor<32; iSensor++) {
		sensorName.Form("sensor_ring1R1_type%d", iSensor);
		//sensor_ring1R1[iSensor] = gGeoMan->MakeTubs(sensorName, pSi,
			//R_min+iSensor*deltaR, R_min+(iSensor+1)*deltaR, thickness1/2., 0., 360./32.);
		//sensor_ring1R2[iSensor] = gGeoMan->MakeTubs(sensorName, pSi,
			//R_min+iSensor*deltaR, R_min+(iSensor+1)*deltaR, thickness1/2., 0., 360./32.);
		 thsp_min1 = TMath::ATan((R_min+iSensor*deltaR)/rsp_min)*TMath::RadToDeg();
		 thsp_max1 = TMath::ATan((R_min+(iSensor+1)*deltaR)/rsp_max)*TMath::RadToDeg();
		 sensor_ring1R1[iSensor]= gGeoMan->MakeSphere(sensorName, pSi,rsp_min,rsp_max,thsp_min1,thsp_max1,0,360./32.);
		 sensor_ring1R2[iSensor]= gGeoMan->MakeSphere(sensorName, pSi,rsp_min,rsp_max,thsp_min1,thsp_max1,0,360./32.);
	}

	// Sectors
	//TGeoVolume* sector_ring1R1 = gGeoMan->MakeTubs("sector_ring1R1", pSi,
		//R_min, R_max, thickness1/2., 0., 360./32.);
	//TGeoVolume* sector_ring1R2 = gGeoMan->MakeTubs("sector_ring1R2", pSi,
		//R_min, R_max, thickness1/2., 0., 360./32.);
	TGeoVolume* sector_ring1R1 = gGeoMan->MakeSphere("sector_ring1R1",pSi,rsp_min,rsp_max,thsp_min,thsp_max,0,360./32.);
	TGeoVolume* sector_ring1R2 = gGeoMan->MakeSphere("sector_ring1R2",pSi,rsp_min,rsp_max,thsp_min,thsp_max,0,360./32.);

	// Positioning sensors inside the sector
	for (UInt_t iSensor=0; iSensor<32; iSensor++) {
		sector_ring1R1->AddNode(sensor_ring1R1[iSensor], iSensor +1 , zeroCombitrans);
		sector_ring1R2->AddNode(sensor_ring1R2[iSensor], iSensor+ 1 , zeroCombitrans);
	}

	// Dead ring
	Double_t R_min1 = R_min - addendR_dead_inner1;
	Double_t R_max1 = R_max + addendR_dead_outer1;
	Float_t rsp_max1 = rsp_min + thickness_dead_before1 + thickness1 + thickness_dead_after1;
    Float_t thsp_min2 = TMath::ATan(R_min1/rsp_min)*TMath::RadToDeg();
    Float_t thsp_max2 = TMath::ATan(R_max1/rsp_max1)*TMath::RadToDeg();
	TGeoVolume* dead_ring1R1 = gGeoMan->MakeSphere("ring1R1_dead_zone",pSi,rsp_min,rsp_max1,thsp_min2,thsp_max2,0.,360.);
    TGeoVolume* dead_ring1R2 = gGeoMan->MakeSphere("ring1R2_dead_zone",pSi,rsp_min,rsp_max1,thsp_min2,thsp_max2,0.,360.);
	// Positioning sectors inside the ring
	TString curRotName;
	for (UInt_t iSector=0; iSector<32; iSector++)
	{
		curRotName.Form("SectorRot1_%d", iSector);
		TGeoRotation* curRot = new TGeoRotation(curRotName, 0., 0., (360./32.)*iSector);
		TGeoCombiTrans* curCombitrans = new TGeoCombiTrans(*zeroTrans, *curRot);
		dead_ring1R1->AddNode(sector_ring1R1, iSector+1, curCombitrans); //TODO check index - from 0 or from 1
		dead_ring1R2->AddNode(sector_ring1R2, iSector+1, curCombitrans);
	}

	// ================================================================
	// Ring2
	Double_t thickness2 = 0.1;
	Double_t thickness_dead_before2 = 0.0001;
	Double_t thickness_dead_after2 = 0.00015;
	Double_t addendR_dead_inner2 = 0.05;
	Double_t addendR_dead_outer2 = 0.05;
	Float_t rsp_max2 = rsp_min + thickness2;
    Float_t thsp_min3 = TMath::ATan(R_min/rsp_min)*TMath::RadToDeg();
    Float_t thsp_max3 = TMath::ATan(R_max/rsp_max2)*TMath::RadToDeg();

	// Sectors
	TGeoVolume* sector_ring2R1 = gGeoMan->MakeSphere("sector_ring2R1",pSi,rsp_min,rsp_max2,thsp_min3,thsp_max3,0,360./32.);
	TGeoVolume* sector_ring2R2 = gGeoMan->MakeSphere("sector_ring2R2",pSi,rsp_min,rsp_max2,thsp_min3,thsp_max3,0,360./32.);

	// Dead ring
	Double_t R_min2 = R_min - addendR_dead_inner2;
	Double_t R_max2 = R_max + addendR_dead_outer2;
	Float_t rsp_max3 = rsp_min + thickness_dead_before2 + thickness2 + thickness_dead_after2;
    Float_t thsp_min4 = TMath::ATan(R_min2/rsp_min)*TMath::RadToDeg();
    Float_t thsp_max4 = TMath::ATan(R_max2/rsp_max3)*TMath::RadToDeg();
	//TGeoVolume* dead_ring2R1 = gGeoMan->MakeTubs("ring2R1_dead_zone", pSi,
	//	R_min-addendR_dead_inner2, R_max+addendR_dead_outer2,
	//	(thickness_dead_before2 + thickness2 + thickness_dead_after2)/2., 0., 360.);
	//TGeoVolume* dead_ring2R2 = gGeoMan->MakeTubs("ring2R2_dead_zone", pSi,
	//	R_min-addendR_dead_inner2, R_max+addendR_dead_outer2,
	//	(thickness_dead_before2 + thickness2 + thickness_dead_after2)/2., 0., 360.);
	TGeoVolume* dead_ring2R1 = gGeoMan->MakeSphere("ring2R1_dead_zone",pSi,rsp_min,rsp_max3,thsp_min4,thsp_max4,0.,360.);
    TGeoVolume* dead_ring2R2 = gGeoMan->MakeSphere("ring2R2_dead_zone",pSi,rsp_min,rsp_max3,thsp_min4,thsp_max4,0,360.);


	// Positioning sectors inside the ring
	for (UInt_t iSector=0; iSector<32; iSector++)
	{
		curRotName.Form("SectorRot2_%d", iSector);
		TGeoRotation* curRot = new TGeoRotation(curRotName, 0., 0., (360./32.)*iSector);
		TGeoCombiTrans* curCombitrans = new TGeoCombiTrans(*zeroTrans, *curRot);
		dead_ring2R1->AddNode(sector_ring2R1, iSector+1, curCombitrans); //TODO check index - from 0 or from 1
		dead_ring2R2->AddNode(sector_ring2R2, iSector+1, curCombitrans);
	}

	// ================================================================
	// Dead crystall

	Double_t t1_dx1 = 0.556737;
	Double_t t1_dx2 = 1.99186;
	Double_t t1_dy = 1.9;
	Double_t t1_dz = 3.60742634;
	Double_t t2_dx1 = 1.99186;
	Double_t t2_dx2 = 1.02;
	Double_t t2_dy1 = 1.9;
	Double_t t2_dy2 = 1.02;
	Double_t t2_dz = 0.8763;

    //CsI crystall
	thickness_dead = 0.0014;
	Double_t cr1_dx1 = t1_dx1 - thickness_dead*2;
	Double_t cr1_dx2 = t1_dx2 - thickness_dead*2;
	Double_t cr1_dy = t1_dy - thickness_dead*2;
	Double_t cr1_dz = t1_dz;
	Double_t cr2_dx1 = t2_dx1 - thickness_dead*2;
	Double_t cr2_dx2 = t2_dx2 - thickness_dead*2;
	Double_t cr2_dy1 = t2_dy1 - thickness_dead*2;
	Double_t cr2_dy2 = t2_dy2 - thickness_dead*2;
	Double_t cr2_dz = t2_dz;


	// Complex crystal shape
	TGeoTrd1* trd1 = new TGeoTrd1("trd1", t1_dx1/2., t1_dx2/2., t1_dy/2., t1_dz/2.);
	TGeoTrd2* trd2 = new TGeoTrd2("trd2", t2_dx1/2., t2_dx2/2., t2_dy1/2., t2_dy2/2., t2_dz/2.);
	TGeoTrd1* cr1 = new TGeoTrd1("cr1", cr1_dx1/2., cr1_dx2/2., cr1_dy/2., cr1_dz/2.);
	TGeoTrd2* cr2 = new TGeoTrd2("cr2", cr2_dx1/2., cr2_dx2/2., cr2_dy1/2., cr2_dy2/2., cr2_dz/2.);
	TGeoCombiTrans* combitransOp2 = new TGeoCombiTrans("M", 0., 0., (t1_dz+t2_dz)/2., zeroRot);
	TGeoCombiTrans* combitransOp3 = new TGeoCombiTrans("P", 0., 0., (cr1_dz+cr2_dz)/2., zeroRot);
	combitransOp2->RegisterYourself();
	combitransOp3->RegisterYourself();
	TGeoCompositeShape* crystalShapeDead = new TGeoCompositeShape("crystalShapeDead", "trd1+trd2:M");
	TGeoCompositeShape* crystalShape = new TGeoCompositeShape("crystalShape", "cr1+cr2:P");
	TGeoVolume* crystalVolDeadR1 = new TGeoVolume("crDeadR1", crystalShapeDead, pSi);
	TGeoVolume* crystalVolR1 = new TGeoVolume("crystallR1", crystalShape, pCsI);
	TGeoVolume* crystalVolDeadR2 = new TGeoVolume("crDeadR2", crystalShapeDead, pSi);
	TGeoVolume* crystalVolR2 = new TGeoVolume("crystallR2", crystalShape, pCsI);


	Double_t alpha = 11.25;
	Double_t x = (t1_dx1/2.) / TMath::Tan(alpha*TMath::DegToRad());
	Double_t radi = x + (t1_dz/2.);

	// crystalArray container shape
	// Inner radius is 1mm less that it should be
	// to avoid overlaps due to mathematical errors

	TGeoVolume* CsIarrayR1 = gGeoMan->MakeTubs("crystalArrayR1", pAir,
		x-0.1, (x+t1_dz+t2_dz)+0.1, 1.9/2., 0., 360.);
	TGeoVolume* CsIarrayR2 = gGeoMan->MakeTubs("crystalArrayR2", pAir,
		x-0.1, (x+t1_dz+t2_dz)+0.1, 1.9/2., 0., 360.);

	// Positioning crystals inside the crystalArray container
	crystalVolDeadR1->AddNode(crystalVolR1, 1, zeroCombitrans);
	crystalVolDeadR2->AddNode(crystalVolR2, 1, zeroCombitrans);
	TString curTransName;
	for (UInt_t iCrystal=0; iCrystal<16; iCrystal++)
	{
		curTransName.Form("CrystalTrans%d", iCrystal);
		curRotName.Form("CrystalRot%d", iCrystal);

		TGeoTranslation* curTrans = new TGeoTranslation(curTransName,
			radi*TMath::Sin(2.*alpha*TMath::DegToRad()*iCrystal),
			-radi*TMath::Cos(2.*alpha*TMath::DegToRad()*iCrystal),
			0.);

		TGeoRotation* curRot = new TGeoRotation(curRotName);
		curRot->RotateX(90.);
		curRot->RotateZ(2.*alpha*iCrystal);

		TGeoCombiTrans* curCombitrans = new TGeoCombiTrans(*curTrans, *curRot);

        //crystalVolDead->AddNode(crystalVol, iCrystal+1, zeroCombitrans);
		CsIarrayR1->AddNode(crystalVolDeadR1, iCrystal+1, curCombitrans);
		CsIarrayR2->AddNode(crystalVolDeadR2, iCrystal+1, curCombitrans);
	}

	// ================================================================
	// Top volume
	gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoMan->SetName("DETgeom");
	TGeoVolume* top = new TGeoVolumeAssembly("TOP");
	gGeoMan->SetTopVolume(top);
	TGeoVolume* RTelescopes = new TGeoVolumeAssembly("RTelescopes");
	TGeoVolume* RTelescope1 = new TGeoVolumeAssembly("RTelescope1");
	TGeoVolume* RTelescope2 = new TGeoVolumeAssembly("RTelescope2");


	// Positioning ring1, ring2 and crystalArray inside top
	TGeoTranslation* ring1Trans = new TGeoTranslation(0., 0., -2000.);
	TGeoCombiTrans* ring1Combitrans = new TGeoCombiTrans(*ring1Trans, *zeroRot);
	RTelescope1->AddNode(dead_ring1R1, 1, ring1Combitrans);
    RTelescope2->AddNode(dead_ring1R2, 1, ring1Combitrans);

	TGeoTranslation* ring2Trans = new TGeoTranslation(0., 0., -2001.);
	TGeoCombiTrans* ring2Combitrans = new TGeoCombiTrans(*ring2Trans, *zeroRot);
	RTelescope1->AddNode(dead_ring2R1, 2, ring2Combitrans);
    RTelescope2->AddNode(dead_ring2R2, 2, ring2Combitrans);

	TGeoTranslation* CsIarrayTrans = new TGeoTranslation(0., 0., 0.1 + t1_dy + thickness2 - 1);
	TGeoCombiTrans* CsIarrayCombitrans = new TGeoCombiTrans(*CsIarrayTrans, *zeroRot);
	RTelescope1->AddNode(CsIarrayR1, 3, CsIarrayCombitrans);
    RTelescope2->AddNode(CsIarrayR2, 3, CsIarrayCombitrans);

    TGeoTranslation* rtelescope1Trans = new TGeoTranslation(0., 0., 0.);
    TGeoCombiTrans* rtelescope1Combitrans = new TGeoCombiTrans(*rtelescope1Trans, *zeroRot);
    RTelescopes->AddNode(RTelescope1, 1, rtelescope1Combitrans);

    TGeoTranslation* rtelescope2Trans = new TGeoTranslation(0., 0., 7.);
    TGeoCombiTrans* rtelescope2Combitrans = new TGeoCombiTrans(*rtelescope2Trans, *zeroRot);
    RTelescopes->AddNode(RTelescope2, 2, rtelescope2Combitrans);

    top->AddNode(RTelescopes, 1, zeroCombitrans);
	// ================================================================
	// Finalize, perform checks
	gGeoMan->CloseGeometry();
	//gGeoMan->CheckGeometryFull();
	gGeoMan->CheckGeometry();
	gGeoMan->CheckOverlaps();
	gGeoMan->PrintOverlaps();
	top->Draw();

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();

	// ================================================================
	// Export
	//gGeoMan->Export("geoManager.root"); // Export full geometry manager
	//gGeoMan->GetTopVolume()->Export("topVolume.root"); // Export only volumes

	// ================================================================
	//TBrowser* bro = new TBrowser("bro", "bro", 1400, 900);
	//gGeoMan->GetTopVolume()->Draw();
}
