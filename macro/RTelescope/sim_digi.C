void sim_digi(Int_t nEvents = 10)
{
  //---------------------Files-----------------------------------------------
  TString outFile= "sim_digi.root";
  TString parFile= "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts.xml";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start(); 
  // ----
  // -----   Create simulation run   ----------------------------------------
  ERRunSim* run = new ERRunSim();
    /** Select transport engine
    * TGeant3
    * TGeant4
    **/
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());          // Output file
  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  //-------- Set MC event header --------------------------------------------
  ERMCEventHeader* header = new ERMCEventHeader();
  run->SetMCEventHeader(header);
  //---------------------------------
  // -----   Create detectors  ----------------------------------------------
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);
  // -----  QTelescope Setup ------------------------------------------------
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeTmp");
  TVector3 fZeroRotation(0., 0., 0.);
  ERGeoSubAssembly* assembly = new ERGeoSubAssembly("Telescope_1", TVector3(0., 0., 5.), fZeroRotation);
  ERRTelescopeGeoComponentDoubleSi* rStation = new ERRTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_R", 
                                                                                    TVector3(0., 0., 0.), TVector3(), "X");
  ERRTelescopeGeoComponentCsI* csi = new ERRTelescopeGeoComponentCsI("CsI", "CsI_Radial", 
                                                                     TVector3(0., 0., 5), TVector3());                
  assembly->AddComponent(rStation);
  assembly->AddComponent(csi);
  setupQTelescope->AddSubAssembly(assembly);
  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERTelescope", kTRUE, 1);
  run->AddModule(qtelescope);
  // ------- QTelescope Digitizer -------------------------------------------
  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(0/**/);
  qtelescopeDigitizer->SetSiElossThreshold(0);
  qtelescopeDigitizer->SetSiElossSigma(0);
  qtelescopeDigitizer->SetSiTimeSigma(0);
  qtelescopeDigitizer->SetCsIElossThreshold(0);
  qtelescopeDigitizer->SetCsIElossSigma(0);
  qtelescopeDigitizer->SetCsITimeSigma(0);
  run->AddTask(qtelescopeDigitizer);
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Int_t pdgId = 2212; // proton  beam
  Double32_t theta1 = 15.;  // polar angle distribution
  Double32_t theta2 = 15.;  
  Double32_t kin_energy = .050; //GeV
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta2);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(0, 360);
  boxGen->SetBoxXYZ(0.,0.,0.,0.,0.);
  primGen->AddGenerator(boxGen);
  run->SetGenerator(primGen);
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
  run->CreateGeometryFile("geometry.root");
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