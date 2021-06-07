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
        channel_cut.Form("(a20_ch == %d && cuthe3_%d_%d)", er_channel, telescope_number, i);
        cut += channel_cut;
        if (i != 15)
            cut += " || ";
    }
    cut += ")";
    return cut;
};