void sim_egor(Int_t nEvents = 1000)
{
  //---------------------Files-----------------------------------------------
  TString workDir = "results/";
  TString outFile = workDir + "sim.root";
  TString parFile = workDir + "par.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  /** Select transport engine
  * TGeant3
  * TGeant4
  **/
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());   // Output file
  // ------------------------------------------------------------------------

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // ------------------------------------------------------------------------

  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------

  //-------- Set MC event header --------------------------------------------
  ERMCEventHeader* header = new ERMCEventHeader();
  run->SetMCEventHeader(header);
  //-------------------------------------------------------------------------

  // -----   Create detectors  ----------------------------------------------
  FairModule* cave = new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo"); // cave air
  run->AddModule(cave);
/*
  ERCollimator* coll = new ERCollimator("COLLIMATOR");
  coll->SetGeometryFileName("collimator.geo.root");
//  coll->SetModulePosition(0., 0., 2.5);
  run->AddModule(coll);
*/
  ERSensPlane* sensPlane = new ERSensPlane("SENSPLANE");
  sensPlane->SetGeometryFileName("sensPlane.geo.root");
//  sensPlane->SetDetectorPosition(0., 0., 10.);
  run->AddModule(sensPlane);

  // ER NeuRad definition
  /* Select verbosity level
   * 1 - only standard logs
   * 2 - print points after each event
   * 3 - GEANT step information
  */
  Int_t verbose = 1;
  ERNeuRad* neuRad = new ERNeuRad("ERNeuRad", kTRUE, verbose);
  neuRad->SetGeometryFileName("NeuRad_Wupper_Proto.geo.root");
  /* Select storing stepss
   * not store steps
   * SetStorePrimarySteps() - store only primary particle step
   * SetStoreAllSteps() - store all steps. WARNING - very slow
  */
  neuRad->SetStorePrimarySteps();
  run->AddModule(neuRad);
  // ------------------------------------------------------------------------

  // -----   Create PrimaryGenerator   --------------------------------------
  Int_t pdgId = 22; // gamma 22
  Double32_t kin_energy = 661.7*1e-6; // GeV // Энергия гамма от Cs137 = 661.7 кэВ
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);

  Double_t thetaMax = TMath::ACos(0.9) * 180./TMath::Pi();

  boxGen->SetEkinRange(kin_energy, kin_energy);
  boxGen->SetThetaRange(0., thetaMax); // 0-90
  boxGen->SetPhiRange(0., 360.); // 0-360
  boxGen->SetBoxXYZ(0., 0., 0., 0., 0.); // xmin, ymin, xmax, ymax, z
  boxGen->SetCosTheta();

  primGen->AddGenerator(boxGen);
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------

  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("HIGH");

  // -----   Initialize simulation run   ------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO"); // DEBUG, DEBUG2, DEBUG3
  run->Init();
  Int_t nSteps = -15000; //TODO Why negative?
  //gMC->SetMaxNStep(nSteps);

  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ---------------------------------------------------------

  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);

  // -----   Export geometry ------------------------------------------------
  //TString erPath = gSystem->Getenv("VMCWORKDIR");
  //gGeoManager->Export(erPath + "/geometry/NeuRad_proto_setup.gdml");
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile.Data() << "." << endl;
  cout << "Parameter file is " << parFile.Data() << "." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
       << " s" << endl << endl;
}
