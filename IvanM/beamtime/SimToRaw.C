void SimToRaw(Int_t nEvents = 100000){
  //---------------------Files-----------------------------------------------
  TString digiFile = "../NeuRad/oscill/data/digi.root";
  TString simFile = "../NeuRad/oscill/data/sim1.root";
  // TString recoFile = "reco.root";
  TString outFile = "../NeuRad/oscill/data/simRaw.root";
  TString parFile = "../NeuRad/oscill/data/par1.root";
  // ------------------------------------------------------------------------
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
  
  // -----   Digitization run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(digiFile);
  //fRun->AddFriend(simFile);
  //fRun->AddFriend(recoFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
 
  // ------------------------NeuRadHitProducer-------------------------------- 
  ERSimtoRaw* converter = new ERSimtoRaw(1);
  fRun->AddTask(converter);
  //------------------------------------------------------------------------
  // -----------Runtime DataBase info -------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  
  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------
  rtdb->setOutput(parIo1);
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
  cout << nEvents <<" its a number of events" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

}