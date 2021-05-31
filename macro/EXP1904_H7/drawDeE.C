#include "./cuts/scripts/create_IDs.C"


void drawDeE() {
    create_he3_T1_cuts();
    create_triton_cuts();
    TFile* f = TFile::Open("test.root","RECREATE");

    auto c = new TCanvas("c", "c");
    c->Divide(2, 2);
    c->cd(1);
    TChain* t_er = new TChain("er");
    t_er->Add("results/h7_ct_*.lmd.cleaned.root");
    TChain* reco_er = new TChain("er");
    reco_er->Add("results/h7_ct_*.lmd.reco.root");
    t_er->AddFriend(reco_er);
    TChain* h7 = new TChain("analysis");
    h7->Add("results/h7_ct_*.lmd.analysis.root");
    t_er->AddFriend(h7);
    t_er->SetAlias("a20_1_un", "TelescopeDigi_Telescope_1_SingleSi_SSD20_1_X.fEdep");
    t_er->SetAlias("a20_1", "TelescopeParticle_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("mult1", "Length$(TelescopeDigi_Telescope_1_SingleSi_SSD20_1_X) == 1");
    t_er->SetAlias("trigger1", "EventHeader.fTrigger != 1");
    t_er->SetAlias("a1_1", "TelescopeDigi_Telescope_1_SingleSi_SSD_1_Y.fEdep");
    t_er->SetAlias("mult_1", "Length$(TelescopeDigi_Telescope_1_SingleSi_SSD_1_Y) == 1");
    t_er->SetAlias("time_1", "TelescopeDigi_Telescope_1_SingleSi_SSD_1_Y_1.fTime > 0.");
    t_er->SetAlias("fXt", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fTargetVertex.X()");
    t_er->SetAlias("fYt", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fTargetVertex.Y()");
    t_er->SetAlias("S20x", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fXStationLocalVertex.X()");
    t_er->SetAlias("SSDy", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fYStationLocalVertex.X()");
    t_er->SetAlias("track", "(TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.GetDirection().Theta() - TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fTargetVertex)");
    t_er->SetAlias("th_he3_1", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.GetDirection().Theta()");
    t_er->SetAlias("x20_1", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fXStationVertex.X()");

    t_er->SetAlias("a20_2_un", "TelescopeDigi_Telescope_2_SingleSi_SSD20_2_Y.fEdep");
    t_er->SetAlias("a20_2", "TelescopeParticle_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("mult2", "Length$(TelescopeDigi_Telescope_2_SingleSi_SSD20_2_Y) == 1");
    t_er->SetAlias("a1_2", "TelescopeDigi_Telescope_2_SingleSi_SSD_2_X.fEdep");
    t_er->SetAlias("mult_2", "Length$(TelescopeDigi_Telescope_2_SingleSi_SSD_2_X) == 1");


    t_er->SetAlias("a20_3_un", "TelescopeDigi_Telescope_3_SingleSi_SSD20_3_X.fEdep");
    t_er->SetAlias("a20_3", "TelescopeParticle_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("mult3", "Length$(TelescopeDigi_Telescope_3_SingleSi_SSD20_3_X) == 1");
    t_er->SetAlias("a1_3", "TelescopeDigi_Telescope_3_SingleSi_SSD_3_Y.fEdep");
    t_er->SetAlias("mult_3", "Length$(TelescopeDigi_Telescope_3_SingleSi_SSD_3_Y) == 1");

    t_er->SetAlias("a20_4_un", "TelescopeDigi_Telescope_4_SingleSi_SSD20_4_Y.fEdep");
    t_er->SetAlias("a20_4", "TelescopeParticle_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("mult4", "Length$(TelescopeDigi_Telescope_4_SingleSi_SSD20_4_Y) == 1");
    t_er->SetAlias("a1_4", "TelescopeDigi_Telescope_4_SingleSi_SSD_4_X.fEdep");
    t_er->SetAlias("mult_4", "Length$(TelescopeDigi_Telescope_4_SingleSi_SSD_4_X) == 1");

    t_er->SetAlias("X_C.", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThinStation");
    t_er->SetAlias("aCsI", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThickStation");

    t_er->SetAlias("target_position", "((fXt)*(fXt) + (fYt)*(fYt))<1.*1.");

    t_er->SetAlias("MM_t1", "h7_lv_t1.Mag() - 4*939.565 - 2808.920");

    t_er->SetAlias("triton_ekin_in_cm_t1", "h3_lv_cm_t1.T() - h3_lv_cm_t1.Mag()");

    c->cd(1);
    t_er->SetMarkerStyle(3);
    Int_t nentries = t_er->Draw("a20_1 : a1_1 + a20_1_un", "mult1 && mult_1 && target_position && a20_1 < 20. && a20_1 > 0. && cuthe3_1_0");
    std::cout << nentries << std::endl;
    c->cd(2);
    nentries = t_er->Draw("X_C. : aCsI", "cut3h_0");
    //nentries = t_er->Draw("a20_2 : a1_2 + a20_2_un", "mult2 && mult_2 && ((fXt)*(fXt) + (fYt)*(fYt))<1.*1. && a20_2 < 20. && a20_2 > 0.");
    std::cout << nentries << std::endl;
    c->cd(3);
    nentries = t_er->Draw("MM_t1", "mult1 && mult_1 && target_position && a20_1 < 20. && a20_1 > 0. && cut3h_0 && cuthe3_1_0");
    //nentries = t_er->Draw("a20_3 : a1_3 + a20_3_un", "mult3 && mult_3 && ((fXt)*(fXt) + (fYt)*(fYt))<1.*1. && a20_3 < 20. && a20_3 > 0.");
    std::cout << nentries << std::endl;
    c->cd(4);
    //nentries = t_er->Draw("a20_4 : a1_4 + a20_4_un", "mult4 && mult_4 && ((fXt)*(fXt) + (fYt)*(fYt))<1.*1. && a20_4 < 20. && a20_4 > 0.");
    nentries = t_er->Draw("MM_t1 : triton_ekin_in_cm_t1", "mult1 && mult_1 && target_position && a20_1 < 20. && a20_1 > 0. && cut3h_0 && cuthe3_1_0");
    std::cout << nentries << std::endl;
    c->Write();
    f->Write();
}