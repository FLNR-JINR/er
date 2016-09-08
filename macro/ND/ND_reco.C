void ND_reco(Int_t nEvents = 10000){
  //---------------------Files-----------------------------------------------
  TString inFile = "sim.root";
  TString outFile = "reco.root";
  TString parFile = "par.root";
  // ------------------------------------------------------------------------
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
  
  // -----   Digitization run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(inFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
 
  // ------------------------muSi hit producer---------------------------------
  ERNDHitFinder* hitFinder = new ERNDHitFinder(1);
  hitFinder->SetLYSigmaA(0.0344);
  hitFinder->SetLYSigmaB(0.0106);
  hitFinder->SetTimeSigmaPar(0.1);
  hitFinder->SetElossThreshold(0.005);
  hitFinder->SetLYThreshold(0.004);
  hitFinder->SetProbabilityB(0.1);
  hitFinder->SetProbabilityC(0.3);
  fRun->AddTask(hitFinder);
  // ------------------------------------------------------------------------
  
  // -----------Runtime DataBase info -------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  
  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------

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
