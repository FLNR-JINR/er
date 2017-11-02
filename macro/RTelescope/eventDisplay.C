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
  FairMCPointDraw *Si11Points =   new FairMCPointDraw ("RTelescope1Si1Point",kGreen,  kFullSquare);
  FairMCPointDraw *Si12Points =   new FairMCPointDraw ("RTelescope1Si2Point",kGreen,  kFullSquare);
  FairMCPointDraw *CsI1Points =   new FairMCPointDraw ("RTelescope1CsIPoint",kOrange,  kFullSquare);
  FairMCPointDraw *Si21Points =   new FairMCPointDraw ("RTelescope2Si1Point",kGreen,  kFullSquare);
  FairMCPointDraw *Si22Points =   new FairMCPointDraw ("RTelescope2Si2Point",kGreen,  kFullSquare);
  FairMCPointDraw *CsI2Points =   new FairMCPointDraw ("RTelescope2CsIPoint",kOrange,  kFullSquare);

  fMan->AddTask(Track);
  fMan->AddTask(Si11Points);
  fMan->AddTask(Si12Points);
  fMan->AddTask(CsI1Points);
  fMan->AddTask(Si21Points);
  fMan->AddTask(Si22Points);
  fMan->AddTask(CsI2Points);

  fMan->Init();
}
