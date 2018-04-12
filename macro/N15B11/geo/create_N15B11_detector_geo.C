void create_N15B11_detector_geo()
{
    // Create a zero rotation
    TGeoRotation *ggZeroRotation = new TGeoRotation("ggZeroRotation");

    // -------   Load media from media file   -----------------------------------
    FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/geometry/N15.media.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    // --------------------------------------------------------------------------

    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/geometry/N15B11_detector.geo.root";
    // --------------------------------------------------------------------------

    // --------------   Create geometry and top volume  -------------------------
    gGeoManager = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoManager->SetName("DETgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    // --------------------------------------------------------------------------


    // Materials and media --------------------------------------------------------------

    // Vacuum material
    TGeoMaterial* vacMat = new TGeoMaterial("vacuum", 1., 1., 1.);
    TGeoMedium* vacMed = new TGeoMedium("vacMed", 1, vacMat);

    // Set film material
    TGeoMaterial* filmMat = new TGeoMaterial("mylar", 1., 1., 1.);
    TGeoMedium* filmMed = new TGeoMedium("filmMed", 1, filmMat);

    // Set ionisation gass material
    TGeoMaterial* gasDetMat = new TGeoMaterial("isobutan3", 1., 1., 1.);
    TGeoMedium* gasDetMed = new TGeoMedium("gasDetMed", 1, gasDetMat);

    // Set detectors casing material
    TGeoMaterial* casDetMat = new TGeoMaterial("aluminium", 1., 1., 1.);
    TGeoMedium* casDetMed = new TGeoMedium("casDetMed", 1, casDetMat);

    // Set semiconductor material
    TGeoMaterial* semiDetMat = new TGeoMaterial("silicon", 1., 1., 1.);
    TGeoMedium* semiDetMed = new TGeoMedium("semiDetMed", 1, semiDetMat);

    // Shapes and volumes ---------------------------------------------------------------

    // Film (mylar)
    TGeoTube* sFilm = new TGeoTube("sFilm", 0., 0.5, 2e-4/2.);
    TGeoVolume* vFilm = new TGeoVolume("vFilm", sFilm, filmMed);

    // Detector cap
    TGeoTube* sTube = new TGeoTube("sTube", 0., 0.5, 0.2/2.);
    TGeoBBox* sBox = new TGeoBBox("sBox", 0.2/2., 0.4/2., 0.2/2.+0.1);
    TGeoCompositeShape* sCap = new TGeoCompositeShape("sCap", "sTube-sBox");
    TGeoVolume* vCap = new TGeoVolume("vCap", sCap, casDetMed);

    // Detector casing part
    TGeoCone* sDetCasingPart = new TGeoCone("sDetCasingPart", (5.-2e-4-0.2-0.4)/2., 0.5, 0.7, 1.5, 1.7);
    TGeoVolume* vDetCasingPart = new TGeoVolume("vDetCasingPart", sDetCasingPart, casDetMed);

    // Detector gas part
    TGeoCone* sDetGasPart = new TGeoCone("sDetGasPart", (5.-2e-4-0.2-0.4)/2., 0., 0.5, 0., 1.5);
    TGeoVolume* vDetGasPart = new TGeoVolume("vDetGasPart", sDetGasPart, gasDetMed);

    // Semiconductor
    TGeoTube* sSemi = new TGeoTube("sSemi", 0., 1.5, 0.4/2.);
    TGeoVolume* vSemi = new TGeoVolume("vSemi", sSemi, semiDetMed);

    // Container for 1 detector
    TGeoVolumeAssembly* vDetContainer = new TGeoVolumeAssembly("vDetContainer");

    TGeoVolumeAssembly* vOneLayer = new TGeoVolumeAssembly("vOneLayer");

    // Vacuum box for detector system
//    TGeoBBox* sVacBox = new TGeoBBox("sVacBox", 100./2., 100./2., 100./2.);
//    TGeoVolume* vVacBox = new TGeoVolume("vVacBox", sVacBox, vacMed);

    // Volume hierarchy -----------------------------------------------------------------

    TGeoTranslation* tFilm = new TGeoTranslation("tFilm", 0., 0., 2e-4/2.);
    TGeoTranslation* tCap = new TGeoTranslation("tCap", 0., 0., 2e-4+0.2/2.);
    TGeoTranslation* tGasAndCase = new TGeoTranslation("tGasAndCase", 0., 0., 2e-4+0.2+(-2e-4-0.2+5.-0.4)/2.);
    TGeoTranslation* tSemi = new TGeoTranslation("tSemi", 0., 0., 2e-4+0.2+(-2e-4-0.2+5.-0.4)+0.4/2.);

    /*vDetContainer->AddNode(vFilm, 1, tFilm);
    vDetContainer->AddNode(vCap, 1, tCap);*/
    vDetContainer->AddNode(vDetCasingPart, 1, tGasAndCase);
    vDetContainer->AddNode(vDetGasPart, 1, tGasAndCase);
    vDetContainer->AddNode(vSemi, 1, tSemi);

    const Int_t detNum = 23; // detectors number

    Double_t angles[23] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                           0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
    Double_t angleStep = 8.;
    Double_t startAngle = 90. - angleStep * (Double_t)((detNum-1)/2);

    for (UInt_t i=0; i<detNum; i++) {
        angles[i] = i*angleStep;
        //cout << "angle " << i << " = " << angles[i] << endl;
    }

    // Set vacuum cones positions
    for (UInt_t i = 0; i < detNum; i++) {
        TString rotCurDetName;
        rotCurDetName.Form("rotCurDet%d", i);
        TGeoRotation* rotCurDet = new TGeoRotation(rotCurDetName, 0., 0., 0.);

        Double_t curAngle = startAngle + angles[i] - 90.;
        rotCurDet->RotateY(curAngle);

        Double_t rotAngle = (curAngle) * TMath::Pi()/180.;
        Double_t distanceOfDet = 21.8;
        Double_t dx = distanceOfDet * sin(rotAngle);
        Double_t dz = distanceOfDet * cos(rotAngle);

        //cout << "i=" << i << ": angle=" << curAngle << "\tdx=" << dx << "\tdz=" << dz << endl;

        TString tcurDetName;
        tcurDetName.Form("tcurDet%d", i);
        TGeoCombiTrans* tcurDet = new TGeoCombiTrans(tcurDetName, dx, 0., dz, rotCurDet);
        tcurDet->RegisterYourself();

        vOneLayer->AddNode(vDetContainer, i+1, tcurDet);
    }

    top->AddNode(vOneLayer, 1);

/*
    TString rOneLayerName;
    Double_t startGamma = -80.;
    Double_t stepGamma = 20.;

    for (UInt_t iLayer=0; iLayer<9; iLayer++) {
        rOneLayerName.Form("rOneLayer_%d", iLayer);
        TGeoRotation* rOneLayer = new TGeoRotation(rOneLayerName, 0., 0., 0.);
        rOneLayer->RotateX(startGamma + iLayer*stepGamma);
        rOneLayer->RegisterYourself();
        top->AddNode(vOneLayer, iLayer+1, rOneLayer);
    }
*/

    // ---------------   Finish   -----------------------------------------------
    gGeoManager->SetTopVolume(top);
    gGeoManager->CloseGeometry();
    gGeoManager->CheckOverlaps(0.001);
    gGeoManager->PrintOverlaps();
    gGeoManager->CheckGeometry();
    //gGeoManager->CheckGeometryFull();

    //gGeoManager->GetTopVolume()->Draw("ogl");

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
    // --------------------------------------------------------------------------
}
