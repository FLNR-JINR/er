void eventDisplay_full()
{
  // FairRunAna *fRun= new FairRunAna();

  // FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  // FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  // parIo1->open("par.root");
  // rtdb->setFirstInput(parIo1);
  // //rtdb->print();

  // fRun->SetInputFile("sim_digi.root");
  // fRun->SetOutputFile("EventDisplay.root");

  // // FairRootManager* ioman = FairRootManager::Instance();
  // // if ( ! ioman ) Fatal("Init", "No FairRootManager");
  // // vector<FairMCPointDraw*> qTelescopePoints;
  // // TList* allbrNames = ioman->GetBranchNameList();
  // // TIter nextBranch(allbrNames);
  // // TObjString* bName;
  // // vector<TString> pointBranches;
  // // while (bName = (TObjString*)nextBranch()) {
  // //   TString bFullName = bName->GetString();
  // //   if (bFullName.Contains("Point") && bFullName.Contains("QTelescope")) {
  // //     Int_t bPrefixNameLength = bFullName.First('_'); 
  // //     TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
  // //     qTelescopePoints.push_back(new FairMCPointDraw (brName, kOrange,  kFullSquare));
  // //   }
  // // }

  
  // FairEventManager *fMan= new FairEventManager();
  // FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");

  // fMan->AddTask(Track);
  // // for (const auto itQTelescopepoints: qTelescopePoints) {
  // //   fMan->AddTask(itQTelescopepoints);
  // // }
  // fMan->Init();

  FairRunAna *fRun= new FairRunAna();

  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open("par.root");
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile("sim_digi1.root");
  fRun->SetOutputFile("EventDisplay.root");

  FairEventManager *fMan= new FairEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");

  FairMCPointDraw *targetPoints =  new FairMCPointDraw ("BeamDetTargetPoint", kRed,  kFullSquare);
 
  fMan->AddTask(Track);
  fMan->AddTask(targetPoints);

  fMan->Init();
}
