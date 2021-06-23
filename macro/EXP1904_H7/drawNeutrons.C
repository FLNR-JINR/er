#include "./cuts/scripts/create_IDs.C"
#include "./cuts/er_cuts.C"

TTree* setup_tree(int telescope, TString tree_name, TString ana_tree_name, TString he_branch_name, TString mm_alias) {
    TChain* h7_by_t1 = new TChain(ana_tree_name);
    h7_by_t1->Add("analysis*.root");
    TChain* er_h7_by_t1 = new TChain(tree_name);
    er_h7_by_t1->Add("selected*.root");
    h7_by_t1->AddFriend(er_h7_by_t1);
    h7_by_t1->SetAlias("mm", mm_alias);
    h7_by_t1->SetAlias("triton_ekin_in_cm", "h3_lv_in_cm.T() - h3_lv_in_cm.Mag()");
    h7_by_t1->SetAlias("neutron_ekin_in_cm", "n_lv_in_cm.T() - n_lv_in_cm.Mag()");
    TString alias;
    alias.Form("a20_%d_un", telescope);
    h7_by_t1->SetAlias(alias, he_branch_name + ".fEdepInThinStation");
    alias.Form("a20_%d", telescope);
    h7_by_t1->SetAlias(alias, he_branch_name + ".fCorrectedEdepInThinStation");
    alias.Form("a1_%d", telescope);
    h7_by_t1->SetAlias("a20_ch", he_branch_name + ".fChannelOfThinStation");
    h7_by_t1->SetAlias(alias, he_branch_name + ".fEdepInThickStation");
    h7_by_t1->SetAlias("DSD", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThinStation");
    h7_by_t1->SetAlias("CsI", "TelescopeParticle_Central_telescope_DoubleSi_DSD_XY_1000010030.fEdepInThickStation");
    h7_by_t1->SetAlias("ND_amp", "NDTrack.fEdep");
    h7_by_t1->SetAlias("ND_tac", "NDTrack.fTAC");
    h7_by_t1->SetMarkerStyle(3);
    return h7_by_t1;
}

void drawNeutrons() {
    create_IDs();
    

    auto* h6_by_t1 = setup_tree(1, "t1", "t1_h6", "TelescopeParticle_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y_1000020040", "h6_lv.Mag() - 3*939.565 - 2808.920");
    auto* h6_by_t2 = setup_tree(2, "t2", "t2_h6", "TelescopeParticle_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y_1000020040", "h6_lv.Mag() - 3*939.565 - 2808.920");
    auto* h6_by_t3 = setup_tree(3, "t3", "t3_h6", "TelescopeParticle_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y_1000020040", "h6_lv.Mag() - 3*939.565 - 2808.920");
    auto* h6_by_t4 = setup_tree(4, "t4", "t4_h6", "TelescopeParticle_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y_1000020040", "h6_lv.Mag() - 3*939.565 - 2808.920");

    TCanvas* c = new TCanvas();
    c->Divide(2, 2);
    c->cd(1);
    h6_by_t1->SetMarkerStyle(3);
    h6_by_t1->Draw("ND_tac : ND_amp >> tac_vs_amp");
    h6_by_t2->Draw("ND_tac : ND_amp >> +tac_vs_amp");
    h6_by_t3->Draw("ND_tac : ND_amp >> +tac_vs_amp");
    h6_by_t4->Draw("ND_tac : ND_amp >> +tac_vs_amp");
    cutNeutron->Draw("same");
    c->cd(2);
    h6_by_t1->Draw("NDParticle.fLV.T() - NDParticle.fLV.Mag() >> Ekin_cut(32, 0, 80)", "neutron_cutg && cuthe4_1");
    h6_by_t2->Draw("NDParticle.fLV.T() - NDParticle.fLV.Mag() >> +Ekin_cut", "neutron_cutg && cuthe4_2");
    h6_by_t3->Draw("NDParticle.fLV.T() - NDParticle.fLV.Mag() >> +Ekin_cut", "neutron_cutg && cuthe4_3");
    h6_by_t4->Draw("NDParticle.fLV.T() - NDParticle.fLV.Mag() >> +Ekin_cut", "neutron_cutg && cuthe4_4");
    c->cd(3);
    h6_by_t1->Draw("neutron_ekin_in_cm >> Ekin_cm", "neutron_cutg && cuthe4_1");
    h6_by_t2->Draw("neutron_ekin_in_cm >> +Ekin_cm", "neutron_cutg && cuthe4_2");
    h6_by_t3->Draw("neutron_ekin_in_cm >> +Ekin_cm", "neutron_cutg && cuthe4_3");
    h6_by_t4->Draw("neutron_ekin_in_cm >> +Ekin_cm", "neutron_cutg && cuthe4_4");
    c->cd(4);
    h6_by_t1->SetMarkerStyle(3);
    TGraph *g = new TGraph(2);
    g->SetPoint(0, 0, 0);
    g->SetPoint(1, 30, 5./6. * 30.);
    g->Draw();

    TLine *l5 = new TLine(0, 0, 30, 5./6. * 30.);
    l5->SetLineColor(kRed);
    l5->Draw("same");

    h6_by_t1->Draw("neutron_ekin_in_cm:mm >> ekin_vs_mm", "neutron_cutg && cuthe4_1", "same");
    h6_by_t2->Draw("neutron_ekin_in_cm:mm >> +ekin_vs_mm", "neutron_cutg && cuthe4_2", "same");
    h6_by_t3->Draw("neutron_ekin_in_cm:mm >> +ekin_vs_mm", "neutron_cutg && cuthe4_3", "same");
    h6_by_t4->Draw("neutron_ekin_in_cm:mm >> +ekin_vs_mm", "neutron_cutg  && cuthe4_4", "same");
    
}