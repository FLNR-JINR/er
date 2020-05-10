void makeElossResultHistByEvent (TString outFile, TString detDbId, Double_t kinE_MevPerNucleon) {
  TFile *file = TFile::Open(outFile.Data());
  auto tree = (TTree*)file->Get("er");
  TClonesArray* siPoint = new TClonesArray("ERQTelescopeSiPoint",10000);
  TString branchName;
  branchName.Form("ERQTelescopeSiPoint_SiDet_%s_X_0", detDbId.Data());
  // branchName.Form("ERQTelescopeSiPoint_SiDet_SingleSi_eloss_d19_8um_X_0");
  tree->SetBranchAddress(branchName, &siPoint);
  
  TString saveName;
  saveName.Form("%s_%d.root", detDbId.Data(), (int)(kinE_MevPerNucleon*4e3));
  TString newFileName = TString("file_") + saveName;
  TFile *fileNewTree = new TFile(newFileName, "recreate");
  TTree *newTree = new TTree("E_loss","E_loss");
  Double_t eloss;
  newTree->Branch("eloss", &eloss, "eloss/D");
  // TH1D *hist = new TH1D(detDbId, detDbId, (int)(8./0.08), 0., 8.);
  cout <<  "tree->GetEntries(): " << tree->GetEntries() << endl; 
  for (unsigned evNb = 0; evNb < tree->GetEntries(); evNb++) {
    tree->GetEntry(evNb);
    for (unsigned pntInEvNb = 0; pntInEvNb < siPoint->GetEntriesFast(); pntInEvNb++) {
      ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)siPoint->At(pntInEvNb);
      if (point->GetPDG() == 1000020040) {
        Double_t px = point->GetPx();
        Double_t py = point->GetPy();
        Double_t pz = point->GetPz();
        Double_t px_out = point->GetPxOut();
        Double_t py_out = point->GetPyOut();
        Double_t pz_out = point->GetPzOut();
        Double_t mass = point->GetMass();
        Double_t E_in = (TMath::Sqrt(mass*mass + px*px + py*py + pz*pz) - mass) * 1e3;
        Double_t E_out = (TMath::Sqrt(mass*mass + px_out*px_out + py_out*py_out + pz_out*pz_out) - mass) * 1e3;
        eloss = E_in - E_out;
        // cout << "Eloss " << eloss << endl;
        newTree->Fill();
        // hist->Fill(eloss);
      }
    }
  }
  fileNewTree->Write();
  fileNewTree->Close();
  cout << "created file " << newFileName << endl;
  // TCanvas *canv = new TCanvas();
  // hist->Draw();
  // TH1F *meanEloss = (TH1F*)gDirectory->Get(detDbId);
  // meanEloss->Draw();
  // meanEloss->SaveAs(TString("hist_") + saveName);
  // canv->SaveAs(TString("canv_") + saveName);
  // delete canv;
  return;
}

