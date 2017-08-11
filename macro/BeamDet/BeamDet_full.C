void BeamDet_full(Int_t nEvents = 100){
  //---------------------Files-----------------------------------------------
  TString outFile= "full.root";
  TString parFile= "par.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start(); 
  // ----
  // -----   Create simulation fRun   ----------------------------------------
  FairRunSim* fRun = new FairRunSim();
/** Select transport engine
* TGeant3
* TGeant4
**/
  fRun->SetName("TGeant4");              // Transport engine
  fRun->SetOutputFile(outFile.Data());          // Output file
// ------------------------------------------------------------------------
// -----   Create media   -------------------------------------------------
  fRun->SetMaterials("media.geo");       // Materials
// ------------------------------------------------------------------------

  //-------- Set MC event header --------------------------------------------
  ERMCEventHeader* header = new ERMCEventHeader();
  fRun->SetMCEventHeader(header);
  //---------------------------------
  // -----   Create detectors  ----------------------------------------------
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  fRun->AddModule(cave);

  // Det definition
  /* Select verbosity level
   * 0 - only standard logs
   * 1 - Print points after each event
  */
  Int_t verbose = 0;
  ERBeamDet* beamDet= new ERBeamDet("ERBeamDet", kTRUE,verbose);
  beamDet->SetGeometryFileName("beamdet.v3.geo.root");

  beamDet->SetIonPID(1000160280);
  fRun->AddModule(beamDet);

 // FairModule* target = new ERTarget("BeamDetTarget", kTRUE, 1);
  //target->SetGeometryFileName("target.h.geo.root");
  //fRun->AddModule(target);
  // ---------------.q
  //--------------------------------------------------------
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  //Ion 28S
  Int_t A = 28;
  Int_t Z = 16;
  Int_t Q = 16;

  ERIonMixGenerator* generator = new ERIonMixGenerator("28S", Z, A, Q, 1);
  Double32_t kin_energy = 40 * 1e-3 * 28; //GeV
  generator->SetKinE(kin_energy);
  generator->SetPSigmaOverP(0.003);
  //generator->SetPSigma(6.7835, 6.7835*0.003);
  //generator->SetKinESigma(kin_energy, 0);

  /*
   * Set flag to spread corrdinates parameters on target and reconstruct   
   * coordinates of beam start by received values 
  */
  generator->SpreadingOnTarget();

  Double32_t theta = 0;
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetThetaSigma(theta, sigmaTheta);
  
  generator->SetPhiRange(0, 360);

  Double32_t distanceToTarget = 1555;
  Double32_t sigmaOnTarget = 0.5;
  generator->SetSigmaXYZ(0, 0, -distanceToTarget, sigmaOnTarget, sigmaOnTarget);
  //generator->SetBoxXYZ(-0.4,-0.4,0.4,0.4, -distanceToTarget);

  generator->AddBackgroundIon("26P", 15, 26, 15, 0.1 / 0.55);
  generator->AddBackgroundIon("26S", 16, 26, 16, 0.15 / 0.55);
  generator->AddBackgroundIon("24Si", 14, 24, 14, 0.2 / 0.55);

  primGen->AddGenerator(generator);
  fRun->SetGenerator(primGen);
  // ------------------------------------------------------------------------
  //-------Set visualisation flag to true------------------------------------
  fRun->SetStoreTraj(kTRUE);
  // ------------------------------------------------------------------------
  // -----   fRuntime database   ---------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  // ------------------------------------------------------------------------

  // —– fRuntime database ——————————————— 
  Bool_t kParameterMerged = kTRUE; 
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());

  rtdb->setOutput(parOut); 
  rtdb->saveOutput(); 
  rtdb->print(); // 
 // ------------------------BeamDetDigitizer---------------------------------
  ERBeamDetDigitizer* digitizer = new ERBeamDetDigitizer(verbose);
  digitizer->SetMWPCElossThreshold(0.006);
  digitizer->SetTofElossThreshold(0.006);
  digitizer->SetTofElossSigmaOverEloss(0);
  digitizer->SetTofTimeSigma(1e-10);
  fRun->AddTask(digitizer);
  // ------------------------------------------------------------------------
  // -----------------------BeamDetTrackFinder------------------------------
  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(1);
  fRun->AddTask(trackFinder);
  // ------------------------------------------------------------------------
  // -----------------------BeamDetTrackPID-------------------------------
  ERBeamDetPID* pid = new ERBeamDetPID(1);
  pid->SetPID(1000160280);
  pid->SetIonMass(26.2716160);
  //pid->SetBoxPID(203., 206., 0.005, 0.12);
  pid->SetBoxPID(0., 500., 0., 1.);
  pid->SetOffsetTOF(0.);
  pid->SetProbabilityThreshold(0.5);

  fRun->AddTask(pid);
  // ------------------------------------------------------------------------

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // —– Initialize simulation fRun ———————————— 
  fRun->Init();
  Int_t nSteps = -15000;
  // -----   fRun simulation  ------------------------------------------------
    fRun->Run(nEvents);

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