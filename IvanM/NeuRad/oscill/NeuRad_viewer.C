void NeuRad_viewer(Int_t nEvents = 1000){
  //---------------------Files-----------------------------------------------
  TString digiFile = "data/digi2.root";
  TString simFile = "data/sim1.root";
  // TString recoFile = "data/reco.root";
  TString outFile = "data/viewer1.root";
  TString parFile = "data/par1.root";
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
  ERNeuRadViewer* viewer = new ERNeuRadViewer(1);
  fRun->AddTask(viewer);
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
  cout << endl;
  // ------------------------------------------------------------------------

}