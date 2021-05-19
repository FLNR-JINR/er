void drawDeE() {
    auto c = new TCanvas("c", "c");
    c->Divide(2, 2);
    c->cd(1);
    TChain* t = new TChain("tree");
    t->Add("ivan_cut/h7_ct_00_000*.cut.root");
    t->SetMarkerStyle(3);
    int nentries;
    //nentries = t->Draw("a20_1_un", "a20_1_un > 0.2 && (t20_1-tF5 > 0 && t20_1-tF5 < 60)");
    nentries = t->Draw("a20_1:a1_1 + a20_1_un", "a1_1 > 0.2 && a20_1_un > 0.2 && (t20_1-tF5 > 0 && t20_1-tF5 < 60) &&  (t1_1-tF5 > 0 && t1_1-tF5 < 60) && ((fXt)*(fXt) + (fYt)*(fYt))<10*10");
    //nentries = t->Draw("th_he3_1", "a1_1 > 0.2 && a20_1_un > 0.2 && (t20_1-tF5 > 0 && t20_1-tF5 < 60) &&  (t1_1-tF5 > 0 && t1_1-tF5 < 60) && ((fXt)*(fXt) + (fYt)*(fYt))<10*10 && flag1");
    //nentries = t->Draw("x20_1", "a1_1 > 0.2 && a20_1_un > 0.2 && (t20_1-tF5 > 0 && t20_1-tF5 < 60) &&  (t1_1-tF5 > 0 && t1_1-tF5 < 60) && ((fXt)*(fXt) + (fYt)*(fYt))<10*10 && flag1");
    //nentries = t->Draw("n20_1", "n20_1 > -1");

    //nentries = t->Draw("a20_1_un:a1_1", "flag1");
    //nentries = t->Draw("fXt", "");
    std::cout << nentries << std::endl;

    TChain* t_er = new TChain("er");
    t_er->Add("cleaned/h7_ct_00_000*.cleaned.root");
    TChain* reco_er = new TChain("er");
    reco_er->Add("reco_wo_dead/h7_ct_00_000*.reco.root");
    t_er->AddFriend(reco_er);
    t_er->SetAlias("a20_1_un", "ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD20_1_X_0.fEdep");
    t_er->SetAlias("a20_1", "ERQTelescopeParticle_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("mult1", "Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD20_1_X_0) == 1");
    t_er->SetAlias("time1", "ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD20_1_X_0.fTime > 0.");
    t_er->SetAlias("trigger1", "EventHeader.fTrigger != 1");
    t_er->SetAlias("a1_1", "ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_1_Y_1.fEdep");
    t_er->SetAlias("mult_1", "Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_1_Y_1) == 1");
    t_er->SetAlias("time_1", "ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_1_Y_1.fTime > 0.");
    t_er->SetAlias("fXt", "ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fTargetVertex.X()");
    t_er->SetAlias("fYt", "ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fTargetVertex.Y()");
    t_er->SetAlias("S20x", "ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fXStationLocalVertex.X()");
    t_er->SetAlias("SSDy", "ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fYStationLocalVertex.X()");
    t_er->SetAlias("track", "(ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.GetDirection().Theta() - ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fTargetVertex)");
    t_er->SetAlias("th_he3_1", "ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.GetDirection().Theta()");
    t_er->SetAlias("x20_1", "ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fXStationVertex.X()");

    t_er->SetAlias("a20_2_un", "ERQTelescopeSiDigi_Telescope_2_SingleSi_SSD20_2_Y_3.fEdep");
    t_er->SetAlias("a20_2", "ERQTelescopeParticle_Telescope_2_SingleSi_SSD_2_X_4Telescope_2_SingleSi_SSD20_2_Y_3_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("mult2", "Length$(ERQTelescopeSiDigi_Telescope_2_SingleSi_SSD20_2_Y_3) == 1");
    t_er->SetAlias("a1_2", "ERQTelescopeSiDigi_Telescope_2_SingleSi_SSD_2_X_4.fEdep");
    t_er->SetAlias("mult_2", "Length$(ERQTelescopeSiDigi_Telescope_2_SingleSi_SSD_2_X_4) == 1");


    t_er->SetAlias("a20_3_un", "ERQTelescopeSiDigi_Telescope_3_SingleSi_SSD20_3_X_6.fEdep");
    t_er->SetAlias("a20_3", "ERQTelescopeParticle_Telescope_3_SingleSi_SSD20_3_X_6Telescope_3_SingleSi_SSD_3_Y_7_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("mult3", "Length$(ERQTelescopeSiDigi_Telescope_3_SingleSi_SSD20_3_X_6) == 1");
    t_er->SetAlias("a1_3", "ERQTelescopeSiDigi_Telescope_3_SingleSi_SSD_3_Y_7.fEdep");
    t_er->SetAlias("mult_3", "Length$(ERQTelescopeSiDigi_Telescope_3_SingleSi_SSD_3_Y_7) == 1");

    t_er->SetAlias("a20_4_un", "ERQTelescopeSiDigi_Telescope_4_SingleSi_SSD20_4_Y_9.fEdep");
    t_er->SetAlias("a20_4", "ERQTelescopeParticle_Telescope_4_SingleSi_SSD_4_X_10Telescope_4_SingleSi_SSD20_4_Y_9_1000020030.fCorrectedEdepInThinStation");
    t_er->SetAlias("mult4", "Length$(ERQTelescopeSiDigi_Telescope_4_SingleSi_SSD20_4_Y_9) == 1");
    t_er->SetAlias("a1_4", "ERQTelescopeSiDigi_Telescope_4_SingleSi_SSD_4_X_10.fEdep");
    t_er->SetAlias("mult_4", "Length$(ERQTelescopeSiDigi_Telescope_4_SingleSi_SSD_4_X_10) == 1");


    //c->cd(2);
    t_er->SetMarkerStyle(3);
    t_er->SetMarkerColor(3);
    //nentries = t_er->Draw("a20_1_un", "mult1 && a20_1_un > 0.2");
    nentries = t_er->Draw("a20_1 : a1_1 + a20_1_un", "mult1 && mult_1 && ((fXt)*(fXt) + (fYt)*(fYt))<1.*1.", "same");
    std::cout << nentries << std::endl;
    c->cd(2);
    nentries = t->Draw("a20_2:a1_2 + a20_2_un", "a1_2 > 0.2 && a20_2_un > 0.2 && (t20_2-tF5 > 0 && t20_2-tF5 < 60) &&  (t1_2-tF5 > 0 && t1_2-tF5 < 60) && ((fXt)*(fXt) + (fYt)*(fYt))<10*10");
    std::cout << nentries << std::endl;
    nentries = t_er->Draw("a20_2 : a1_2 + a20_2_un", "mult2 && mult_2 && ((fXt)*(fXt) + (fYt)*(fYt))<1.*1.", "same");
    std::cout << nentries << std::endl;
    c->cd(3);
    nentries = t->Draw("a20_3:a1_3 + a20_3_un", "a1_3 > 0.2 && a20_3_un > 0.2 && (t20_3-tF5 > 0 && t20_3-tF5 < 60) &&  (t1_3-tF5 > 0 && t1_3-tF5 < 60) && ((fXt)*(fXt) + (fYt)*(fYt))<10*10");
    std::cout << nentries << std::endl;
    nentries = t_er->Draw("a20_3 : a1_3 + a20_3_un", "mult3 && mult_3 && ((fXt)*(fXt) + (fYt)*(fYt))<1.*1.", "same");
    std::cout << nentries << std::endl;
    c->cd(4);
    nentries = t->Draw("a20_4:a1_4 + a20_4_un", "a1_4 > 0.2 && a20_4_un > 0.2 && (t20_4-tF5 > 0 && t20_4-tF5 < 60) &&  (t1_4-tF5 > -100 && t1_4-tF5 < 200) && ((fXt)*(fXt) + (fYt)*(fYt))<10*10");
    std::cout << nentries << std::endl;
    nentries = t_er->Draw("a20_4 : a1_4 + a20_4_un", "mult4 && mult_4 && ((fXt)*(fXt) + (fYt)*(fYt))<1.*1.", "same");
    std::cout << nentries << std::endl;

    //nentries = t_er->Draw("cos(th_he3_1)", "mult1 && mult_1 && ((fXt)*(fXt) + (fYt)*(fYt))<1.*1. && SSDy > -3 + 6./16.");
    //nentries = t_er->Draw("x20_1", "mult1 && mult_1 && ((fXt)*(fXt) + (fYt)*(fYt))<1.*1. && SSDy > -3 + 6./16.");
    //nentries = t_er->Draw("SSDy", "");
}