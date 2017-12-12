void eventDisplay(TString filename="results/sim.root")
{
  FairRunAna* fRun = new FairRunAna();
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open("results/par.root");
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile(filename);
  fRun->SetOutputFile("results/eventDisplay.root");

  FairEventManager* fMan= new FairEventManager();

  FairMCTracks* Track = new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw* SensPlaneFrontPoints = new FairMCPointDraw ("ERSensPlaneFrontPoint", kOrange, kFullSquare);
  FairMCPointDraw* SensPlaneBackPoints = new FairMCPointDraw ("ERSensPlaneBackPoint", kRed, kFullSquare);
  FairMCPointDraw* NeuRadPoints = new FairMCPointDraw ("NeuRadPoint", kMagenta, kCircle);

  fMan->AddTask(Track);
  fMan->AddTask(SensPlaneFrontPoints);
  fMan->AddTask(SensPlaneBackPoints);
  fMan->AddTask(NeuRadPoints);

  fMan->Init();
}
