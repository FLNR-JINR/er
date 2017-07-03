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
  FairMCPointDraw *mwpcPoints =  new FairMCPointDraw ("BeamDetTOFPoint", kOrange,  kFullSquare);
  FairMCPointDraw *tofPoints =   new FairMCPointDraw ("BeamDetMWPCPoint", kRed,  kFullSquare);
 
  fMan->AddTask(Track);
  fMan->AddTask(mwpcPoints);
  fMan->AddTask(tofPoints);
  
  fMan->Init();
}
