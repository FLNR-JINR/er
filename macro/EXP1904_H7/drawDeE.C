#include "./cuts/scripts/create_IDs.C"
#include "./cuts/er_cuts.C"



void drawDeE() {
    create_IDs();

    auto c = new TCanvas("c", "c");
    c->Divide(4, 3);
    c->cd(1);
    TChain* t_er = new TChain("er");
    t_er->Add("results/h7_ct_*.lmd.reco.root");
    TChain* reco_er = new TChain("er");
    reco_er->Add("results/h7_ct_*.lmd.cleaned.root");
    t_er->AddFriend(reco_er);
    t_er->SetAlias("trigger1", "EventHeader.fTrigger != 1");

    t_er->SetAlias("fXt", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fTargetVertex.X()");
    t_er->SetAlias("fYt", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fTargetVertex.Y()");
    t_er->SetAlias("target_position", "((fXt)*(fXt) + (fYt)*(fYt))<1.*1.");

    t_er->SetAlias("a20_1_un", "TelescopeDigi_Telescope_1_SingleSi_SSD20_1_X.fEdep");
    t_er->SetAlias("a20_1", "TelescopeParticle_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("a1_1", "TelescopeDigi_Telescope_1_SingleSi_SSD_1_Y.fEdep");
    t_er->SetAlias("mult1_a20", "Length$(TelescopeDigi_Telescope_1_SingleSi_SSD20_1_X) == 1");
    t_er->SetAlias("mult1_a", "Length$(TelescopeDigi_Telescope_1_SingleSi_SSD_1_Y) == 1");
    t_er->SetAlias("a20_1_ch", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fXChannel"); 
    
    t_er->SetAlias("a20_2_un", "TelescopeDigi_Telescope_2_SingleSi_SSD20_2_Y.fEdep");
    t_er->SetAlias("a20_2", "TelescopeParticle_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("a1_2", "TelescopeDigi_Telescope_2_SingleSi_SSD_2_X.fEdep");
    t_er->SetAlias("mult2_a20", "Length$(TelescopeDigi_Telescope_2_SingleSi_SSD20_2_Y) == 1");
    t_er->SetAlias("mult2_a", "Length$(TelescopeDigi_Telescope_2_SingleSi_SSD_2_X) == 1");
    t_er->SetAlias("a20_2_ch", "TelescopeTrack_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y.fYChannel");

    t_er->SetAlias("a20_3_un", "TelescopeDigi_Telescope_3_SingleSi_SSD20_3_X.fEdep");
    t_er->SetAlias("a20_3", "TelescopeParticle_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("a1_3", "TelescopeDigi_Telescope_3_SingleSi_SSD_3_Y.fEdep");
    t_er->SetAlias("mult3_a20", "Length$(TelescopeDigi_Telescope_3_SingleSi_SSD20_3_X) == 1");
    t_er->SetAlias("mult3_a", "Length$(TelescopeDigi_Telescope_3_SingleSi_SSD_3_Y) == 1");
    t_er->SetAlias("a20_3_ch", "TelescopeTrack_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y.fXChannel");

    t_er->SetAlias("a20_4_un", "TelescopeDigi_Telescope_4_SingleSi_SSD20_4_Y.fEdep");
    t_er->SetAlias("a20_4", "TelescopeParticle_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("a1_4", "TelescopeDigi_Telescope_4_SingleSi_SSD_4_X.fEdep");
    t_er->SetAlias("mult4_a20", "Length$(TelescopeDigi_Telescope_4_SingleSi_SSD20_4_Y) == 1");
    t_er->SetAlias("mult4_a", "Length$(TelescopeDigi_Telescope_4_SingleSi_SSD_4_X) == 1");
    t_er->SetAlias("a20_4_ch", "TelescopeTrack_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y.fYChannel");

    t_er->SetAlias("X_C.", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThinStation");
    t_er->SetAlias("X_C", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThinStation");
    t_er->SetAlias("aCsI", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThickStation");
    t_er->SetAlias("CsI_ch", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fChannelOfThickStation"); 

    auto h3_cut = create_h3_cut();
    TFile* f = TFile::Open("deE.root","RECREATE");
    c->cd(1);
    Int_t nentries = t_er->Draw("a20_1 : a1_1 + a20_1_un", TString("mult1_a20 && mult1_a && target_position && ") + create_he3_cut(1));
    std::cout << nentries << std::endl;
    c->cd(2);
    nentries = t_er->Draw("a20_2 : a1_2 + a20_2_un", TString("mult2_a20 && mult2_a && target_position"));
    cuthe3_2[0]->Draw("same");
    std::cout << nentries << std::endl;
    c->cd(3);
    nentries = t_er->Draw("a20_3 : a1_3 + a20_3_un", TString("mult3_a20 && mult3_a && target_position && ") + create_he3_cut(3));
    std::cout << nentries << std::endl;
    c->cd(4);
    nentries = t_er->Draw("a20_4 : a1_4 + a20_4_un", TString("mult4_a20 && mult4_a && target_position && ") + create_he3_cut(4));
    std::cout << nentries << std::endl;
    c->cd(5);
    nentries = t_er->Draw("a20_1 : a1_1 + a20_1_un", "mult1_a20 && mult1_a && target_position && cuthe4_1");
    std::cout << nentries << std::endl;
    c->cd(6);
    nentries = t_er->Draw("a20_2 : a1_2 + a20_2_un", "mult2_a20 && mult2_a && target_position && cuthe4_2");
    std::cout << nentries << std::endl;
    c->cd(7);
    nentries = t_er->Draw("a20_3 : a1_3 + a20_3_un", "mult3_a20 && mult3_a && target_position && cuthe4_3");
    std::cout << nentries << std::endl;
    c->cd(8);
    nentries = t_er->Draw("a20_4 : a1_4 + a20_4_un", "mult4_a20 && mult4_a && target_position && cuthe4_4");
    std::cout << nentries << std::endl;
    c->cd(9);
    nentries = t_er->Draw("X_C. : aCsI", h3_cut);
    std::cout << nentries << std::endl;
    c->Write();
    f->Write();
}