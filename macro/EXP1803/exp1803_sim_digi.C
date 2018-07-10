void exp1803_sim_digi(Int_t nEvents = 10000) {
  // --------------- Telescope T1 -------------------------------------------
  Double_t T1Dl = 0.5;         // [cm]      
  Double_t T1PosZ = 10.;       // [cm] 
  Double_t T1D1Thick = 0.002;  // [cm]  
  Double_t T1D2Thick = 0.1;    // [cm] 
  Double_t T1Side = 6.2;       // [cm]  this parameter should coincide with SD1 side length from /db/QTelescope/QTelescopeParts.xml
  Double_t T1Aperture = 3.1;   // [cm]  
  // --------------- Telescope D1 -------------------------------------------
  Double_t D1PosZ = 20.;       // [cm]
  Double_t D1Thick = 0.03;     // [cm]
  // --------------- BeamDet ------------------------------------------------
  Double_t BeamDetLToF = 1500.;     // [cm] 
  Double_t BeamDetPosZToF = -50;  // [cm] 
  Double_t BeamDetLMWPC = 32.;     // [cm]
  Double_t BeamDetPosZMWPC = -8;  // [cm]  
  // --------------- Beam start position ------------------------------------
  Double_t beamStartPosition = -1600;  // [cm]
  // --------------- Target -------------------------------------------------
  Double_t targetH2Thickness = 0.4;  // [cm] this parameter should coincide with target H2 thickness in /macro/geo/create_target_h2_geo.C
  //---------------------Files-----------------------------------------------
  TString outFile= "sim_digi.root";
  TString parFile= "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts.xml";
  TString paramFileBeamDet = workDirPath
                         + "/db/BeamDet/BeamDetParts.xml";
  TString targetGeoFileName = workDirPath + "/geometry/target_CD2_geo.root";
  TString gadastGeoFileName = workDirPath + "/geometry/partOfGadast.v1.geo.root";
  TString ndGeoFileName = workDirPath + "/geometry/ND.geo.root";
  TString magnetGeoFileName = workDirPath + "/geometry/magnet.geo.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer; 
  timer.Start();
  // ------------------------------------------------------------------------
  // -----   Create simulation run   ----------------------------------------
  ERRunSim* run = new ERRunSim();
  /** Select transport engine
  * TGeant3
  * TGeant4
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
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF - BeamDetLToF);       // 
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF);                     //  BeamDet parts should be added in ascending order   
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZMWPC - BeamDetLMWPC);   //  of Z-coordinate of part.
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZMWPC);                  // 
  setupBeamDet->SetSensitiveTarget();

  // -----   Create target  -------------------------------------------------
  FairModule* target = new ERTarget("targetH2", kTRUE, 1);
  target->SetGeometryFileName(targetGeoFileName);
  // run->AddModule(target);

  // -----   Create Part of Gadast ------------------------------------------
  ERGadast* gadast = new ERGadast("PartofGadast", kTRUE, 1);
  gadast->SetGeometryFileName(gadastGeoFileName);
  //run->AddModule(gadast);

  // -----   Create Stilbene wall -------------------------------------------
  ERND* neutronDetector = new ERND("StilbeneWall", kTRUE, 1);
  neutronDetector->SetGeometryFileName(ndGeoFileName);
  //run->AddModule(neutronDetector);

  // -----   Create Magnet geometry -----------------------------------------
  FairModule* magnet = new ERTarget("magnet", 1, kTRUE);
  magnet->SetGeometryFileName(magnetGeoFileName);
  //run->AddModule(magnet);

  // -----  QTelescope Setup ------------------------------------------------
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeTmp");
  // ----- T1 parameters ----------------------------------------------------
  Double_t xPos, yPos, zPos;
  Double_t radius = 23.;
  TVector3 rotationT1(0., 12., 0.);
  xPos = radius * TMath::Sin(rotationT1.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationT1.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assemblyT1 = new ERGeoSubAssembly("T1", TVector3(xPos, yPos, zPos), rotationT1);
  ERQTelescopeGeoComponentSingleSi* thinT1 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_1", 
                                                                                  TVector3(0., 0., -5.36), TVector3(), "X");
  ERQTelescopeGeoComponentDoubleSi* thickT1 = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_SD2", 
                                                                                  TVector3(0, 0, 0.), TVector3(), "X");
  ERQTelescopeGeoComponentCsI* csi1 = new ERQTelescopeGeoComponentCsI("CsI", "CsI_1", TVector3(0, 0, 5.), TVector3());
  assemblyT1->AddComponent(thinT1);
  assemblyT1->AddComponent(thickT1);
  assemblyT1->AddComponent(csi1);

  setupQTelescope->AddSubAssembly(assemblyT1);
  // ----- T2 parameters ----------------------------------------------------
  radius = 29.;
  TVector3 rotationT2(0., -8.27, 0.);
  xPos = radius * TMath::Sin(rotationT2.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationT2.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assemblyT2 = new ERGeoSubAssembly("T2", TVector3(xPos, yPos, zPos), rotationT2);
  /*ERQTelescopeGeoComponentSingleSi* thinT2 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_1", 
                                                                                  TVector3(0, 0, 0.), TVector3(), "X");*/
  ERQTelescopeGeoComponentDoubleSi* thickT2 = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_SD2", 
                                                                                  TVector3(0, 0, 0.), TVector3(), "X");
  ERQTelescopeGeoComponentCsI* csi2 = new ERQTelescopeGeoComponentCsI("CsI", "CsI_1", TVector3(0, 0, 5.), TVector3());
  //assemblyT2->AddComponent(thinT2);
  assemblyT2->AddComponent(thickT2);
  assemblyT2->AddComponent(csi2);

  setupQTelescope->AddSubAssembly(assemblyT2);
  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE,verbose);
  run->AddModule(qtelescope);

  // ------BeamDet ----------------------------------------------------------
  ERBeamDet* beamdet= new ERBeamDet("ERBeamDet", kTRUE,verbose);
  run->AddModule(beamdet);

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  Double_t  kinE_MevPerNucleon = 40.;
  // Int_t     Z = 1, A = 3, Q = 1;
  // TString   ionName = "3H";
    Int_t Z = 2, A = 6, Q = 2;
  TString ionName = "6He";
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  generator->SetKinE(kin_energy);
  generator->SetPSigmaOverP(0);
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  // generator->SetKinERange(0,kin_energy);
  generator->SetThetaSigma(0, 0);
  generator->SetPhiRange(0, 360);
  generator->SetBoxXYZ(0, 0, 0, 0, beamStartPosition);
  generator->SpreadingOnTarget(); 

  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);

  // ------- Decayer --------------------------------------------------------
  Double_t massH5 = 4.69036244;  // [GeV]

  ERDecayer* decayer = new ERDecayer();
  ERDecayEXP1803* targetDecay = new ERDecayEXP1803();
  targetDecay->SetAngularDistribution("Cs_6He_d_3He_5H_35-25AMeV.txt");
  targetDecay->SetTargetVolumeName("targetBodyH2"); //"boxCD"  "tubeH2"
  targetDecay->SetTargetThickness(targetH2Thickness);
  targetDecay->SetH5Mass(massH5);
  targetDecay->SetH5Exitation(0.0004, 0.00002355, 1);
  targetDecay->SetH5Exitation(0.0012, 0.0002355, 1);
  targetDecay->SetMinStep(1e-2);
  targetDecay->SetInteractionVolumeName("boxCD");
  targetDecay->SetNuclearInteractionLength(0.2);
  targetDecay->SetMaxPathLength(TMath::Sqrt(4. + 0.16) * 1.1);

  decayer->AddDecay(targetDecay);
  run->SetDecayer(decayer);

  // ------- Magnetic field -------------------------------------------------
  // ERFieldMap* magField = new ERFieldMap("exp1803Field","A"); //exp1803Field, testField
  // magField->SetVolume("magnet");
  // run->SetField(magField);
    // ------- QTelescope Digitizer -------------------------------------------
  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(verbose);
  qtelescopeDigitizer->SetSiElossThreshold(0);
  qtelescopeDigitizer->SetSiElossSigma(0);
  qtelescopeDigitizer->SetSiTimeSigma(0);

  qtelescopeDigitizer->SetCsIElossThreshold(0);
  qtelescopeDigitizer->SetCsIElossSigma(0);
  qtelescopeDigitizer->SetCsITimeSigma(0);
  run->AddTask(qtelescopeDigitizer);
  // ------  Gadast Digitizer -----------------------------------------------
  ERGadastDigitizer* gadastDigitizer = new ERGadastDigitizer(verbose);
  //run->AddTask(gadastDigitizer);
  // -----  BeamDet Digitizer ----------------------------------------------
  ERBeamDetDigitizer* beamDetDigitizer = new ERBeamDetDigitizer(verbose);
  // beamDetDigitizer->SetMWPCElossThreshold(0.006);
  // beamDetDigitizer->SetToFElossThreshold(0.006);
  // beamDetDigitizer->SetToFElossSigmaOverEloss(0);
  // beamDetDigitizer->SetToFTimeSigma(1e-10);
  run->AddTask(beamDetDigitizer);

  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
  run->AddTask(trackFinder);
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
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
  //gMC->SetMaxNStep(nSteps);
  run->CreateGeometryFile("setup.root");
  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is sim.root" << endl;
  cout << "Parameter file is par.root" << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
          << "s" << endl << endl;
}