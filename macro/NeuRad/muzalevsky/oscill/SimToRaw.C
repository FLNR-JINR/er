void SimToRaw(Int_t nEvents = 200000){
  //---------------------Files-----------------------------------------------
  //TString digiFile = "/home/muzalevsky/work/dataER/simNeuRad/digi_6000_20.root";
  TString digiFile = "/home/muzalevsky/work/dataER/simNeuRad/tests/test_6000.root";
  // TString simFile = "sim.root";
  // TString recoFile = "reco.root";
  TString outFile = "/home/muzalevsky/work/dataER/simNeuRad/tests/simraw6000_0.root";
  TString parFile = "/home/muzalevsky/work/dataER/simNeuRad/parColl2.root";
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
