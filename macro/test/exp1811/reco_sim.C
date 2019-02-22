int fTriggerNum = 2;
void reco_sim(){
  //---------------------Files-----------------------------------------------
  // TString inFile = "./he8_10_0010.Digi.root";
  TString inFile = "./sim_digi_1811.root";
  auto file = TFile::Open(inFile.Data());
  auto tree = (TTree*)file->Get("er");
  Int_t nEvents = tree->GetEntriesFast();//1443710

  // nEvents = /*1;*/ 14437;
  TString parFile = "par_1811.root";
  TString geoFile = "setup_exp1811.root";
// sim_digi_1811.root
// par_1811.root
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  TString userCut;
  TString outFile;
  // userCut.Form("EventHeader.fTrigger==%d", fTriggerNum);
  outFile.Form("reco_sim_data.root");
  // run->SetUserCut(userCut.Data());

  //run->SetGeomFile(geoFile);
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);

  // ------------------------BeamDetTrackFinger--------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
  trackFinder->SetTargetVolume("tubeD2"); // targetBodyH2 - embedded BeamDet target volume, tubeD2 - D2 taget volume
  run->AddTask(trackFinder);
  // -----------------------BeamDetTrackPID----------------------------------
  Int_t Z = 2, A = 8, Q = 2;
  TString ionName = "8He";
  ERBeamDetPID* pid = new ERBeamDetPID(verbose);
  pid->SetBoxPID(0., 1000., 0., 1000.);
  pid->SetOffsetToF(68.);
  pid->SetProbabilityThreshold(0);
  pid->SetIonMass(8.033922); //????
  pid->SetPID(1000020080);
  run->AddTask(pid);

  // ------- QTelescope TrackFinder -------------------------------------------
 ERQTelescopeTrackFinder* qtelescopeTrackFinder = new ERQTelescopeTrackFinder(verbose);

  // qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_CsI_0");
  // qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_DoubleSi_DSD_C_XY_0_Y");
  qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_DoubleSi_DSD_C_XY_0_X", 
                                                            "Central_telescope_DoubleSi_DSD_C_XY_0_Y");

  // qtelescopeTrackFinder->SetHitStation("Left_telescope", "Left_telescope_DoubleSi_DSD_L_XY_1_X",
  //                                                        "Left_telescope_DoubleSi_DSD_L_XY_1_Y");
  // qtelescopeTrackFinder->SetHitStation("Left_telescope", "Left_telescope_DoubleSi_DSD_L_XY_1_Y");
  // qtelescopeTrackFinder->SetHitStation("Left_telescope", "Left_telescope_SingleSi_SSD20_L_X_0");
  // qtelescopeTrackFinder->SetHitStation("Left_telescope", "Left_telescope_SingleSi_SSD_L_X_1");


  // qtelescopeTrackFinder->SetHitStation("Right_telescope","Right_telescope_SingleSi_SSD20_R_X_2",
  //                                                        "Right_telescope_SingleSi_SSDY_R_Y_3");
  // qtelescopeTrackFinder->SetHitStation("Right_telescope","Right_telescope_SingleSi_SSDY_R_Y_3");
  // qtelescopeTrackFinder->SetHitStation("Right_telescope","Right_telescope_SingleSi_SSD_R_Y_4");

  qtelescopeTrackFinder->SetStripEdepRange(0., 1000.);          // [GeV]
  //qtelescopeTrackFinder->SetTargetPoint(0., 0., 0.);
  //qtelescopeTrackFinder->SetStripEdepRange(0.0097, 100.);   // [GeV]
  //qtelescopeTrackFinder->SetEdepDiffXY(5.);                 // [GeV]
  qtelescopeTrackFinder->SetEdepMaxDiffXY(1000); 
  run->AddTask(qtelescopeTrackFinder);

  // ------- QTelescope PID -------------------------------------------
  ERQTelescopePID* qtelescopePID = new ERQTelescopePID(verbose);

  // qtelescopePID->SetUserCut("ERQTelescopeSiDigi_T2_DoubleSi_SD2_XY_1_X.fEdep>0.009");

  qtelescopePID->SetStationParticle("Left_telescope_SingleSi_SSD20_L_X_0",1000020030);
  qtelescopePID->SetStationParticle("Left_telescope_DoubleSi_DSD_L_XY_0",1000020030);
  qtelescopePID->SetStationParticle("Left_telescope_SingleSi_SSD_L_X_0",1000020030);

  qtelescopePID->SetStationParticle("Central_telescope_DoubleSi_DSD_C_XY_0",1000010030);
  qtelescopePID->SetStationParticle("Central_telescope_CsI_0",1000010030);

  qtelescopePID->SetStationParticle("Right_telescope_SingleSi_SSD20_R_X_0",1000020030);
  qtelescopePID->SetStationParticle("Right_telescope_SingleSi_SSDY_R_Y_0",1000020030);
  qtelescopePID->SetStationParticle("Right_telescope_SingleSi_SSD_R_Y_0",1000020030);

  // run->AddTask(qtelescopePID); 

  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parIO = new FairParRootFileIo();
  parIO->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIO);

  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  
  run->Init();
  run->Run(0, nEvents); // in Vratislav 's macro-recoMWPC, first event is triggered by 1

  // ------------------------------------------------------------------------;
  rtdb->setOutput(parIO);
  rtdb->saveOutput();

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file writen:  "    << outFile << endl;
  cout << "Parameter file writen " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;

  // ---------
}
