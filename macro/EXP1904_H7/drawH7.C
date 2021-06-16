
TTree* setup_tree(int telescope, TString tree_name,  TString ana_tree_name, TString he_branch_name, TString mm_alias) {
    TChain* h7_by_t1 = new TChain(ana_tree_name);
    h7_by_t1->Add("analysis*.root");
    TChain* er_h7_by_t1 = new TChain(tree_name);
    er_h7_by_t1->Add("selected*.root");
    h7_by_t1->AddFriend(er_h7_by_t1);
    h7_by_t1->SetAlias("mm", mm_alias);
    h7_by_t1->SetAlias("triton_ekin_in_cm", "h3_lv_in_cm.T() - h3_lv_in_cm.Mag()");
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
    h7_by_t1->SetMarkerStyle(3);
    return h7_by_t1;
}

#include "./cuts/scripts/create_IDs.C"
#include "./cuts/er_cuts.C"

void drawH7() {
    create_IDs();
  
    auto c = new TCanvas("c", "c");
    c->Divide(3, 4);
    TString mm = "h7_lv.Mag() - 4*939.565 - 2808.920";
    auto* h7_by_t1 = setup_tree(1, "t1", "t1_h7", "TelescopeParticle_Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y_1000020030", mm);
    auto* h7_by_t2 = setup_tree(2, "t2", "t2_h7", "TelescopeParticle_Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y_1000020030", mm);
    auto* h7_by_t3 = setup_tree(3, "t3", "t3_h7", "TelescopeParticle_Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y_1000020030", mm);
    auto* h7_by_t4 = setup_tree(4, "t4", "t4_h7", "TelescopeParticle_Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y_1000020030", mm);

    c->cd(1);
    h7_by_t1->SetLineColor(kBlue);
    h7_by_t1->Draw("mm>>mm1(25,0,25)", create_he3_cut(1));
    h7_by_t1->SetLineColor(kRed);
    h7_by_t1->Draw("mm>>mm1_cut(25,0,25)", create_he3_cut(1) + " && triton_ekin_in_cm < 4./7. * mm", "same");
    c->cd(2);
    h7_by_t1->Draw("triton_ekin_in_cm:mm", create_he3_cut(1));
    TLine *l1 = new TLine(0, 0, 30, 4./7. * 30);
    l1->SetLineColor(kRed);
    l1->Draw("same");
    c->cd(3);
    h7_by_t1->SetLineColor(kBlue);
    h7_by_t1->Draw("theta_cm:mm", create_he3_cut(1));
    
    c->cd(4);
    h7_by_t2->SetLineColor(kBlue);
    h7_by_t2->Draw("mm>>mm2(25,0,25)", create_he3_cut(2));
    h7_by_t2->SetLineColor(kRed);
    h7_by_t2->Draw("mm>>mm2_cut(25,0,25)", create_he3_cut(2) + " && triton_ekin_in_cm < 4./7. * mm", "same");
    c->cd(5);
    h7_by_t2->Draw("triton_ekin_in_cm:mm", create_he3_cut(2));
    TLine *l2 = new TLine(0, 0, 30,  4./7. * 30);
    l2->SetLineColor(kRed);
    l2->Draw("same");
    c->cd(6);
    h7_by_t2->SetLineColor(kBlue);
    h7_by_t2->Draw("theta_cm:mm",  create_he3_cut(2));

    c->cd(7);
    h7_by_t3->SetLineColor(kBlue);
    h7_by_t3->Draw("mm>>mm3(25,0,25)", create_he3_cut(3));
    h7_by_t3->SetLineColor(kRed);
    h7_by_t3->Draw("mm>>mm3_cut(25,0,25)",  create_he3_cut(3) + " && triton_ekin_in_cm < 4./7. * mm", "same");
    c->cd(8);
    h7_by_t3->Draw("triton_ekin_in_cm:mm", create_he3_cut(3));
    TLine *l3 = new TLine(0, 0, 30,  4./7. * 30);
    l3->SetLineColor(kRed);
    l3->Draw("same");
    c->cd(9);
    h7_by_t3->SetLineColor(kBlue);
    h7_by_t3->Draw("theta_cm:mm", create_he3_cut(3));

    c->cd(10);
    h7_by_t4->SetLineColor(kBlue);
    h7_by_t4->Draw("mm>>mm4(25,0,25)", create_he3_cut(4));
    h7_by_t4->SetLineColor(kRed);
    h7_by_t4->Draw("mm>>mm4_cut(25,0,25)", create_he3_cut(4) + " && triton_ekin_in_cm < 4./7. * mm", "same");
    c->cd(11);
    h7_by_t4->Draw("triton_ekin_in_cm:mm", create_he3_cut(4));
    TLine *l4 = new TLine(0, 0, 30,  4./7. * 30);
    l4->SetLineColor(kRed);
    l4->Draw("same");
    c->cd(12);
    h7_by_t4->SetLineColor(kBlue);
    h7_by_t4->Draw("theta_cm:mm", create_he3_cut(4));
    
    auto c1 = new TCanvas("c1", "c1");
    c1->Divide(2, 2);
    c1->cd(1);
    h7_by_t1->SetLineColor(kBlue);
    h7_by_t1->Draw("mm >> all_mm(25,0,25)", create_he3_cut(1));
    h7_by_t2->Draw("mm >> +all_mm", create_he3_cut(2));
    h7_by_t3->Draw("mm >> +all_mm", create_he3_cut(3));
    h7_by_t4->Draw("mm >> +all_mm", create_he3_cut(4));

    h7_by_t1->SetLineColor(kRed);
    h7_by_t1->Draw("mm >> all_mm_cut(25,0,25)", create_he3_cut(1) + " && triton_ekin_in_cm < 4./7. * mm", "same");
    h7_by_t2->Draw("mm >> +all_mm_cut", create_he3_cut(2) +  " && triton_ekin_in_cm < 4./7. * mm", "same");
    h7_by_t3->Draw("mm >> +all_mm_cut", create_he3_cut(3) + " && triton_ekin_in_cm < 4./7. * mm", "same");
    h7_by_t4->Draw("mm >> +all_mm_cut", create_he3_cut(4) +  " && triton_ekin_in_cm < 4./7. * mm", "same");

    h7_by_t1->SetLineColor(kGreen);
    h7_by_t1->Draw("mm >> all_mm_cut10(25,0,25)", create_he3_cut(1) + " && triton_ekin_in_cm < 2./7. * mm", "same");
    h7_by_t2->Draw("mm >> +all_mm_cut10", create_he3_cut(2) +  " && triton_ekin_in_cm < 2./7. * mm", "same");
    h7_by_t3->Draw("mm >> +all_mm_cut10", create_he3_cut(3) + " && triton_ekin_in_cm < 2./7. * mm", "same");
    h7_by_t4->Draw("mm >> +all_mm_cut10", create_he3_cut(4) +  " && triton_ekin_in_cm < 2./7. * mm", "same");

    c1->cd(2);

    h7_by_t1->SetLineColor(kRed);
    h7_by_t1->Draw("mm >> all_mm_cut2(25,0,25)", create_he3_cut(1) + " && triton_ekin_in_cm < 4./7. * mm");
    h7_by_t2->Draw("mm >> +all_mm_cut2", create_he3_cut(2) +  " && triton_ekin_in_cm < 4./7. * mm");
    h7_by_t3->Draw("mm >> +all_mm_cut2", create_he3_cut(3) + " && triton_ekin_in_cm < 4./7. * mm");
    h7_by_t4->Draw("mm >> +all_mm_cut2", create_he3_cut(4) +  " && triton_ekin_in_cm < 4./7. * mm");
    /*
    h7_by_t1->SetLineColor(kGreen);
    h7_by_t1->Draw("mm >> all_mm_cut3(25,0,25)", create_he3_cut(1) + " && triton_ekin_in_cm < 4./7. * mm && theta_cm < 18", "same");
    h7_by_t2->Draw("mm >> +all_mm_cut3", create_he3_cut(2) +  " && triton_ekin_in_cm < 4./7. * mm && theta_cm < 18", "same");
    h7_by_t3->Draw("mm >> +all_mm_cut3", create_he3_cut(3) + " && triton_ekin_in_cm < 4./7. * mm && theta_cm < 18", "same");
    h7_by_t4->Draw("mm >> +all_mm_cut3", create_he3_cut(4) +  " && triton_ekin_in_cm < 4./7. * mm && theta_cm < 18", "same");
    */
    c1->cd(3);
    h7_by_t1->Draw("triton_ekin_in_cm:mm", create_he3_cut(1));
    h7_by_t2->Draw("triton_ekin_in_cm:mm", create_he3_cut(2), "same");
    h7_by_t3->Draw("triton_ekin_in_cm:mm", create_he3_cut(3), "same");
    h7_by_t4->Draw("triton_ekin_in_cm:mm", create_he3_cut(4), "same");
    TLine *l5 = new TLine(0, 0, 30,  4./7. * 30);
    l5->SetLineColor(kRed);
    l5->Draw("same");

    c1->cd(4);
    h7_by_t1->Draw("theta_cm:mm", create_he3_cut(1));
    h7_by_t2->Draw("theta_cm:mm", create_he3_cut(2), "same");
    h7_by_t3->Draw("theta_cm:mm", create_he3_cut(3), "same");
    h7_by_t4->Draw("theta_cm:mm", create_he3_cut(4), "same");
}