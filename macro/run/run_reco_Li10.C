void run_reco_Li10(Int_t nEvents = 10000){
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
 
  // ------------------------ND hit finder---------------------------------
  ERNDHitFinder* NDhitFinder = new ERNDHitFinder(1);
  NDhitFinder->SetLYSigmaA(0.0344);
  NDhitFinder->SetLYSigmaB(0.0106);
  NDhitFinder->SetTimeSigmaPar(0.1);
  NDhitFinder->SetElossThreshold(0.005);
  NDhitFinder->SetLYThreshold(0.004);
  NDhitFinder->SetProbabilityB(0.1);
  NDhitFinder->SetProbabilityC(0.3);
  fRun->AddTask(NDhitFinder);
  // ------------------------------------------------------------------------

  // ------------------------DSRD hit producer---------------------------------
  ERDSRDHitFinder* DSRDhitFinder = new ERDSRDHitFinder(1);
  DSRDhitFinder->SetElossSigma(0.01);
  DSRDhitFinder->SetTimeSigmaPar(0.1);
  fRun->AddTask(DSRDhitFinder);
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
