void reco(Int_t nEvents = 1){
  // -----   Logger  --------------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  //---------------------Files-----------------------------------------------
  TString inFile  = "sim_digi.root";
  TString outFile = "sim_reco.root";
  TString parFile = "par.root";
  TString geoFile = "setup.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  run->HoldEventsCount(); //forbid different entry number in the input and output file
  //run->SetUserCut("EventHeader.fTrigger==1");
  run->SetGeomFile(geoFile);
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  run->SetEventHeader(header);

  ERParticle* particle = new ERParticle("2H",1,2,1,2000);
  ERParticle* particlePDG = new ERParticle("neutron",2112);

  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parIO = new FairParRootFileIo();
  parIO->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIO);

  run->Init();

  particle->DefineMass();
  particle->Print();

  particlePDG->DefineMass();
  particlePDG->Print();

  run->Run(0, nEvents);
  // ------------------------------------------------------------------------;
  rtdb->setOutput(parIO);
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
