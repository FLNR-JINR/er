void eventDisplay()
{
  FairRunAna *fRun= new FairRunAna();

  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open("par.root");
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile("sim.root");
  fRun->SetOutputFile("EventDisplay.root");

  FairEventManager *fMan= new FairEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw *SiPoints =   new FairMCPointDraw ("RTelescope1Si1Point",kGreen,  kFullSquare);
  FairMCPointDraw *Si2Points =   new FairMCPointDraw ("RTelescope1Si2Point",kGreen,  kFullSquare);
  FairMCPointDraw *CsIPoints =   new FairMCPointDraw ("RTelescope1CsIPoint",kOrange,  kFullSquare);

  fMan->AddTask(Track);
  fMan->AddTask(SiPoints);
  fMan->AddTask(Si2Points);

  fMan->AddTask(CsIPoints);

  fMan->Init();
}
