// int fTriggerNum = 2;
void expCalib_reco(){
  //---------------------Files-----------------------------------------------
  TString inFile = "alltel_90_1to9_digi.root";
  auto file = TFile::Open(inFile.Data());
  auto tree = (TTree*)file->Get("er");
  Int_t nEvents = tree->GetEntriesFast();//1443710
  // nEvents /= 4;//1443710
  // nEvents = 1000000;//1443710

  // nEvents = /*1;*/ 14437;
  TString parFile = "par_Calib.root";
  TString geoFile = "geo_expCalib.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  TString userCut;
  TString outFile;
  // userCut.Form("EventHeader.fTrigger==%d", fTriggerNum);
  // outFile.Form("he8_10_0010.trigger%d.reco.root", fTriggerNum);
  outFile.Form("expCalib_reco.root");
  run->SetUserCut(userCut.Data());

  run->SetGeomFile("geo_expCalib.root");
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  Int_t verbose = 1;
  // ------- QTelescope TrackFinder -------------------------------------------
  ERQTelescopeTrackFinder* qtelescopeTrackFinder = new ERQTelescopeTrackFinder(verbose);
  qtelescopeTrackFinder->SetTargetPoint(0., 0., 0.);
  qtelescopeTrackFinder->SetHitStation("Telescope_1", "Telescope_1_SingleSi_SSD20_1_Y_0",
                                                         "Telescope_1_SingleSi_SSD_1_X_0");
  // qtelescopeTrackFinder->SetHitPseudoStation("Telescope_1", 
  //                                            "Telescope_1_SingleSi_SSD20_1_Y_0",
  //                                            "Telescope_1_SingleSi_SSD_1_X_0",
  //                                            "Telescope_1_Pseudo_SingleSi_SSD20_L_SingleSi_DSD_L_0");

  qtelescopeTrackFinder->SetStripEdepRange(0., 1000.);          // [GeV]
  //qtelescopeTrackFinder->SetTargetPoint(0., 0., 0.);
  //qtelescopeTrackFinder->SetStripEdepRange(0.0097, 100.);   // [GeV]
  //qtelescopeTrackFinder->SetEdepDiffXY(5.);                 // [GeV]
  qtelescopeTrackFinder->SetEdepMaxDiffXY(1000); 
  run->AddTask(qtelescopeTrackFinder);

  // ------- QTelescope PID -------------------------------------------
  ERQTelescopePID* qtelescopePID = new ERQTelescopePID(verbose);

  // qtelescopePID->SetUserCut("ERQTelescopeSiDigi_T2_SingleSi_SD2_XY_1_X.fEdep>0.009");
  // qtelescopePID->SetUserCut("1");

  // qtelescopePID->SetStationParticle("Telescope_1_SingleSi_SSD20_1_Y_0",1000020040);
  // qtelescopePID->SetStationParticle("Telescope_1_SingleSi_SSD_1_X_0",1000020040);
  ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_1_X_0
  ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD20_1_Y_0
  qtelescopePID->SetStationParticle("Telescope_1_SingleSi_SSD20_1_Y_0Telescope_1_SingleSi_SSD_1_X_0", 1000020040);

  // qtelescopePID->SetStationParticle("Telescope_1_SingleSi_SSD_L_X_0",1000020030);

  run->AddTask(qtelescopePID); 
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parInput = new FairParRootFileIo();
  parInput->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parInput);
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  run->Init();
  run->Run(0, nEvents);

  // // -----------Runtime DataBase info ---------------------------------------
  // FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // FairParRootFileIo*  parIO = new FairParRootFileIo();
  // parIO->open(parFile.Data(), "UPDATE");

  // // -----   Intialise and run   --------------------------------------------
  // FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  
  // run->Init();
  // run->Run(0, nEvents); // in Vratislav 's macro-recoMWPC, first event is triggered by 1

  // ------------------------------------------------------------------------;
  rtdb->setOutput(parInput);
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
