
void sim_digi (Int_t nEvents = 10000) {
//----------------------------------
  Double_t BeamDetLToF = 1232.0;     // [cm] 12348
  Double_t BeamDetPosZToF = -95.3;  // [cm] 
  Double_t BeamDetPosZ1MWPC = -81.5;     // [cm]
  Double_t BeamDetPosZ2MWPC = -27.;  // [cm]  
  // --------------- Beam start position ------------------------------------
  Double_t beamStartPosition = -1600.;  // [cm]
  // --------------- Target -------------------------------------------------
  Double_t targetD2Thickness = 0.6;  // [cm] this parameter should coincide with target H2 thickness in /macro/geo/create_target_D2_geo.C
  // Double_t targetD2Thickness = 0.42;  // [cm] this parameter should coincide with target H2 thickness in /macro/geo/create_target_D2_geo.C
  //---------------------Files-----------------------------------------------
//  TString outFile= "sim_digi_mylar.root";
//  TString outFile= "sim_digi_Steel.root";
  TString outFile= "sim_digi.root";
  TString datFile= "new_10he_0p_r.dat";

//  TString outFile= "sim_digi_100eV_Steel.root";
//  TString datFile= "new_10he_0p_100eV.dat";
//  TString outFile= "sim_digi_1eV_Steel.root";
//  TString datFile= "new_10he_0p_1eV.dat";
  
  TString parFile= "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
 // TString paramFileQTelescope = workDirPath
 //                        + "/db/QTelescope/QTelescopeParts3.xml";
  TString paramFileQTelescope = workDirPath 
						 + "/db/QTelescope/QTelescopeParts_10he.xml";
  // TString paramFileBeamDet = workDirPath
                         // + "/db/BeamDet/BeamDetParts.xml";
	TString paramFileBeamDet = workDirPath
                         + "/db/BeamDet/BeamDetParts_10he.xml";
  // TString targetGeoFileName = workDirPath + "/geometry/target.T.gas.root";
  // TString targetGeoFileName = workDirPath + "/geometry/target.D2.gas.root";
  
  
  // TString targetGeoFileName = workDirPath + "/geometry/target.T.D2.gas.root";
  // TString interactionVol = "tubeD2";
//  TString targetGeoFileName = workDirPath + "/geometry/target.3h_mylar.geo.root";
  TString targetGeoFileName = workDirPath + "/geometry/target.3h_Steel.geo.root";
  // TString targetGeoFileName = workDirPath + "/geometry/target.3h_thin.geo.root";
  TString interactionVol = "target3HVol";

  TString ndGeoFileName = workDirPath + "/geometry/ND.geo.root";
    
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer; 
  timer.Start();
  // ------------------------------------------------------------------------
  // -----   Create simulation run   ----------------------------------------
  ERRunSim* run = new ERRunSim();
  /** Select transport engine
  * TGeant3
  * TGeant4exp1904_sim_digi.C
  **/
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());          // Output file
  // ------------------------------------------------------------------------
  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  //-------- Set MC event header --------------------------------------------
  ER10Heto8HeEventHeader* decayMCheader = new ER10Heto8HeEventHeader();
  run->SetMCEventHeader(decayMCheader);
  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // -----   Create detectors  ----------------------------------------------   
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);
   
  Int_t verbose = 0;
  // -----  BeamDet Setup ---------------------------------------------------
  ERBeamDetSetup* setupBeamDet = ERBeamDetSetup::Instance();
  setupBeamDet->SetXmlParametersFile(paramFileBeamDet);

  // -----  BeamDet parameters ----------------------------------------------
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF - BeamDetLToF);     
  setupBeamDet->AddToF("ToF2", BeamDetPosZToF);    //  BeamDet parts should be added in ascending order   
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZ1MWPC);   //  of Z-coordinate of part.
  setupBeamDet->SetMWPCnumberingInvOrderX(); 

  setupBeamDet->AddMWPC("MWPC2", BeamDetPosZ2MWPC);    	
  setupBeamDet->SetMWPCnumberingInvOrderX(); 

  //setupBeamDet->SetSensitiveTarget();

  // -----   Create target  -------------------------------------------------
  FairModule* target = new ERTarget("target", kTRUE, 1);
  target->SetGeometryFileName(targetGeoFileName);
  run->AddModule(target);

  // -----  QTelescope Setup ------------------------------------------------
  ERTelescopeSetup* setupQTelescope = ERTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeTmp");

  // ----- instead of annular parameters ----------------------------------------------------
  Double_t xPos, yPos, zPos;
  // xPos = 0.125;
  xPos = 0.;
  yPos = 0.;
  // yPos = 3.;
  // yPos = 0.;
  zPos = -10.05;
  TVector3 rotationC(0., 0., 0.);
  
  ERGeoSubAssembly* assembly_proton= new ERGeoSubAssembly("Telescope_proton", TVector3(xPos, yPos, zPos), rotationC);  
  
  ERRTelescopeGeoComponentDoubleSi* det_proton = new ERRTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_R", 
                                                                                    TVector3(0., 0., 0.), TVector3(), "X");
  
  // ERQTelescopeGeoComponentDoubleSi* det_proton = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_DSD", 
                                                                                  // TVector3(0., 0., 0.), TVector3(), "X");  
  assembly_proton->AddComponent(det_proton);

  setupQTelescope->AddSubAssembly(assembly_proton);

  // ----- square telescope parameters ----------------------------------------------------
  Double_t x,y,z_d1,z_d2,z;
  x = 0.;
  y = 0.;
  z = 25.15;
  z_d1 = 0.75;
  z_d2 = 1.05;
  TVector3 fZeroRotation(0., 0., 0.);
  //координаты детекторов:
  // -10.05 см - круглый телескоп
  //  25.15  25.9  26.95  28  29.05  30.1 см - квадратный телескоп
  ERGeoSubAssembly* assembly_he8 = new ERGeoSubAssembly("Telescope_he8", TVector3(x, y, z), fZeroRotation);
  ERQTelescopeGeoComponentSingleSi* det_he8_X = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "X");
  ERQTelescopeGeoComponentSingleSi* det_he8_Y1 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_1", 
                                                                                  TVector3(0., 0., z_d1), TVector3(), "Y");
  ERQTelescopeGeoComponentSingleSi* det_he8_Y2 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_2", 
                                                                                  TVector3(0., 0., z_d1+z_d2), TVector3(), "Y");
  ERQTelescopeGeoComponentSingleSi* det_he8_Y3 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_3", 
                                                                                  TVector3(0., 0., z_d1+z_d2*2), TVector3(), "Y");
  ERQTelescopeGeoComponentSingleSi* det_he8_Y4 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_4", 
                                                                                  TVector3(0., 0., z_d1+z_d2*3), TVector3(), "Y");
  ERQTelescopeGeoComponentSingleSi* det_he8_Y5 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_5", 
                                                                                  TVector3(0., 0., z_d1+z_d2*4), TVector3(), "Y");																				  

  assembly_he8->AddComponent(det_he8_X);
  assembly_he8->AddComponent(det_he8_Y1);
  assembly_he8->AddComponent(det_he8_Y2);
  assembly_he8->AddComponent(det_he8_Y3);
  assembly_he8->AddComponent(det_he8_Y4);
  assembly_he8->AddComponent(det_he8_Y5);

  setupQTelescope->AddSubAssembly(assembly_he8);

 
  // ------QTelescope -------------------------------------------------------
  ERTelescope* qtelescope= new ERTelescope("ERTelescope", kTRUE,verbose);
  run->AddModule(qtelescope);
  // ------BeamDet ----------------------------------------------------------
  ERBeamDet* beamdet= new ERBeamDet("ERBeamDet", kTRUE,verbose);
  run->AddModule(beamdet);
  // ------ND ---------------------------------------------------------------
  ERND* nd= new ERND("ERND", kTRUE,verbose);
  nd->SetGeometryFileName(ndGeoFileName);
  // run->AddModule(nd);
  //-------------------------------------------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  Double_t  kinE_MevPerNucleon = 21.5;

  Int_t Z = 2, A = 8, Q = 2;
  TString ionName = "8He";
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  // FairIon *ion8He = generator -> Ion();
  // printf("\n\n massHe8 = %f \n\n",ion8He->GetMass());
  
 // generator->AddBackgroundIon("Triton",1,3,1,0.1);
  
  Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  generator->SetKinE(kin_energy);	
  generator->SetPSigmaOverP(0.04);				//sigmaPOverP	percentage of ion momentum. 
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();

//  generator->SetThetaSigma(0, sigmaTheta);	//равномерный тета
  generator->SetThetaSigma(1, sigmaTheta);
  // generator->SetThetaSigma(0, 0);
  generator->SetPhiRange(0, 45);
//  generator->SetPhiRange(0, 360);		//равномерный фи
//  generator->SetBoxXYZ(0, 0, 0., 0., beamStartPosition);	//задает ограничение размытия пятна пучка на мишени
  generator->SetBoxXYZ(0.7, 0.7, 0.8, 0.8, beamStartPosition);
  
  generator->SpreadingOnTarget(); 				//Sets spreading of x and y coordinates on target (where z-position is zero) and 
  								//reconstruct them to the beam start position (to settes z-coordinate) along momentum vector. 
  								//By default spreading of x and y coordinates implements on setted z-coordinate. 

  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);

/////////////////////////////////////////////////////////////////////////////
  // ------- Decayer --------------------------------------------------------
  
  // Double_t massHe10 = 9.36268;//  // [GeV] 10He	wrong!
  // Double_t massHe10 = 9.3616696;//  // [GeV] 10He = 8He + 2n
  Double_t massHe10 = 9.3616583459;//  // [GeV] 10He = 8He + 2n уточненная
  // Double_t massHe10 = 9.364141;//  // [GeV] 10He from LISE++
  
  
  ERDecayer* decayer = new ERDecayer();  
  ERDecay10Heto8He* targetDecay = new ERDecay10Heto8He();
  targetDecay->SetHe10Mass(massHe10);
 
  targetDecay->SetInteractionVolumeName(interactionVol);
  
  // MaxPathLength = 105% target thickness in cm
  // InteractionLength = 5-10 * MaxPathLength in cm
  targetDecay->SetNuclearInteractionLength(6.3);
  targetDecay->SetMaxPathLength(0.63/*2e-4 * 10 * 1.1*/);
  
  //targetDecay->SetMaxPathLength(0.063/*2e-4 * 10 * 1.1*/);		//для увеличения количеcтва событий
 //  targetDecay->SetMaxPathLength(0.022/*2e-4 * 10 * 1.1*/);		//для тонкой мишени
 
 // targetDecay->SetAngularDistribution("cos_tetta_cross.txt");
  //targetDecay->SetAngularDistribution("sin_theta_cross.txt");		//лежит в папке er/input
  
  // targetDecay->SetDecayFile("pmom-pv-1_short.dat", 0.0005 /*excitation in file [GeV]*/);
  // targetDecay->SetDecayFile("10he_int-3030_short.dat", 0.001 /*excitation in file [GeV]*/);
  // targetDecay->SetDecayFile("10he_int-3030.dat", 0.001 /*excitation in file [GeV]*/);  
  // targetDecay->SetDecayFile("new_10he_0p_r.dat", 0.001 /*excitation in file [GeV]*/);
  
//  targetDecay->SetDecayFile("new_10he_0p_r.dat", 0.001 /*excitation in file [GeV]*/);

    targetDecay->SetDecayFile(datFile.Data(), 0.001 /*excitation in file [GeV]*/);
    
//    targetDecay->SetDecayFile("new_10he_0p_1eV.dat", 0.001 /*excitation in file [GeV]*/);
  
  targetDecay->SetHe10Exitation(0.0021, 0.002, 1);			//[GeV]
  // targetDecay->SetHe10Exitation(-0.0236364, 0, 1);		//поправка по энергии
  
  //targetDecay->SetH7Exitation(0.0012, 0.0002355, 1);
  targetDecay->SetMinStep(1e-2);
  // targetDecay->SetMaxPathLength(0.63/*2e-4 * 10 * 1.1*/);

  // targetDecay->PrintMass();

  decayer->AddDecay(targetDecay);
  run->SetDecayer(decayer);

  // ------- QTelescope Digitizer -------------------------------------------
  ERTelescopeDigitizer* qtelescopeDigitizer = new ERTelescopeDigitizer(verbose);
  qtelescopeDigitizer->SetSiElossThreshold(0);
  qtelescopeDigitizer->SetSiElossSigma(0);
  qtelescopeDigitizer->SetSiTimeSigma(0);
  qtelescopeDigitizer->SetCsIElossThreshold(0);
  qtelescopeDigitizer->SetCsIElossSigma(0);
  qtelescopeDigitizer->SetCsITimeSigma(0);
  run->AddTask(qtelescopeDigitizer);

  // -----  BeamDet Digitizer ----------------------------------------------
  ERBeamDetDigitizer* beamDetDigitizer = new ERBeamDetDigitizer(verbose);
  //beamDetDigitizer->SetMWPCElossThreshold(0.006);
  //beamDetDigitizer->SetToFElossThreshold(0.006);
  //beamDetDigitizer->SetToFElossSigmaOverEloss(0);
  //beamDetDigitizer->SetToFTimeSigma(1e-10);
  run->AddTask(beamDetDigitizer);
  // ------------------------------------------------------------------------
  ERNDDigitizer* ndDigitizer = new ERNDDigitizer(1);
  ndDigitizer->SetEdepError(0.0,0.04,0.02);
  ndDigitizer->SetLYError(0.0,0.04,0.02);
  ndDigitizer->SetTimeError(0.001);
  ndDigitizer->SetQuenchThreshold(0.005);
  ndDigitizer->SetLYThreshold(0.004);
  ndDigitizer->SetProbabilityB(0.1);
  ndDigitizer->SetProbabilityC(0.3);
  // run->AddTask(ndDigitizer);
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);

  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  // FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");		// для отладки

  // -----   Initialize simulation run   ------------------------------------
  run->Init();
  Int_t nSteps = -15000;
  // printf("Pdg table\n\n");
  // TDatabasePDG::Instance()->Print();

  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();

  TString setup_name = outFile;
  setup_name.Prepend("setup_");
  run->CreateGeometryFile(setup_name);
//  run->CreateGeometryFile("setup_exp10he.root");

  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
          << "s" << endl << endl;
  TDatabasePDG *tdb = TDatabasePDG::Instance();
//  tdb->Print();
}
// TODO: переименовать:
// ERQTelescope -> ERTelescope
// ERQTelescopeDigitizer -> ERTelescopeDigitizer
// ERQTelescopeSetup -> ERTelescopeSetup
// ERQTelescopeTrackFinder -> ERTelescopeTrackFinder
// ERQTelescopePID -> ERTelescopePID

 
