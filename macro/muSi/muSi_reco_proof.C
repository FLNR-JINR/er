void muSi_reco_proof(Int_t nEvents = 10000){
  //---------------------Files-----------------------------------------------
  TString inFile = "sim.root";
  TString outFile = "reco.root";
  TString parFile = "/home/vitaliy/er/macro/muSi/par.root";
  // ------------------------------------------------------------------------
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
  
  // -----   Digitization run   -------------------------------------------
  FairRunAnaProof *fRun= new FairRunAnaProof("workers=2");
  fRun->SetInputFile(inFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
 
  // ------------------------muSi hit producer---------------------------------
  ERmuSiHitProducer* hitProducer = new ERmuSiHitProducer(1);
  fRun->AddTask(hitProducer);
  // ------------------------------------------------------------------------
/*
  // ------------------------muSi track finder---------------------------------
  ERmuSiTrackFinder* trackFinder = new ERmuSiTrackFinder(1);
  trackFinder->SetAngleCut(0.003);
  fRun->AddTask(trackFinder);
  // ------------------------------------------------------------------------

  ERmuSiVertexFinder* vertexFinder = new ERmuSiVertexFinder(1);
  fRun->AddTask(vertexFinder);

  // ------------------------muSi matcher---------------------------------
  ERmuSiMatcher* matcher = new ERmuSiMatcher(1);
  fRun->AddTask(matcher);
  // ------------------------------------------------------------------------
  */
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
