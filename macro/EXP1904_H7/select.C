#include "./cuts/scripts/create_IDs.C"
#include "./cuts/er_cuts.C"

void select(int i=0) {
    create_IDs();

    TChain* t_er = new TChain("er");
    TString input;
    input.Form("results_new/h7_ct_%d*.lmd.reco.root", i);
    t_er->Add(input);

    t_er->SetAlias("fXt", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fTargetVertex.X()");
    t_er->SetAlias("fYt", "TelescopeTrack_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y.fTargetVertex.Y()");
    t_er->SetAlias("target_position", "((fXt)*(fXt) + (fYt)*(fYt))<1.*1.");

    t_er->SetAlias("mult1", "Length$(TelescopeParticle_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y_1000020030) == 1");
    t_er->SetAlias("mult2", "Length$(TelescopeParticle_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y_1000020030) == 1");
    t_er->SetAlias("mult3", "Length$(TelescopeParticle_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y_1000020030) == 1");
    t_er->SetAlias("mult4", "Length$(TelescopeParticle_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y_1000020030) == 1");

    t_er->SetAlias("X_C.", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThinStation");
    t_er->SetAlias("X_C", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThinStation");
    t_er->SetAlias("aCsI", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThickStation");
    t_er->SetAlias("CsI_ch", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fChannelOfThickStation"); 

    TString output;
    output.Form("selected%d.root", i);
    TFile* f = TFile::Open(output,"RECREATE");

    auto h3_in_ct = create_h3_cut();

    f->cd();
    TString t1_cut = "target_position && mult1 && " + h3_in_ct;
    TString t2_cut = "target_position && mult2 && " + h3_in_ct;
    TString t3_cut = "target_position && mult3 && " + h3_in_ct;
    TString t4_cut = "target_position && mult4 && " + h3_in_ct;
    auto* t1 = t_er->CopyTree(t1_cut);
    t1->SetName("t1");
    auto* t2 = t_er->CopyTree(t2_cut);
    t2->SetName("t2");
    auto* t3 = t_er->CopyTree(t3_cut);
    t3->SetName("t3");
    auto* t4 = t_er->CopyTree(t4_cut);
    t4->SetName("t4");
    f->Write();
}