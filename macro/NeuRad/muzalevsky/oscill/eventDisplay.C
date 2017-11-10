void eventDisplay()
{
  FairRunAna *fRun= new FairRunAna();
  
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open("par2.root");
  rtdb->setFirstInput(parIo1);
  //rtdb->print();
  
  fRun->SetInputFile("sim2.root");
  fRun->SetOutputFile("EventDisplay.root");
  
  FairEventManager *fMan= new FairEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw *LandPoints =   new FairMCPointDraw ("NeuRadPoint",kOrange,  kFullSquare);
  
  fMan->AddTask(Track);
  fMan->AddTask(LandPoints);
  
  fMan->Init();
}
