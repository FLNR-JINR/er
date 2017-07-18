void SimToRaw(Int_t nEvents = 500000, UInt_t index=1){
  //---------------------Files-----------------------------------------------
  TString dir = "/store/ivan/simParallel/";
  TString indexStr;
  indexStr.Form("%d", index);
  TString digiFile = dir + "digi" + indexStr + ".root";
  TString outFile = dir + "simRaw" + indexStr + ".root";
  TString parFile = dir + "par" + indexStr + ".root";

  cout << "Output file: " << outFile << endl;
  cout << "Parameters file: " << parFile << endl;

  gRandom->SetSeed(index);
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
