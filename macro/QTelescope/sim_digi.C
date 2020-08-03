void sim_digi(Int_t nEvents = 10)
{
  //---------------------Files-----------------------------------------------
  TString outFile= "sim_digi.root";
  TString parFile= "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts3.xml";
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
  // -----  QTelescope Setup ------------------------------------------------
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeTmp");

  // ----- 1 parameters ----------------------------------------------------
  Double_t x, y, z;
  x = 4.85;
  y = 2.35;
  //x = 4.625; //20 mcm
  //y = 2.075;
  z = 17.3+1.5;
  TVector3 fZeroRotation(0., 0., 0.);
  ERGeoSubAssembly* assembly_1 = new ERGeoSubAssembly("Telescope_1", TVector3(-x, y, z), fZeroRotation);
  ERQTelescopeGeoComponentSingleSi* thin1 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_1", 
                                                                                  TVector3(0., 0., -1.5), TVector3(), "X");
  ERQTelescopeGeoComponentSingleSi* thick1 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_1", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "Y");
  ERQTelescopeGeoComponentSingleSi* veto1 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_V_1", 
                                                                                  TVector3(0., 0., 1.0), TVector3(), "Y");
  /*ERQTelescopeGeoComponentPassive* passive = new ERQTelescopeGeoComponentPassive("passive_component.root", "PassiveElement",
                                                                                  TVector3(0., 0., -3.), TVector3());    */                                                                            
  
  assembly_1->AddComponent(thin1);
  assembly_1->AddComponent(thick1);
  assembly_1->AddComponent(veto1);
  //assembly_1->AddComponent(passive);

  setupQTelescope->AddSubAssembly(assembly_1);

  // ----- 2 parameters ----------------------------------------------------
  ERGeoSubAssembly* assembly_2 = new ERGeoSubAssembly("Telescope_2", TVector3(-y, -x, z), fZeroRotation);
  ERQTelescopeGeoComponentSingleSi* thin2 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_2", 
                                                                                  TVector3(0., 0., -1.5), TVector3(), "Y");
  ERQTelescopeGeoComponentSingleSi* thick2 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_2", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "X");
  ERQTelescopeGeoComponentSingleSi* veto2 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_V_2", 
                                                                                  TVector3(0., 0., 1.0), TVector3(), "X");

  assembly_2->AddComponent(thin2);
  assembly_2->AddComponent(thick2);
  assembly_2->AddComponent(veto2);

  setupQTelescope->AddSubAssembly(assembly_2);

  // ----- CENTRAL parameters ----------------------------------------------------
  Double_t xPos, yPos, zPos;
  Double_t radius = 32.3+2.9;
  TVector3 rotationC(0., 0., 0.);
  xPos = radius * TMath::Sin(rotationC.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationC.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assembly_Central= new ERGeoSubAssembly("Central_telescope", TVector3(xPos, yPos, zPos), rotationC);
  
  ERQTelescopeGeoComponentDoubleSi* thick_Central = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_DSD", 
                                                                                  TVector3(0., 0., -2.9), TVector3(), "X");
  ERQTelescopeGeoComponentCsI* csi = new ERQTelescopeGeoComponentCsI("CsI", "CsI", TVector3(0., 0., 0.), TVector3());

  assembly_Central->AddComponent(thick_Central);
  assembly_Central->AddComponent(csi);

  setupQTelescope->AddSubAssembly(assembly_Central);

  // ----- 3 parameters ----------------------------------------------------
  ERGeoSubAssembly* assembly_3 = new ERGeoSubAssembly("Telescope_3", TVector3(x, -y, z), fZeroRotation);
  ERQTelescopeGeoComponentSingleSi* thin3 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_3", 
                                                                                  TVector3(0., 0., -1.5), TVector3(), "X");
  ERQTelescopeGeoComponentSingleSi* thick3 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_3", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "Y");
  ERQTelescopeGeoComponentSingleSi* veto3 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_V_3", 
                                                                                  TVector3(0., 0., 1.0), TVector3(), "Y");

  assembly_3->AddComponent(thin3);
  assembly_3->AddComponent(thick3);
  assembly_3->AddComponent(veto3);

  setupQTelescope->AddSubAssembly(assembly_3);

  // ----- 4 parameters ----------------------------------------------------
  ERGeoSubAssembly* assembly_4 = new ERGeoSubAssembly("Telescope_4", TVector3(y, x, z), fZeroRotation);
  ERQTelescopeGeoComponentSingleSi* thin4 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_4", 
                                                                                  TVector3(0., 0., -1.5), TVector3(), "Y");
  ERQTelescopeGeoComponentSingleSi* thick4 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_4", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "X");
  ERQTelescopeGeoComponentSingleSi* veto4 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_V_4", 
                                                                                  TVector3(0., 0., 1.0), TVector3(), "X");

  assembly_4->AddComponent(thin4);
  assembly_4->AddComponent(thick4);
  assembly_4->AddComponent(veto4);

  setupQTelescope->AddSubAssembly(assembly_4);
  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE, 1);
  run->AddModule(qtelescope);

  // ------- QTelescope Digitizer -------------------------------------------
  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(0);
  qtelescopeDigitizer->SetSiElossThreshold(0);
  qtelescopeDigitizer->SetSiElossSigma(0);
  qtelescopeDigitizer->SetSiTimeSigma(0);
  qtelescopeDigitizer->SetCsIElossThreshold(0);
  qtelescopeDigitizer->SetCsIElossSigma(0);
  qtelescopeDigitizer->SetCsITimeSigma(0);
  run->AddTask(qtelescopeDigitizer);

 // FairModule* target = new ERTarget("BeamDetTarget", kTRUE, 1);
  //target->SetGeometryFileName("target.h.geo.root");
  //run->AddModule(target);
  // ---------------.q
  //--------------------------------------------------------
  // -----   Create PrimaryGenerator   --------------------------------------
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Int_t pdgId = 2212; // proton  beam
  Double32_t theta1 = 10.;  // polar angle distribution
  Double32_t theta2 = 10.;  //ПОДОБРАТЬ ТЕТТА_1 и ТЕТТА_2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  Double32_t kin_energy = .500; //GeV
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta2);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(0, 0); // ?????????????????????????????????????????????????????????????????????
  boxGen->SetBoxXYZ(0.,0.,0.,0.,0.);

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
  // cout << "Energy " << momentum << "; mass " << mass << endl; 
}