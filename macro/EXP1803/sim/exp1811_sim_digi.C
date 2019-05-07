
void exp1811_sim_digi (Int_t nEvents = 50) {
//----------------------------------
  Double_t BeamDetLToF = 1232.0;     // [cm] 12348
  Double_t BeamDetPosZToF = -95.3;  // [cm] 
  Double_t BeamDetPosZ1MWPC = -81.5;     // [cm]
  Double_t BeamDetPosZ2MWPC = -27.;  // [cm]  
  // --------------- Beam start position ------------------------------------
  Double_t beamStartPosition = -1600.;  // [cm]
  // --------------- Target -------------------------------------------------
  Double_t targetD2Thickness = 0.42;  // [cm] this parameter should coincide with target H2 thickness in /macro/geo/create_target_D2_geo.C
  //---------------------Files-----------------------------------------------
  TString outFile= "sim_digi_1811.root";
  TString parFile= "par_1811.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts2.xml";
  TString paramFileBeamDet = workDirPath
                         + "/db/BeamDet/BeamDetParts.xml";
  TString targetGeoFileName = workDirPath + "/geometry/target.D2.gas.root";
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer; 
  timer.Start();
  // ------------------------------------------------------------------------
  // -----   Create simulation run   ----------------------------------------
  ERRunSim* run = new ERRunSim();
  /** Select transport engine
  * TGeant3
  * TGeant4exp1811_sim_digi.C
  **/
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());          // Output file
  // ------------------------------------------------------------------------
  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  //-------- Set MC event header --------------------------------------------
  ERDecayMCEventHeader* decayMCheader = new ERDecayMCEventHeader();
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
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF);    //  BeamDet parts should be added in ascending order   
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZ1MWPC);   //  of Z-coordinate of part.
  setupBeamDet->SetMWPCnumberingInvOrderX(); 

  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZ2MWPC);    
  setupBeamDet->SetMWPCnumberingInvOrderX(); 

  //setupBeamDet->SetSensitiveTarget();


  // -----   Create target  -------------------------------------------------
  FairModule* target = new ERTarget("target", kTRUE, 1);
  target->SetGeometryFileName(targetGeoFileName);
  run->AddModule(target);

  // -----  QTelescope Setup ------------------------------------------------
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeTmp");

  // ----- LEFT parameters ----------------------------------------------------
  Double_t xPos, yPos, zPos;
  Double_t radius = 18.;
  
  TVector3 rotationL(0., 17., 0.);
  xPos = radius * TMath::Sin(rotationL.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationL.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assembly_Left = new ERGeoSubAssembly("Left_telescope", TVector3(xPos, yPos, zPos), rotationL);
  ERQTelescopeGeoComponentSingleSi* thin_Left = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_L", 
                                                                                  TVector3(0., 0., -1.4), TVector3(), "X");
  ERQTelescopeGeoComponentDoubleSi* thick1_Left = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_DSD_L", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "X");
  ERQTelescopeGeoComponentSingleSi* thick2_Left = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_L", 
                                                                                  TVector3(0., 0., 1.0), TVector3(), "X");
  assembly_Left->AddComponent(thin_Left);
  assembly_Left->AddComponent(thick1_Left);
  assembly_Left->AddComponent(thick2_Left);

  setupQTelescope->AddSubAssembly(assembly_Left);

  // ----- CENTRAL parameters ----------------------------------------------------
  radius = 28.;
  TVector3 rotationC(0., 0., 0.);
  xPos = radius * TMath::Sin(rotationC.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationC.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assembly_Central= new ERGeoSubAssembly("Central_telescope", TVector3(xPos, yPos, zPos), rotationC);
  
  ERQTelescopeGeoComponentDoubleSi* thick_Central = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_DSD_C", 
                                                                                  TVector3(0., 0., -2.9), TVector3(), "X");
  ERQTelescopeGeoComponentCsI* csi = new ERQTelescopeGeoComponentCsI("CsI", "CsI_C", TVector3(0., 0., 0.), TVector3());

  assembly_Central->AddComponent(thick_Central);
  assembly_Central->AddComponent(csi);

  setupQTelescope->AddSubAssembly(assembly_Central);

  // ----- RIGHT parameters ----------------------------------------------------
  radius = 18.;
  TVector3 rotationR(0.,-17., 0.);
  xPos = radius * TMath::Sin(rotationR.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationR.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assembly_Right = new ERGeoSubAssembly("Right_telescope", TVector3(xPos, yPos, zPos), rotationR);
  ERQTelescopeGeoComponentSingleSi* thin_Right = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_R", 
                                                                                  TVector3(0., 0.,-1.4), TVector3(), "X");
  ERQTelescopeGeoComponentSingleSi* thick1_Right = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_R", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "Y");
  ERQTelescopeGeoComponentSingleSi* thick2_Right = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_R", 
                                                                                  TVector3(0., 0., 1.0), TVector3(), "Y");
  assembly_Right->AddComponent(thin_Right);
  assembly_Right->AddComponent(thick1_Right);
  assembly_Right->AddComponent(thick2_Right);

  setupQTelescope->AddSubAssembly(assembly_Right);
  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE,verbose);
  run->AddModule(qtelescope);
  // ------BeamDet ----------------------------------------------------------
  ERBeamDet* beamdet= new ERBeamDet("ERBeamDet", kTRUE,verbose);
  run->AddModule(beamdet);

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  Double_t  kinE_MevPerNucleon = 30.;

  Int_t Z = 2, A = 8, Q = 2;
  TString ionName = "8He";
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  generator->SetKinE(kin_energy);
  generator->SetPSigmaOverP(0);
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();

  generator->SetThetaSigma(0, sigmaTheta);
  generator->SetPhiRange(0, 360);
  generator->SetBoxXYZ(0, 0, 0., 0., beamStartPosition);
  generator->SpreadingOnTarget(); 

  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);

/////////////////////////////////////////////////////////////////////////////
  // ------- Decayer --------------------------------------------------------
  
  Double_t massH7 = 6.5691;//7.5061760;  // [GeV]


  ERDecayer* decayer = new ERDecayer();
  ERDecayEXP1811* targetDecay = new ERDecayEXP1811();

  targetDecay->SetInteractionVolumeName("tubeD2");

  targetDecay->SetNuclearInteractionLength(20.);

  //targetDecay->SetAngularDistribution("Cs_6He_d_3He_5H_35-25AMeV.txt");
  targetDecay->SetH7Mass(massH7);
  // targetDecay->SetH7Exitation(0.0004, 0.00002355, 1);
  //targetDecay->SetH7Exitation(0.0012, 0.0002355, 1);
  targetDecay->SetMinStep(1e-1);
  targetDecay->SetMaxPathLength(2./*2e-4 * 10 * 1.1*/);

  decayer->AddDecay(targetDecay);
  run->SetDecayer(decayer);

  // ------- QTelescope Digitizer -------------------------------------------
  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(verbose);

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

  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
  trackFinder->SetTargetVolume("tubeD2");

  run->AddTask(trackFinder);

  // -----------------------BeamDetTrackPID----------------------------------
  ERBeamDetPID* pid = new ERBeamDetPID(verbose);
  pid->SetBoxPID(0., 1000., 0., 1000.);
  pid->SetOffsetToF(0.);
  pid->SetProbabilityThreshold(0);
  pid->SetIonMass(6.5696366); //????
  pid->SetPID(1000020080);
  // run->AddTask(pid);

  //-------Set visualisation flag to true------------------------------------
  //run->SetStoreTraj(kTRUE);

  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogScreenLevel("FATAL");

  // -----   Initialize simulation run   ------------------------------------
  run->Init();
  Int_t nSteps = -15000;

  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();

  run->CreateGeometryFile("setup_exp1811.root");

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
  tdb->Print();

}

 
 
