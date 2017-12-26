void SimToRaw1(Int_t nEvents = 400000){
  //---------------------Files-----------------------------------------------
  TString digiFile = "/home/muzalevsky/work/dataER/simNeuRad/6000_3.2_noCross/digi1.root";
  TString outFile = "/home/muzalevsky/work/dataER/simNeuRad/6000_3.2_noCross/simRaw1_100.root";
  TString parFile = "/home/muzalevsky/work/dataER/simNeuRad/parColl1.root";
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
