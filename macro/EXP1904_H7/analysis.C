
void calc_mm(TFile* input, TFile* output, TString tree_name, TString out_tree_name, TString branch_with_he, TString result_name) {
   TTree* tree = (TTree*)input->Get(tree_name);
   TClonesArray* he8_ar = new TClonesArray("ERBeamDetParticle", 10);
   tree->SetBranchAddress("BeamDetParticle.",&he8_ar);
   TClonesArray* he_ar = new TClonesArray("ERTelescopeParticle", 10);
   tree->SetBranchAddress(branch_with_he, &he_ar);
   TClonesArray* triton_ar = new TClonesArray("ERTelescopeParticle", 10);
   tree->SetBranchAddress("TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030", &triton_ar);
   TClonesArray* neutron_ar = new TClonesArray("ERNDParticle", 10);
   tree->SetBranchAddress("NDParticle", &neutron_ar);
   output->cd();
   TTree* output_tree = new TTree(out_tree_name, out_tree_name);
   TLorentzVector* h7_lv = new TLorentzVector();
   output_tree->Branch(result_name, "TLorentzVector", &h7_lv);
   TLorentzVector* h7_lv_cm = new TLorentzVector();
   output_tree->Branch(result_name + "_in_cm", "TLorentzVector", &h7_lv_cm);
   TLorentzVector* he8_lv_in_cm = new TLorentzVector();
   output_tree->Branch("he8_lv_in_cm", "TLorentzVector", &he8_lv_in_cm);
   TLorentzVector* h3_lv_in_cm = new TLorentzVector();
   output_tree->Branch("h3_lv_in_cm", "TLorentzVector", &h3_lv_in_cm);
   TLorentzVector* n_lv_in_cm = new TLorentzVector();
   output_tree->Branch("n_lv_in_cm", "TLorentzVector", &n_lv_in_cm);
   Float_t theta_cm;
   output_tree->Branch("theta_cm", &theta_cm, "theta_cm/F");
   
   
   for (Long64_t i=0; i < tree->GetEntriesFast(); i++) {
      tree->GetEntry(i);
      const auto* he8  = he8_ar->GetEntriesFast() == 1 ? dynamic_cast<ERBeamDetParticle*>(he8_ar->At(0)) : nullptr;
      if (!he8) {
         std::cerr << "He8 not found in entry!" << std::endl;
         exit(-1);
      }
      const auto he8_lv = he8->GetLVTarget();
      
      const auto he = he_ar->GetEntriesFast() == 1 ? dynamic_cast<ERTelescopeParticle*>(he_ar->At(0)) : nullptr;
      if (!he) {
         std::cerr << "He3 or He4 not found in entry!" << std::endl;
         exit(-1);
      }
      const auto he_lv = he->GetLVInteraction();  
      const TLorentzVector h2_lv(0., 0., 0., 1875.612);
      *h7_lv = he8_lv + h2_lv - he_lv;

      *h7_lv_cm = *h7_lv;
      h7_lv_cm->Boost(-(h2_lv + he8_lv).BoostVector());
      *he8_lv_in_cm = he8_lv;
      he8_lv_in_cm->Boost(-(h2_lv + he8_lv).BoostVector());
      theta_cm = h7_lv_cm->Angle(he8_lv_in_cm->Vect()) * 180. /  TMath::Pi();
      const auto triton = triton_ar->GetEntriesFast() == 1 ? dynamic_cast<ERTelescopeParticle*>(triton_ar->At(0)) : nullptr;
      if (!triton) {
         std::cerr << "H3 not found in entry!" << std::endl;
         exit(-1);
      }
      const auto h3_lv = triton->GetLVInteraction();
      *h3_lv_in_cm = h3_lv;
      h3_lv_in_cm->Boost(-h7_lv->BoostVector());

      n_lv_in_cm->SetXYZT(0, 0, 0, 0);
      const auto neutron = neutron_ar->GetEntriesFast() == 1 ? dynamic_cast<ERNDParticle*>(neutron_ar->At(0)) : nullptr;
      if (neutron) {
         const auto n_lv = neutron->LV();
         *n_lv_in_cm = n_lv;
         n_lv_in_cm->Boost(-h7_lv->BoostVector());
      }
      output_tree->Fill();
   }
}

void analysis(TString input_name, TString output_name) { 
   // input
   TFile* input = new TFile(input_name);
   TFile* output = new TFile(output_name, "recreate");
   calc_mm(input, output, "t1", "t1_h7", "TelescopeParticle_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y_1000020030", "h7_lv");
   calc_mm(input, output, "t2", "t2_h7", "TelescopeParticle_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y_1000020030", "h7_lv");
   calc_mm(input, output, "t3", "t3_h7", "TelescopeParticle_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y_1000020030", "h7_lv");
   calc_mm(input, output, "t4", "t4_h7", "TelescopeParticle_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y_1000020030", "h7_lv");
   calc_mm(input, output, "t1", "t1_h6", "TelescopeParticle_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y_1000020040", "h6_lv");
   calc_mm(input, output, "t2", "t2_h6", "TelescopeParticle_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y_1000020040", "h6_lv");
   calc_mm(input, output, "t3", "t3_h6", "TelescopeParticle_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y_1000020040", "h6_lv");
   calc_mm(input, output, "t4", "t4_h6", "TelescopeParticle_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y_1000020040", "h6_lv");
   output->Write();
}