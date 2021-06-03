#include "./cuts/scripts/create_IDs.C"

void select() {
    create_IDs();

    TChain* t_er = new TChain("er");
    t_er->Add("results/h7_ct_00_0001.lmd.reco.root");
    TChain* cleaned_er = new TChain("er");
    cleaned_er->Add("results/h7_ct_00_0001.lmd.cleaned.root");
    t_er->AddFriend(cleaned_er);

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

    auto create_he3_cut = [](int telescope_number) {
        std::map<ushort, ushort> inverse16Mapping = {
            {0, 15},
            {1, 14},
            {2, 13},
            {3, 12},
            {4, 11},
            {5, 10},
            {6, 9},
            {7, 8},
            {8, 7},
            {9, 6},
            {10, 5},
            {11, 4},
            {12, 3},
            {13, 2},
            {14, 1},
            {15, 0}
        };
        TString cut = "(";
        for (ushort i = 0; i < 16; i++) {
            TString channel_cut;
            ushort er_channel = i;
            if (telescope_number == 1 || telescope_number == 2) {
                er_channel = inverse16Mapping[i];
            }
            channel_cut.Form("(a20_%d_ch == %d && cuthe3_%d_%d)",telescope_number, er_channel, telescope_number, i);
            cut += channel_cut;
            if (i != 15)
                cut += " || ";
        }
        cut += ")";
        return cut;
    };

    auto create_h3_cut = []() {
        std::map<ushort, ushort> csiMapping = {
            {0, 15},
            {1, 11},
            {2, 7},
            {3, 3},
            {4, 14},
            {5, 10},
            {6, 6},
            {7, 2},
            {8, 13},
            {9, 9},
            {10, 5},
            {11, 1},
            {12, 12},
            {13, 8},
            {14, 4},
            {15, 0}
        };
        TString cut = "(";
        for (ushort i = 0; i < 16; i++) {
            TString channel_cut;
            const ushort er_channel = csiMapping[i];
            channel_cut.Form("(CsI_ch == %d && cut3h_%d)", er_channel, i);
            cut += channel_cut;
            if (i != 15)
                cut += " || ";
        }
        cut += ")";
        return cut;
    };
    
    TFile* f = TFile::Open("selected.root","RECREATE");

    auto h3_in_ct = create_h3_cut();
    auto he3_in_t1 = TString("mult1_a20 && mult1_a && ") + create_he3_cut(1);
    auto he3_in_t2 = TString("mult2_a20 && mult2_a && ") + create_he3_cut(2);
    auto he3_in_t3 = TString("mult3_a20 && mult3_a && ") + create_he3_cut(3);
    auto he3_in_t4 = TString("mult4_a20 && mult4_a && ") + create_he3_cut(4);
    auto he4_in_t1 = TString("mult1_a20 && mult1_a && ") + "cuthe4_1";
    auto he4_in_t2 = TString("mult2_a20 && mult2_a && ") + "cuthe4_2";
    auto he4_in_t3 = TString("mult3_a20 && mult3_a && ") + "cuthe4_3";
    auto he4_in_t4 = TString("mult4_a20 && mult4_a && ") + "cuthe4_4";

    f->cd();
    auto* h7_by_t1 = t_er->CopyTree(h3_in_ct + " && " + he3_in_t1 + " && target_position");
    h7_by_t1->SetName("h7_by_t1");
    h7_by_t1->Write();
    auto* h7_by_t2 = t_er->CopyTree(h3_in_ct + " && " + he3_in_t2 + " && target_position");
    h7_by_t2->SetName("h7_by_t2");
    h7_by_t2->Write();
    auto* h7_by_t3 = t_er->CopyTree(h3_in_ct + " && " + he3_in_t3 + " && target_position");
    h7_by_t3->SetName("h7_by_t3");
    h7_by_t3->Write();
    auto* h7_by_t4 = t_er->CopyTree(h3_in_ct + " && " + he3_in_t4 + " && target_position");
    h7_by_t4->SetName("h7_by_t4");
    h7_by_t4->Write();
    auto* h6_by_t1 = t_er->CopyTree(h3_in_ct + " && " + he4_in_t1 + " && target_position");
    h6_by_t1->SetName("h6_by_t1");
    h6_by_t1->Write();
    auto* h6_by_t2 = t_er->CopyTree(h3_in_ct + " && " + he4_in_t2 + " && target_position");
    h6_by_t2->SetName("h6_by_t2");
    h6_by_t2->Write();
    auto* h6_by_t3 = t_er->CopyTree(h3_in_ct + " && " + he4_in_t3 + " && target_position");
    h6_by_t3->SetName("h6_by_t3");
    h6_by_t3->Write();
    auto* h6_by_t4 = t_er->CopyTree(h3_in_ct + " && " + he4_in_t4 + " && target_position");
    h6_by_t4->SetName("h6_by_t4");
    h6_by_t4->Write();
    f->Write();
}