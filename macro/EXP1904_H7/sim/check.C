void check_entries_number(TTree* tree, const TString& selection, unsigned int number) {
    if (tree->GetEntries(selection) != number) {
        std::cerr << "Wrong entries in selection " << selection << std::endl;
        exit(1);
    }
}

void check() {
    auto* sim = new TFile("sim_digi.root");
    auto* sim_tree = dynamic_cast<TTree*>(sim->Get("er"));
    auto* reco = new TFile("reco.root");
    auto* reco_tree = dynamic_cast<TTree*>(reco->Get("er"));
    check_entries_number(sim_tree, "BeamDetToFDigi1.fEdep > 0.", 1000);
    check_entries_number(sim_tree, "BeamDetToFDigi2.fEdep > 0.", 1000);
    check_entries_number(sim_tree, "BeamDetMWPCDigiX1.fEdep > 0", 1000);
    check_entries_number(sim_tree, "BeamDetMWPCDigiY1.fEdep > 0", 1000);
    check_entries_number(sim_tree, "BeamDetMWPCDigiX2.fEdep > 0", 1000);
    check_entries_number(sim_tree, "BeamDetMWPCDigiY2.fEdep > 0", 1000);
    check_entries_number(sim_tree, "NDDigi.fEdep > 0", 18);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_1_SingleSi_SSD20_1_X.fEdep > 0.", 52);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_1_SingleSi_SSD_1_Y.fEdep > 0.", 87);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_1_SingleSi_SSD_V_1_Y.fEdep > 0.", 83);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_2_SingleSi_SSD20_2_Y.fEdep > 0.", 61);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_2_SingleSi_SSD_2_X.fEdep > 0.", 88);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_2_SingleSi_SSD_V_2_X.fEdep > 0.", 86);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_3_SingleSi_SSD20_3_X.fEdep > 0.", 63);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_3_SingleSi_SSD_3_Y.fEdep > 0.", 94);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_3_SingleSi_SSD_V_3_Y.fEdep > 0.", 91);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_3_SingleSi_SSD_V_3_Y.fEdep > 0.", 91);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_4_SingleSi_SSD20_4_Y.fEdep > 0.", 56);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_4_SingleSi_SSD_4_X.fEdep > 0.", 93);
    check_entries_number(sim_tree, "TelescopeDigi_Telescope_4_SingleSi_SSD_V_4_X.fEdep > 0.", 94);
    check_entries_number(sim_tree, "TelescopeDigi_Central_telescope_DoubleSi_DSD_XY_X.fEdep > 0.", 715);
    check_entries_number(sim_tree, "TelescopeDigi_Central_telescope_DoubleSi_DSD_XY_Y.fEdep > 0.", 715);
    check_entries_number(sim_tree, "TelescopeDigi_Central_telescope_CsI.fEdep > 0.", 704);
    check_entries_number(reco_tree, "BeamDetTrack@.GetEntries()", 985);
    check_entries_number(reco_tree, "BeamDetParticle@.GetEntries()", 985);
    check_entries_number(reco_tree, "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y@.GetEntries()", 52);
    check_entries_number(reco_tree, "TelescopeTrack_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y@.GetEntries()", 61);
    check_entries_number(reco_tree, "TelescopeTrack_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y@.GetEntries()", 62);
    check_entries_number(reco_tree, "TelescopeTrack_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y@.GetEntries()", 56);
    check_entries_number(reco_tree, "TelescopeTrack_Central_telescope_DoubleSi_DSD_XY_XCentral_telescope_DoubleSi_DSD_XY_Y@.GetEntries()", 702);
    check_entries_number(reco_tree, "ERTelescopeParticle_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y_1000020030@.GetEntries()", 52);
    check_entries_number(reco_tree, "ERTelescopeParticle_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y_1000020030@.GetEntries()", 61);
    check_entries_number(reco_tree, "ERTelescopeParticle_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y_1000020030@.GetEntries()", 62);
    check_entries_number(reco_tree, "ERTelescopeParticle_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y_1000020030@.GetEntries()", 56);
    check_entries_number(reco_tree, "ERTelescopeParticle_Central_telescope_DoubleSi_DSD_XY_XCentral_telescope_DoubleSi_DSD_XY_Y_1000010030@.GetEntries()", 702);
    check_entries_number(reco_tree, "NDTrack@.GetEntries()", 18);
    check_entries_number(reco_tree, "NDParticle@.GetEntries()", 18);
}