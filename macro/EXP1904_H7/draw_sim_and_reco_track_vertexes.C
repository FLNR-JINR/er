void draw_sim_and_reco_track_vertexes() {
    TFile* sim = new TFile("sim_digi.root");
    TTree* t = (TTree*)sim->Get("er");
    auto c = new TCanvas("c", "c");
    c->Divide(2, 1);
    c->cd(2);
    t->AddFriend("er", "reco_sim.root");
    t->SetMarkerStyle(3);
    t->SetMarkerColor(1);
    long entries = 0;
    std::string cut_with_track = "ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD_1_Y_1.fPDG == 1000020030 && Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_V_1_Y_2) == 0 && Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD20_1_X_0) == 1 && Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_1_Y_1) == 1 && ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fXEdep > 0";
    std::string cut_wo_particle = "ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD_1_Y_1.fPDG == 1000020030  && Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_V_1_Y_2) == 0 && Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD20_1_X_0) == 1 && Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_1_Y_1) == 1 && ERQTelescopeParticle_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1_1000020030.fEdepInThinStation < 0";
    entries = t->Draw("ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD_1_Y_1.fY : ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD_1_Y_1.fX", cut_with_track.c_str());
    std::cout << entries << std::endl;
    
    t->SetMarkerColor(4);
    entries = t->Draw("ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fYStationVertex.fY : ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fYStationVertex.fX", cut_wo_particle.c_str(), "same");
    std::cout << entries << std::endl;
    t->SetMarkerColor(3);
    entries = t->Draw("ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD_1_Y_1.fY : ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD_1_Y_1.fX", cut_wo_particle.c_str(), "same");
    std::cout << entries << std::endl;

    TLine *line = new TLine(-7.92375, -0.889, -1.23625,  -0.889);
    line->SetLineColor(kRed);
    line->Draw();
    TLine *line2 = new TLine(-7.92375, 5.11, -1.23625,  5.11);
    line2->SetLineColor(kRed);
    line2->Draw();
    TLine *line3 = new TLine(-7.92375, -0.889 + 6./16./2, -1.23625,   -0.889 + 6./16./2);
    line3->SetLineColor(kRed);
    line3->Draw();
    TLine *line4 = new TLine(-7.92375,  -0.889 + 6./16., -1.23625,   -0.889 + 6./16.);
    line4->SetLineColor(kRed);
    line4->Draw();
    TLine *line5 = new TLine(-7.92375, 5.11 - 6./16./2, -1.23625,   5.11 - 6./16./2);
    line5->SetLineColor(kRed);
    line5->Draw();
    TLine *line6 = new TLine(-7.92375,  5.11 - 6./16., -1.23625,  5.11 - 6./16);
    line6->SetLineColor(kRed);
    line6->Draw();
    c->cd(1);
    t->SetMarkerColor(1);
    entries = t->Draw("ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD20_1_X_0.fY : ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD20_1_X_0.fX", cut_with_track.c_str());
    std::cout << entries << std::endl;
    t->SetMarkerColor(3);
    entries = t->Draw("ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD20_1_X_0.fY : ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD20_1_X_0.fX", cut_wo_particle.c_str(), "same");
    std::cout << entries << std::endl;
    t->SetMarkerColor(4);
    entries = t->Draw("ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fXStationVertex.fY : ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fXStationVertex.fX", cut_wo_particle.c_str(), "same");
    TLine *line7 = new TLine(-7.92375, 4.34, -1.23625,  4.34);
    line7->SetLineColor(kRed);
    line7->Draw();
    TLine *line8 = new TLine(-7.92375, -0.66, -1.23625,  -0.66);
    line8->SetLineColor(kRed);
    line8->Draw();
    /*
    c->cd(3);
    t->SetLineColor(3);
    entries = t->Draw("ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD20_1_X_0.fPy_out - ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD20_1_X_0.fPy", cut_wo_particle.c_str());
    t->SetLineColor(1);
    entries = t->Draw("ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD20_1_X_0.fPy_out - ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD20_1_X_0.fPy", cut_with_track.c_str(), "same");
    
    c->cd(3);
    t->SetMarkerColor(1);
    entries = t->Draw("BeamDetTrack.fYt : BeamDetTrack.fXt", cut_with_track.c_str());
    std::cout << entries << std::endl;
    t->SetMarkerColor(3);
    entries = t->Draw("BeamDetTrack.fYt : BeamDetTrack.fXt", cut_wo_particle.c_str(), "same");
    std::cout << entries << std::endl;
    c->cd(4);
    t->SetMarkerColor(1);
    //t->Draw("BeamDetTrack.fYt - MCEventHeader.fReactionPos.fY", "ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fXEdep > 0");
    t->SetLineColor(3);
    entries = t->Draw("((ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.fTargetVertex.X() - MCEventHeader.fReactionPos.fX)^2)^(0.5)", cut_wo_particle.c_str());
    std::cout << entries << std::endl;
    */
}