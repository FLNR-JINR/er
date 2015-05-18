void NeuRad_analyse(Int_t nEvents = 1000){
  //---------------------Files-----------------------------------------------
  TString simFile = "sim.root";
  TString digiFile = "digi.root";
  TString outFile = "analyse.root";
  TString parFile = "par.root";
  // ------------------------------------------------------------------------
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
  
  // -----   Analyse run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(simFile);
  fRun->AddFriend(digiFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  
  // ------------------------NeuRadPointAnalyser-------------------------------
  ERNeuRadPointAnalyser* pointAnalyser = new ERNeuRadPointAnalyser();
  // ------------------------------------------------------------------------
  
  // ------------------------NeuRadPointAnalyser-------------------------------
  ERNeuRadDigiAnalyser* digiAnalyser = new ERNeuRadDigiAnalyser();
  // ------------------------------------------------------------------------
  
  fRun->AddTask(pointAnalyser);
  fRun->AddTask(digiAnalyser);
  
  // -----------Runtime DataBase info --------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data());
  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------
  
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
