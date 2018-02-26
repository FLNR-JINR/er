void base_sim(Int_t nEvents = 10){
  //---------------------Files-----------------------------------------------
  TString outFile= "sim.root";
  TString parFile= "par.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start(); 
  // ----
  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
/** Select transport engine
* TGeant3
* TGeant4
**/
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());          // Output file
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
  //---------------------------------
  // -----   Create detectors  ----------------------------------------------
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  // Det definition
  /* Select verbosity level
   * 0 - only standard logs
   * 1 - Print points after each event
  */
  Int_t verbose = 0;
  ERDetector* detector= new ERDetector("TestDetector", kTRUE,verbose);
  detector->SetGeometryFileName("base.geo.root");
  detector->AddSensetive("gas");
  detector->AddSensetive("Si");
  run->AddModule(detector);

 // FairModule* target = new ERTarget("BeamDetTarget", kTRUE, 1);
  //target->SetGeometryFileName("target.h.geo.root");
  //run->AddModule(target);
  // ---------------.q
  //--------------------------------------------------------
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  /*
  //Ion 15N
  Int_t A = 15;
  Int_t Z = 7;
  Int_t Q = 7;

  ERIonMixGenerator* generator = new ERIonMixGenerator("15N", Z, A, Q, 1);
  Double32_t kin_energy = 2.86 * 1e-3 * 15; //GeV
  //generator->SetKinESigma(kin_energy, 0);
  generator->SetPSigma(6.7835, 6.7835*0.003);
  //  generator->SetKinESigma(kin_energy, 0);

  generator->SpreadingOnTarget();

  Double32_t theta = 0;
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetThetaSigma(theta, sigmaTheta);
  
  generator->SetPhiRange(0, 360);

  Double32_t distanceToTarget = 200;
  Double32_t sigmaOnTarget = 0.5;
  generator->SetSigmaXYZ(0, 0, -distanceToTarget, sigmaOnTarget, sigmaOnTarget);
  //generator->SetBoxXYZ(-0.4,-0.4,0.4,0.4, -distanceToTarget);

  //generator->AddBackgroundIon("26P", 15, 26, 15, 0.25);
  //generator->AddBackgroundIon("26S", 16, 26, 16, 0.25);
  //generator->AddBackgroundIon("24Si", 14, 24, 14, 0.25);
*/
  //primGen->AddGenerator(generator);
 
  Int_t pdgId = 2212; // 
  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 0.;
  //Double32_t momentum = 0.05; //GeV
  Double32_t kin_energy = .05; //GeV
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV

  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta2);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(0,360);
  boxGen->SetXYZ(0.,0., 0.);

  primGen->AddGenerator(boxGen);
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // —– Initialize simulation run ———————————— 
  run->Init();
  Int_t nSteps = -15000;

  // —– Runtime database ——————————————— 
  Bool_t kParameterMerged = kTRUE; 
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data()); 
  rtdb->setOutput(parOut); 
  rtdb->saveOutput(); 
  rtdb->print(); // 
  // -----   Run simulation  ------------------------------------------------
    run->Run(nEvents);
    run->CreateGeometryFile("allGeo.root");
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
   // cout << "Energy " << momentum << "; mass " << mass << endl; 
}
