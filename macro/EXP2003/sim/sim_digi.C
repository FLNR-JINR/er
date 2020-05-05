
void sim_digi (Int_t nEvents = 100000) {
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
  TString outFile= "sim_digi.root";
  TString parFile= "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts3.xml";
  TString paramFileBeamDet = workDirPath
                         + "/db/BeamDet/BeamDetParts.xml";
  TString targetGeoFileName = workDirPath + "/geometry/target.D2.gas.root";
  TString ndGeoFileName = workDirPath + "/geometry/ND.geo.root";
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer; 
  timer.Start();
  // ------------------------------------------------------------------------
  // -----   Create simulation run   ----------------------------------------
  ERRunSim* run = new ERRunSim();
  run->SetName("TGeant4");
  run->SetOutputFile(outFile.Data());
  // ------------------------------------------------------------------------
  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  //-------- Set MC event header --------------------------------------------
  EREXP1811EventHeader* decayMCheader = new EREXP1811EventHeader();
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
  // ----- CENTRAL parameters ----------------------------------------------------
  Double_t xPos, yPos, zPos;
  Double_t radius = 32.3+2.9;
  TVector3 rotationC(0., 0., 0.);
  xPos = radius * TMath::Sin(rotationC.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationC.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assembly_Central= new ERGeoSubAssembly("Central_telescope", TVector3(xPos, yPos, zPos), rotationC);
  ERQTelescopeGeoComponentSingleSi* f06 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_F06", 
                                                                                TVector3(), TVector3(), "X");
  assembly_Central->AddComponent(f06);
  setupQTelescope->AddSubAssembly(assembly_Central);
  //-------------------------------------------------------------------------
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
  // ------- Decayer --------------------------------------------------------
  
  Double_t massH7 = 6.5691;//7.5061760;  // [GeV]
  ERDecayer* decayer = new ERDecayer();
  ERDecayEXP1811* targetDecay = new ERDecayEXP1811();
  targetDecay->SetInteractionVolumeName("tubeD2");
  targetDecay->SetNuclearInteractionLength(20.);
  //targetDecay->SetAngularDistribution("Cs_6He_d_3He_5H_35-25AMeV.txt");
  targetDecay->SetH7Mass(massH7);
  //targetDecay->SetDecayFile("pmom-pv-1_short.dat", 0.0005 /*excitation in file [GeV]*/);
  //targetDecay->SetH7Exitation(0.0004, 0.00002355, 1);
  //targetDecay->SetH7Exitation(0.0012, 0.0002355, 1);
  targetDecay->SetMinStep(1e-1);
  targetDecay->SetMaxPathLength(2./*2e-4 * 10 * 1.1*/);

  decayer->AddDecay(targetDecay);
  run->SetDecayer(decayer);
  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE,verbose);
  run->AddModule(qtelescope);
  // ------- QTelescope Digitizer -------------------------------------------
  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(verbose);
  run->AddTask(qtelescopeDigitizer);
  //-------Set visualisation flag to true------------------------------------
  //run->SetStoreTraj(kTRUE);

  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");

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

  run->CreateGeometryFile("setup_exp2003.root");

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

 
