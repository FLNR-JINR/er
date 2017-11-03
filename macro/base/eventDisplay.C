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
  FairMCPointDraw *points1 =  new FairMCPointDraw ("TestDetectorgasPoint", kOrange,  kFullSquare);
  FairMCPointDraw *points2 =   new FairMCPointDraw ("TestDetectorplasticPoint", kRed,  kFullSquare);
 
  fMan->AddTask(Track);
  fMan->AddTask(points1);
  fMan->AddTask(points2);
  
  fMan->Init();
}
