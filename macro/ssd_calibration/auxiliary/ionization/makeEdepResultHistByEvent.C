void makeEdepResultHistByEvent (TString outFile, TString detDbId, Double_t kinE_MevPerNucleon) {
  TFile *file = TFile::Open(outFile.Data());
  auto tree = (TTree*)file->Get("er");
  TClonesArray* siDigi = new TClonesArray("ERQTelescopeSiDigi",10000);
  TString branchName;
  branchName.Form("ERQTelescopeSiDigi_SiDet_%s_X_0", detDbId.Data());
  // branchName.Form("ERQTelescopeSiDigi_SiDet_SingleSi_Edep_d19_8um_X_0");
  tree->SetBranchAddress(branchName, &siDigi);
  
  TString saveName;
  // saveName.Form("%s_%s_%d.root", outFile.Remove(outFile.Length() - 4, outFile.Length()).Data(), 
  //                                               detDbId.Data(), 
  //                                               (int)(kinE_MevPerNucleon*4e3));
  saveName.Form("%s_%d.root", detDbId.Data(), (int)(kinE_MevPerNucleon*4e3));
  TString newFileName = TString("file_") + saveName;
  TFile *fileNewTree = new TFile(newFileName, "recreate");
  TTree *newTree = new TTree("E_loss","E_loss");
  Double_t edep;
  newTree->Branch("edep", &edep, "edep/D");
  // TH1D *hist = new TH1D(detDbId, detDbId, (int)(8./0.08), 0., 8.);
  cout <<  "tree->GetEntries(): " << tree->GetEntries() << endl; 
  for (unsigned evNb = 0; evNb < tree->GetEntries(); evNb++) {
    tree->GetEntry(evNb);
    for (unsigned pntInEvNb = 0; pntInEvNb < siDigi->GetEntriesFast(); pntInEvNb++) {
      ERQTelescopeSiDigi* digi = (ERQTelescopeSiDigi*)siDigi->At(pntInEvNb);
      edep = digi->GetEdep();
      cout << "edep " << edep << endl;
      newTree->Fill();
      // hist->Fill(Edep);
    }
  }
  fileNewTree->Write();
  fileNewTree->Close();
  cout << "created file " << newFileName << endl;
  // TCanvas *canv = new TCanvas();
  // hist->Draw();
  // TH1F *meanEdep = (TH1F*)gDirectory->Get(detDbId);
  // meanEdep->Draw();
  // meanEdep->SaveAs(TString("hist_") + saveName);
  // canv->SaveAs(TString("canv_") + saveName);
  // delete canv;
  return;
}

