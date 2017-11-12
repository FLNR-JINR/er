void eventDisplay_Si24()
{
  FairRunAna *fRun= new FairRunAna();

  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open("par_Si.root");
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile("sim_Si.root");
  fRun->SetOutputFile("EventDisplay_Si.root");

  FairEventManager *fMan= new FairEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw *CsIPoints =   new FairMCPointDraw ("ERQTelescopeCsIPoint",kGreen,  kFullSquare);
  FairMCPointDraw *SiPoints =   new FairMCPointDraw ("ERQTelescopeSiPoint",kOrange,  kFullSquare);

  fMan->AddTask(Track);
  fMan->AddTask(SiPoints);
  fMan->AddTask(CsIPoints);


  fMan->Init();
}
