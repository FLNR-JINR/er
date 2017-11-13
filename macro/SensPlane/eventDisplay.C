void eventDisplay()
{
  FairRunAna* fRun = new FairRunAna();
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open("par.root");
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile("sim.root");
  fRun->SetOutputFile("eventDisplay.root");

  FairEventManager* fMan= new FairEventManager();
  FairMCTracks* Track = new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw* SensPlanePoints = new FairMCPointDraw ("ERSensPlanePoint", kOrange, kFullSquare);

  fMan->AddTask(Track);
  fMan->AddTask(SensPlanePoints);

  fMan->Init();
}
