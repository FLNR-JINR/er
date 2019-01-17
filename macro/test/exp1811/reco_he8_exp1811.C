int fTriggerNum = 2;
void reco_he8_exp1811(){
  //---------------------Files-----------------------------------------------
  TString inFile = "./he8_10_0010.Digi.root";
  auto file = TFile::Open(inFile.Data());
  auto tree = (TTree*)file->Get("er");
  Int_t nEvents = tree->GetEntriesFast();//1443710
  TString parFile = "par_1811.root";
  TString geoFile = "setup_exp1811.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  TString userCut;
  TString outFile;
  userCut.Form("EventHeader.fTrigger==%d", fTriggerNum);
  outFile.Form("he8_10_0010.trigger%d.reco.root", fTriggerNum);
  run->SetUserCut(userCut.Data());

  run->SetGeomFile(geoFile);
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);

  // ------------------------BeamDetTrackFinger--------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
  trackFinder->SetTargetVolume("targetBodyH2"); // targetBodyH2 - embedded BeamDet target volume, tubeD2 - D2 taget volume
  run->AddTask(trackFinder);
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parIO = new FairParRootFileIo();
  parIO->open(parFile.Data(), "UPDATE");
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  
  run->Init();
  run->Run(1, nEvents); // in Vratislav 's macro-recoMWPC, first event is triggered by 1
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
  // ------------------------------------------------------------------------
}