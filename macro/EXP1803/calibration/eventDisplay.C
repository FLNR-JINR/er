void eventDisplay(TString runId)
{
  TString simFile, parFile;
  parFile.Form("par_calib_%s.root", runId.Data());
  simFile.Form("sim_digi_calib_%s.root", runId.Data());
  FairRunAna *fRun= new FairRunAna();

  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(parFile);
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile(simFile);
  fRun->SetOutputFile("EventDisplay.root");

  FairEventManager *fMan= new FairEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");

  FairMCPointDraw *targetPoints =  new FairMCPointDraw ("BeamDetTargetPoint", kRed,  kFullSquare);
 
  fMan->AddTask(Track);
  fMan->AddTask(targetPoints);

  fMan->Init();
}
