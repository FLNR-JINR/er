void eventDisplay()
{
  TString inDir(".");
  TString parFileName;
  TString simFileName;
  TString outFileName;

  parFileName.Form("%s/par_0.root", inDir.Data());
  simFileName.Form("%s/sim_0.root", inDir.Data());
  outFileName.Form("%s/eventDisplay.root", inDir.Data());

  FairRunAna* fRun = new FairRunAna();
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parFileName);
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile(simFileName);
  fRun->SetOutputFile(outFileName);

  FairEventManager* fMan = new FairEventManager();
  FairMCTracks* Track = new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw* PointsGas2 = new FairMCPointDraw ("Gas_Layer_0_Det_2_Point", kOrange, kFullSquare);
  FairMCPointDraw* PointsSi2 = new FairMCPointDraw ("Semi_Layer_0_Det_2_Point", kRed, kFullSquare);
  FairMCPointDraw* PointsGas13 = new FairMCPointDraw ("Gas_Layer_0_Det_13_Point", kOrange, kFullSquare);
  FairMCPointDraw* PointsSi13 = new FairMCPointDraw ("Semi_Layer_0_Det_13_Point", kRed, kFullSquare);

  fMan->AddTask(Track);
  fMan->AddTask(PointsGas2);
  fMan->AddTask(PointsSi2);
  fMan->AddTask(PointsGas13);
  fMan->AddTask(PointsSi13);

  fMan->Init();
}
