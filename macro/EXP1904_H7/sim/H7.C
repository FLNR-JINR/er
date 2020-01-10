void H7() { 
  // input
  TFile* reco_file = new TFile("reco.root");
  TTree* tree = (TTree*)reco_file->Get("er");
  TClonesArray* beamdet_he8 = new TClonesArray("ERBeamDetParticle", 10);
  tree->SetBranchAddress("BeamDetParticle.",&beamdet_he8);
  TClonesArray* t1_he3 = new TClonesArray("ERQTelescopeParticle", 10);
  tree->SetBranchAddress("ERQTelescopeParticle_T1_SingleSi_SSD20_X_0T1_SingleSi_SSD_Y_1_1000020030",&t1_he3);
  TClonesArray* t2_he3 = new TClonesArray("ERQTelescopeParticle", 10);
  tree->SetBranchAddress("ERQTelescopeParticle_T2_SingleSi_SSD_X_4T2_SingleSi_SSD20_Y_3s_1000020030",&t2_he3);
  TClonesArray* t3_he3 = new TClonesArray("ERQTelescopeParticle", 10);
  tree->SetBranchAddress("ERQTelescopeParticle_T3_SingleSi_SSD20_X_6T3_SingleSi_SSD_Y_7_1000020030",&t3_he3);
  TClonesArray* t4_he3 = new TClonesArray("ERQTelescopeParticle", 10);
  tree->SetBranchAddress("ERQTelescopeParticle_T4_SingleSi_SSD_X_10T4_SingleSi_SSD20_Y_9_1000020030",&t4_he3);
  //output
  TFile* h7_file = new TFile("h7.root", "recreate");
  TTree* h7_tree = new TTree("h7_tree", "Tree with h7 Lorentz vectors");
  TLorentzVector* h7_lv = new TLorentzVector();
  h7_tree->Branch("h7.", "TLorentzVector", &h7_lv);
  // event loop
  for (Long64_t i=0; i < tree->GetEntriesFast(); i++) {
    tree->GetEntry(i);
    const size_t he3_in_detectors_count = t1_he3->GetEntriesFast() + t2_he3->GetEntriesFast() 
        + t3_he3->GetEntriesFast() + t4_he3->GetEntriesFast();
    if (he3_in_detectors_count != 1
        || beamdet_he8->GetEntriesFast() != 1)
      continue;
    const auto he3 = t1_he3->GetEntriesFast() == 1 ? static_cast<ERQTelescopeParticle*>(t1_he3->At(0)) : 
                     t2_he3->GetEntriesFast() == 1 ? static_cast<ERQTelescopeParticle*>(t2_he3->At(0)) :
                     t3_he3->GetEntriesFast() == 1 ? static_cast<ERQTelescopeParticle*>(t3_he3->At(0)) :
                     t4_he3->GetEntriesFast() == 1 ? static_cast<ERQTelescopeParticle*>(t4_he3->At(0)) : 
                                                     nullptr;
    const auto he8  = static_cast<ERBeamDetParticle*>(beamdet_he8->At(0));
    const auto he3_lv = he3->GetLVInteraction();
    const auto he8_lv = he8->GetLVTarget();
    TLorentzVector h2_lv(0., 0., 0., 1.875612 /*GeV*/);                                            
    *h7_lv = he8_lv + h2_lv - he3_lv;
    h7_tree->Fill();
  }
  h7_tree->Write();
  h7_file->Write();
}