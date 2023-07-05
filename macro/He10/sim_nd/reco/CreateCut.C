TCutG *cut_8he;

void CreateCut(){
   cut_8he = new TCutG("cut_8he",24);
   cut_8he->SetVarX("TelescopeParticle_Telescope_he8_SingleSi_SSD20_XTelescope_he8_SingleSi_SSD20_1_Y_1000020080.fEdepInThinStation");
   cut_8he->SetVarY("TelescopeParticle_Telescope_he8_SingleSi_SSD20_XTelescope_he8_SingleSi_SSD20_1_Y_1000020080.fEdepInThickStation");
   cut_8he->SetTitle("Graph");
   cut_8he->SetFillColor(1);
   cut_8he->SetPoint(0,18.6774,162.562);
   cut_8he->SetPoint(1,19.4322,163.098);
   cut_8he->SetPoint(2,22.7534,136.827);
   cut_8he->SetPoint(3,25.7726,121.279);
   cut_8he->SetPoint(4,32.2639,98.7607);
   cut_8he->SetPoint(5,36.1134,90.4506);
   cut_8he->SetPoint(6,43.586,78.3874);
   cut_8he->SetPoint(7,56.1912,68.4688);
   cut_8he->SetPoint(8,63.5128,65.52);
   cut_8he->SetPoint(9,63.4373,63.9116);
   cut_8he->SetPoint(10,58.1537,65.2519);
   cut_8he->SetPoint(11,49.6244,68.4688);
   cut_8he->SetPoint(12,41.3215,75.4386);
   cut_8he->SetPoint(13,33.3206,88.306);
   cut_8he->SetPoint(14,32.2639,91.7909);
   cut_8he->SetPoint(15,31.8865,93.6674);
   cut_8he->SetPoint(16,30.075,98.2246);
   cut_8he->SetPoint(17,25.999,111.628);
   cut_8he->SetPoint(18,23.2062,126.104);
   cut_8he->SetPoint(19,20.4135,143.797);
   cut_8he->SetPoint(20,18.6019,160.685);
   cut_8he->SetPoint(21,18.6774,162.562);
   cut_8he->SetPoint(22,18.6774,162.562);
   cut_8he->SetPoint(23,18.6774,162.562);
}


