void sim_digi(Int_t events_count = 100) {

//----------------------------------
  Double_t BeamDetLToF = 1232.0;     // [cm] 12348
  Double_t BeamDetPosZToF = -95.3;  // [cm] 
  Double_t BeamDetPosZ1MWPC = -81.;     // [cm]
  Double_t BeamDetPosZ2MWPC = -27.5;  // [cm]  
  // --------------- Beam start position ------------------------------------
  Double_t beamStartPosition = -1600.;  // [cm]
  // --------------- Target -------------------------------------------------
  Double_t targetD2Thickness = 0.6;  // [cm] this parameter should coincide with target H2 thickness in /macro/geo/create_target_D2_geo.C
  //---------------------Files-----------------------------------------------
  TString outFile = "sim_digi.root";
  TString parFile = "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath + "/db/QTelescope/QTelescopeParts.xml";
  TString paramFileBeamDet = workDirPath + "/db/BeamDet/BeamDetParts.xml";
  TString targetGeoFileName = workDirPath + "/geometry/BeamDet/target_exp2409.root";
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer; 
  timer.Start();
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogScreenLevel("FATAL");
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
  // ER2H_6LiEventHeader
  ERDecay6He4He2nTransferEventHeader* decayMCheader = new ERDecay6He4He2nTransferEventHeader();
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

  setupBeamDet->SetSensitiveTarget();

  // -----   Create target  -------------------------------------------------
  FairModule* target = new ERTarget("target", kTRUE, 1);
  target->SetGeometryFileName(targetGeoFileName);
  run->AddModule(target);

  // -----  QTelescope Setup ------------------------------------------------
  ERTelescopeSetup* setupQTelescope = ERTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeTmp");

  // ----- Left parameters ----------------------------------------------------
  Double_t xPos, yPos, zPos;
  Double_t radius = 33.;
  TVector3 fMyRotation(0., 0., 0.);

  TVector3 rotationL(0., 10., 0.);
  xPos = radius * TMath::Sin(rotationL.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationL.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assembly_left = new ERGeoSubAssembly("Telescope_left", TVector3(xPos, yPos, zPos), rotationL);
  ERQTelescopeGeoComponentDoubleSi* thick_Left = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_DSD_L", 
                                                                                  TVector3(0., 0., -3.), TVector3(), "X");
  ERQTelescopeGeoComponentCsI* csi_left = new ERQTelescopeGeoComponentCsI("CsI", "CsI", TVector3(0., 0., 0.), TVector3());

  assembly_left->AddComponent(thick_Left);
  assembly_left->AddComponent(csi_left);

  setupQTelescope->AddSubAssembly(assembly_left);

  // ----- Right parameters ----------------------------------------------------

  TVector3 rotationR(0., -25., 0.);
  radius = 12.5;
  xPos = radius * TMath::Sin(rotationR.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationR.Y() * TMath::DegToRad());
  
  ERGeoSubAssembly* assembly_right = new ERGeoSubAssembly("Telescope_right", TVector3(xPos, yPos, zPos), rotationR);

  ERQTelescopeGeoNonUniformSingleSi* thin_right = new ERQTelescopeGeoNonUniformSingleSi("SingleSi", "SingleSi_SSD_20",
                                                                                   TVector3(0.22, -0.27, -2.5), TVector3(), "X",
                                                                                   "input/map.root"); 
  ERQTelescopeGeoComponentDoubleSi* thick_right = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_DSD_R", 
                                                                                  TVector3(0., 0., -1.), TVector3(), "X");
  ERQTelescopeGeoComponentSingleSi* veto_right = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_Rv", 
                                                                               TVector3(0., 0., 0.), TVector3(), "Y");

  assembly_right->AddComponent(thin_right);
  assembly_right->AddComponent(thick_right);
  assembly_right->AddComponent(veto_right);

  setupQTelescope->AddSubAssembly(assembly_right);
  
  // ------QTelescope -------------------------------------------------------
  ERTelescope* qtelescope= new ERTelescope("ERQTelescope", kTRUE,verbose);
  run->AddModule(qtelescope);
  // ------BeamDet ----------------------------------------------------------
  ERBeamDet* beamdet= new ERBeamDet("ERBeamDet", kTRUE,verbose);
  run->AddModule(beamdet);

  //-------------------------------------------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  Double_t  kinE_MevPerNucleon = 33.5;
  
  Int_t Z = 2, A = 6, Q = 2;
  TString ionName = "8He";
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  generator->SetKinE(kin_energy);
  generator->SetPSigmaOverP(0.017);
  generator->SetThetaSigma(0.75, 0.4);
  generator->SetPhiRange(0, 360);
  generator->SetSigmaXYZ(0.033, -0.072, beamStartPosition, 0.5, 0.4);
  generator->SpreadingOnTarget(); 

  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);

/////////////////////////////////////////////////////////////////////////////
  // ------- Decayer --------------------------------------------------------
  
  // Double_t massn4 = 4*0.939565;//7.5061760;  // [GeV]
  ERDecayer* decayer = new ERDecayer();
  ERDecay6He4He2nTransfer* targetDecay = new ERDecay6He4He2nTransfer();
  targetDecay->SetInteractionVolumeName("shapeD2");
  targetDecay->SetNuclearInteractionLength(63.);
  targetDecay->Set6HeExcitation(0.00001, 0.0001, 1);
  // targetDecay->Set4nExitation(0.00237, 0.00001, 1);
  targetDecay->SetMinStep(1e-4);
  targetDecay->SetMaxPathLength(0.63/*2e-4 * 10 * 1.1*/);
  // targetDecay->SetAngularDistribution("/home/ivan/work/macro/exp2409/sim/input/rawCS.txt");

  decayer->AddDecay(targetDecay);
  run->SetDecayer(decayer);

  // ------- QTelescope Digitizer -------------------------------------------
  ERTelescopeDigitizer* qtelescopeDigitizer = new ERTelescopeDigitizer(verbose);
  qtelescopeDigitizer->SetSiElossThreshold(0.2);
  qtelescopeDigitizer->SetSiElossSigma(0.025);
  qtelescopeDigitizer->SetSiTimeSigma(0);
  qtelescopeDigitizer->SetCsIElossThreshold(1);
  qtelescopeDigitizer->SetCsIElossSigma(0.35);
  qtelescopeDigitizer->SetCsITimeSigma(0);
  run->AddTask(qtelescopeDigitizer);

  // -----  BeamDet Digitizer ----------------------------------------------
  ERBeamDetDigitizer* beamDetDigitizer = new ERBeamDetDigitizer(verbose);
  beamDetDigitizer->SetMWPCElossThreshold(0.);
  beamDetDigitizer->SetToFElossThreshold(0.);  
  beamDetDigitizer->SetToFElossSigmaOverEloss(0);
  beamDetDigitizer->SetToFTimeSigma(0.148);
  run->AddTask(beamDetDigitizer);
  // ------------------------------------------------------------------------

  //-------Set visualisation flag to true------------------------------------
  //run->SetStoreTraj(kTRUE);
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

  TString geometryName = "input/setup.root";
  run->CreateGeometryFile(geometryName.Data());

  // -----   Run simulation  ------------------------------------------------
  // events_count = 1;
  run->Run(events_count);

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

}

 
