void BeamDet_sim(Int_t nEvents = 10){
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
  //beamDet->SetGeometryFileName("beamdet.temp.root");
  ERBeamDetSetup* setup = ERBeamDetSetup::Instance();

  setup->SetXmlParametersFile("equip.xml");
  setup->AddMWPC("MWPC1", -40.);
  setup->AddMWPC("MWPC1", -8.);
  setup->AddToF("ToF1",-1550.);
  setup->AddToF("ToF2",-50.);

  //beamDet->SetIonPID(1000160280);
  run->AddModule(beamDet);

 // FairModule* target = new ERTarget("BeamDetTarget", kTRUE, 1);
  //target->SetGeometryFileName("target.h.geo.root");
  //run->AddModule(target);
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
  //generator->SetKinESigma(kin_energy, 0);
  generator->SetPSigma(6.7835, 6.7835*0.003);
//  generator->SetKinESigma(kin_energy, 0);

  /*
   * Set flag to spread corrdinates parameters on target and reconstruct   
   * coordinates of beam start by received values 
  */
  generator->SpreadingOnTarget();

  Double32_t theta = 0;
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetThetaSigma(theta, sigmaTheta);
  
  generator->SetPhiRange(0, 360);

  Double32_t distanceToTarget = 1533;
  Double32_t sigmaOnTarget = 0.5;
  generator->SetSigmaXYZ(0, 0, -distanceToTarget, sigmaOnTarget, sigmaOnTarget);
  //generator->SetBoxXYZ(-0.4,-0.4,0.4,0.4, -distanceToTarget);

  generator->AddBackgroundIon("26P", 15, 26, 15, 0.25);
  generator->AddBackgroundIon("26S", 16, 26, 16, 0.25);
  generator->AddBackgroundIon("24Si", 14, 24, 14, 0.25);

  primGen->AddGenerator(generator);
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