void eventDisplay()
{
  FairRunAna* fRun = new FairRunAna();
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open("output/par0.root");
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile("output/sim0.root");
  fRun->SetOutputFile("output/eventDisplay.root");

  FairEventManager* fMan= new FairEventManager();
  FairMCTracks* Track = new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw* PointsGas = new FairMCPointDraw ("TestDetector1vDetGasPartPoint", kOrange, kFullSquare);
  FairMCPointDraw* PointsSi = new FairMCPointDraw ("TestDetector1vSemiPoint", kRed, kFullSquare);

  fMan->AddTask(Track);
  fMan->AddTask(PointsGas);
  fMan->AddTask(PointsSi);
  fMan->Init();
}
