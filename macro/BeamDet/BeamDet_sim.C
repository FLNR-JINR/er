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
Int_t verbose = 1;
ERBeamDet* beamDet= new ERBeamDet("ERBeamDet", kTRUE,verbose);
beamDet->SetGeometryFileName("beamdet.v1.geo.root");
run->AddModule(beamDet);
// ------------------------------------------------------------------------
// -----   Create PrimaryGenerator   --------------------------------------
FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
/*Int_t pdgId = 2212; // proton  beam
Double32_t theta1 = 0.;  // polar angle distribution
Double32_t theta2 = 0.0;
Double32_t kin_energy = .500; //GeV
Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
boxGen->SetThetaRange(theta1, theta2);
boxGen->SetPRange(momentum, momentum);
boxGen->SetPhiRange(0.0, 0.0);
boxGen->SetBoxXYZ(0.,0.,0.6,0.6,0.);
*/
  //Ion 28S
/*  Int_t A = 28;
  Int_t Z = 16;
  Int_t Q = 16;
  Double_t Pz = 40 * 1e-3;// AGeV
  FairIonGenerator* ionGenerator = new FairIonGenerator(Z,A,Q,1,0.,0.,Pz,0.,0.,-10.);
*/
  //Ion 27F
  Int_t A = 28;
  Int_t Z = 16;
  Int_t Q = 16;
  Double_t Pz = 0.04;// AGeV
  //FairIonGenerator* ionGenerator = new FairIonGenerator(Z,A,Q,1,0.,0.,Pz,0.,0.,-10.);

  ERIonGenerator* sgenerator = new ERIonGenerator("28S", Z, A, Q, 1);
  Double32_t kin_energy = 0.4; //GeV
  Double_t   mass = sgenerator->Ion()->GetMass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
  sgenerator->SetPRange(momentum, momentum);

  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 0.0001*TMath::RadToDeg();
  sgenerator->SetThetaRange(theta1, theta2);
  sgenerator->SetPhiRange(0, 360);
  sgenerator->SetBoxXYZ(-0.4,-0.4,0.4,0.4,-10);



primGen->AddGenerator(sgenerator);
run->SetGenerator(primGen);
// ------------------------------------------------------------------------
//-------Set visualisation flag to true------------------------------------
run->SetStoreTraj(kTRUE);

//-------Set LOG verbosity  -----------------------------------------------
FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
// —– Initialize simulation run ———————————— 
run->Init();

// —– Runtime database ——————————————— 
Bool_t kParameterMerged = kTRUE; FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
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
  cout << "Energy " << momentum << "; mass " << mass << endl; 
}