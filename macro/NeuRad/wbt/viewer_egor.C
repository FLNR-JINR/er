void viewer_egor(Int_t nEvents = 1000)
{
  //---------------------Files-----------------------------------------------
  TString workDir = "results/";
  TString parFile  = workDir + "par.root";
  TString simFile  = workDir + "sim.root";
  TString digiFile = workDir + "digi.root";
  //TString recoFile = workDir + "reco.root";
  TString outFile  = workDir + "viewer.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(digiFile);
  //fRun->AddFriend(simFile);
  //fRun->AddFriend(recoFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
 
  // ------------------------------------------------------------------------
  ERNeuRadViewer* viewer = new ERNeuRadViewer(1);
  fRun->AddTask(viewer);
  // ------------------------------------------------------------------------

  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
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
  cout << "Output file writen: " << outFile << endl;
  cout << "Parameter file writen " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

}
