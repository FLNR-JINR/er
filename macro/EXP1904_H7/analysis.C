void analysis(TString in_file, TString out_file) { 
   // input
   TFile* reco_file = new TFile(in_file);
   TTree* reco_tree = (TTree*)reco_file->Get("er");
   TClonesArray* beamdet_he8 = new TClonesArray("ERBeamDetParticle", 10);
   reco_tree->SetBranchAddress("BeamDetParticle.",&beamdet_he8);
   TClonesArray* t1_he3 = new TClonesArray("ERTelescopeParticle", 10);
   reco_tree->SetBranchAddress("TelescopeParticle_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y_1000020030", &t1_he3);
   TClonesArray* t2_he3 = new TClonesArray("ERTelescopeParticle", 10);
   reco_tree->SetBranchAddress("TelescopeParticle_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y_1000020030", &t2_he3);
   TClonesArray* t3_he3 = new TClonesArray("ERTelescopeParticle", 10);
   reco_tree->SetBranchAddress("TelescopeParticle_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y_1000020030", &t3_he3);
   TClonesArray* t4_he3 = new TClonesArray("ERTelescopeParticle", 10);
   reco_tree->SetBranchAddress("TelescopeParticle_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y_1000020030", &t4_he3);
   TClonesArray* ct_triton = new TClonesArray("ERTelescopeParticle", 10);
   reco_tree->SetBranchAddress("TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030", &ct_triton);
   
   TFile* file = new TFile(out_file, "recreate");
   TTree* tree = new TTree("analysis", "analysis");
   TLorentzVector* h7_lv_t1 = new TLorentzVector();
   tree->Branch("h7_lv_t1.", "TLorentzVector", &h7_lv_t1);
   TLorentzVector* h7_lv_t2 = new TLorentzVector();
   tree->Branch("h7_lv_t2.", "TLorentzVector", &h7_lv_t2);
   TLorentzVector* h7_lv_t3 = new TLorentzVector();
   tree->Branch("h7_lv_t3.", "TLorentzVector", &h7_lv_t3);
   TLorentzVector* h7_lv_t4 = new TLorentzVector();
   tree->Branch("h7_lv_t4.", "TLorentzVector", &h7_lv_t4);
   TLorentzVector* h3_lv_cm_t1 = new TLorentzVector();
   tree->Branch("h3_lv_cm_t1.", "TLorentzVector", &h3_lv_cm_t1);
   TLorentzVector* h3_lv_cm_t2 = new TLorentzVector();
   tree->Branch("h3_lv_cm_t2.", "TLorentzVector", &h3_lv_cm_t2);
   TLorentzVector* h3_lv_cm_t3 = new TLorentzVector();
   tree->Branch("h3_lv_cm_t3.", "TLorentzVector", &h3_lv_cm_t3);
   TLorentzVector* h3_lv_cm_t4 = new TLorentzVector();
   tree->Branch("h3_lv_cm_t4.", "TLorentzVector", &h3_lv_cm_t4);
   // event loop
   for (Long64_t i=0; i < reco_tree->GetEntriesFast(); i++) {
      reco_tree->GetEntry(i);
      h7_lv_t1->SetXYZM(0., 0., 0., 0.);
      h7_lv_t2->SetXYZM(0., 0., 0., 0.);
      h7_lv_t3->SetXYZM(0., 0., 0., 0.);
      h7_lv_t4->SetXYZM(0., 0., 0., 0.);
      h3_lv_cm_t1->SetXYZM(0., 0., 0., 0.);
      h3_lv_cm_t2->SetXYZM(0., 0., 0., 0.);
      h3_lv_cm_t3->SetXYZM(0., 0., 0., 0.);
      h3_lv_cm_t4->SetXYZM(0., 0., 0., 0.);

      const auto* he8  = beamdet_he8->GetEntriesFast() == 1 ? dynamic_cast<ERBeamDetParticle*>(beamdet_he8->At(0)) : nullptr;
      if (!he8) {
         tree->Fill();
         continue;
      }
      const auto he8_lv = he8->GetLVTarget();
      const TLorentzVector h2_lv(0., 0., 0., 1875.612);     
      const auto he3_t1 = t1_he3->GetEntriesFast() == 1 ? dynamic_cast<ERTelescopeParticle*>(t1_he3->At(0)) : 
                                                         nullptr;
      const auto he3_t2 = t2_he3->GetEntriesFast() == 1 ? dynamic_cast<ERTelescopeParticle*>(t2_he3->At(0)) : 
                                                         nullptr;
      const auto he3_t3 = t3_he3->GetEntriesFast() == 1 ? dynamic_cast<ERTelescopeParticle*>(t3_he3->At(0)) : 
                                                         nullptr;
      const auto he3_t4 = t4_he3->GetEntriesFast() == 1 ? dynamic_cast<ERTelescopeParticle*>(t4_he3->At(0)) : 
                                                         nullptr;
      auto calc_h7 = [&he8_lv, &h2_lv](const ERTelescopeParticle* he3, TLorentzVector* h7_lv) {
         if (!he3)
            return;
         const auto he3_lv = he3->GetLVInteraction();                                       
         *h7_lv = he8_lv + h2_lv - he3_lv;
      };
      calc_h7(he3_t1, h7_lv_t1);
      calc_h7(he3_t2, h7_lv_t2);
      calc_h7(he3_t3, h7_lv_t3);
      calc_h7(he3_t4, h7_lv_t4);
      
      const auto triton = ct_triton->GetEntriesFast() == 1 ? dynamic_cast<ERTelescopeParticle*>(ct_triton->At(0)) : 
                                                         nullptr;
      if (triton) {
         const auto h3_lv = triton->GetLVInteraction();
         *h3_lv_cm_t1 = h3_lv;
         *h3_lv_cm_t2 = h3_lv;
         *h3_lv_cm_t3 = h3_lv;
         *h3_lv_cm_t4 = h3_lv;
         auto calc_h3_cm = [](const ERTelescopeParticle* he3, TLorentzVector* h7,  TLorentzVector* h3) {
            if (!he3)
               return;
            h3->Boost(-h7->BoostVector());
         };
         calc_h3_cm(he3_t1, h7_lv_t1, h3_lv_cm_t1);
         calc_h3_cm(he3_t2, h7_lv_t2, h3_lv_cm_t2);
         calc_h3_cm(he3_t3, h7_lv_t3, h3_lv_cm_t3);
         calc_h3_cm(he3_t4, h7_lv_t4, h3_lv_cm_t4);
      }
      tree->Fill();
   }
   tree->Write();
   file->Write();
}