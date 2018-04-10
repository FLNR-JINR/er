void eventDisplay()
{
  TString inDir("output");
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
//  FairMCPointDraw* PointsGas = new FairMCPointDraw ("TestDetector1vDetGasPartPoint", kOrange, kFullSquare);
//  FairMCPointDraw* PointsSi = new FairMCPointDraw ("TestDetector1vSemiPoint", kRed, kFullSquare);

  fMan->AddTask(Track);
//  fMan->AddTask(PointsGas);
//  fMan->AddTask(PointsSi);

  fMan->Init();
}
