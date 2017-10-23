void BeamDet_pid(Int_t nEvents = 2000){
  //---------------------Files-----------------------------------------------
  TString digiFile = "digi.root";
  TString trackFile = "track.root";
  TString outFile = "pid.root";
  TString parFile = "par.root";    
  // ------------------------------------------------------------------------
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
  
  // -----   Digitization run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(digiFile);
  fRun->AddFriend(trackFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  // ------------------------------------------------------------------------

  // ------------------------ track finder---------------------------------
  ERBeamDetPID* identification = new ERBeamDetPID(1);

  identification->SetPID(1000160280);
  identification->SetIonMass(26.2716160);
  identification->SetBoxPID(189., 192., 0.02, 0.09);
  identification->SeToFfsetToF(0.);
  identification->SetProbabilityThreshold(0.5);

  fRun->AddTask(identification);
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
