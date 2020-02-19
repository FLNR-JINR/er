void expCalib_reco() {
  //---------------------Files-----------------------------------------------
  TString inFile = "calib_65_1to10_digi.root";
  auto file = TFile::Open(inFile.Data());
  auto tree = (TTree*)file->Get("er");
  Int_t nEvents = tree->GetEntriesFast();//1443710:
  //nEvents = 100;
  TString parFile = "par_Calib.root";
  TString geoFile = "geo_expCalib.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  TString userCut;
  TString outFile;
  outFile.Form("expCalib_reco_quad_mean_1_sens.root");
  run->SetGeomFile("geo_expCalib.root");
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  Int_t verbose = 1;
  // ------- QTelescope TrackFinder -------------------------------------------
  ERQTelescopeTrackFinder* qtelescopeTrackFinder = new ERQTelescopeTrackFinder(verbose);
  qtelescopeTrackFinder->SetTargetPoint(0., 0., 0.);
  qtelescopeTrackFinder->SetHitStation("Telescope_1", "Telescope_1_DoubleSi_SSD20_1_XY_0_X",
                                                      "Telescope_1_DoubleSi_SSD20_1_XY_1_Y");
  qtelescopeTrackFinder->SetStripEdepRange(0.2e-3, 8.35e-3);          // [GeV]
  qtelescopeTrackFinder->SetEdepMaxDiffXY(1000); 
  run->AddTask(qtelescopeTrackFinder);
  // ------- QTelescope PID -------------------------------------------
  ERQTelescopePID* qtelescopePID = new ERQTelescopePID(verbose);
  qtelescopePID->SetStationParticle("Telescope_1_DoubleSi_SSD20_1_XY_0_XTelescope_1_DoubleSi_SSD20_1_XY_1_Y", 1000020040);
  run->AddTask(qtelescopePID); 
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parInput = new FairParRootFileIo();
  parInput->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parInput);
  // -----   Intialise and run   --------------------------------------------
  //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //run->HoldEventsCount();
  run->MarkFill(kFALSE);
  run->Init();
  run->Run(0, nEvents);
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
