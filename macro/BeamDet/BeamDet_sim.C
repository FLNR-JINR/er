void BeamDet_sim(Int_t nEvents = 1000){
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
  ERBeamDet* beamDet= new ERBeamDet("ERBeamDet", kTRUE,verbose);
  beamDet->SetGeometryFileName("beamdet.v1.geo.root");
  run->AddModule(beamDet);

  FairModule* target = new ERTarget("BeamDetTarget", kTRUE, 1);
  target->SetGeometryFileName("target.h.geo.root");
  run->AddModule(target);
  // ------------------------------------------------------------------------
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  //Ion 28S
  Int_t A = 28;
  Int_t Z = 16;
  Int_t Q = 16;

  ERIonMixGenerator* sgenerator = new ERIonMixGenerator("28S", Z, A, Q, 1);
  Double32_t kin_energy = 40 * 1e-3 * 28; //GeV
  sgenerator->SetKinERange(kin_energy, kin_energy);
//  sgenerator->SetKinESigma(kin_energy, 0);

  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 0.0001*TMath::RadToDeg();
  sgenerator->SetThetaRange(theta1, theta1);
  sgenerator->SetPhiRange(0, 360);
  sgenerator->SetBoxXYZ(-0.4,-0.4,0.4,0.4, -1533);

  //sgenerator->AddBackgroundIon("BgIon1", 9, 27, 9, 0.5);
  //sgenerator->AddBackgroundIon("BgIon2", 3, 9, 3, 0.3);


  primGen->AddGenerator(sgenerator);
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