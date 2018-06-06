// void simulation_calib_strips() 
{
  // cout << "LOG" << endl;
  std::vector<TString> files = {
                                "sim_digi_calib_direct1.root",
                                "sim_digi_calib_direct4.root",
                                "sim_digi_calib_rot1.root",
                                "sim_digi_calib_rot4.root"
                               };
  // vector<Double_t> ede
  TCanvas* canvasMeanEdepByStrip = new TCanvas("MeanEdepByStrip");

  TCanvas* tmpCanvas = new TCanvas("tmpCanvas");
  tmpCanvas->cd(1);
  std::vector<TH1F*> histsMeanEdepByStrip;

  Double_t maxMean = 0.00752245; //0.00729998
  Int_t    sensDeadCount = 10;
  Int_t    deadLayersLikeSensCount = 7; // 5; 6;
  Int_t    thicknessDead = 12;
  Int_t calibrateStripCount = 32;


  Double_t a = 0.0211;
  // for (Int_t d = deadLayersLikeSensCount; d >= 7; d--) {
    histsMeanEdepByStrip.clear();
    for (auto itFile : files) {
      TFile* inFile = new TFile(itFile);
      TTree* tree = (TTree*)inFile->Get("er");


      histsMeanEdepByStrip.push_back(new TH1F("hMeanEdepByStrip",
                                              "MeanEdepByStrip",
                                              calibrateStripCount, 0, calibrateStripCount));

      TString cutToChooseStrip = "";
      TString varExp = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fEdep";

      cutToChooseStrip = "";

      for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {  
        varExp = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fEdep";
        for (Int_t deadLayerSensNb = 0; deadLayerSensNb < deadLayersLikeSensCount; deadLayerSensNb++) {
          TString tmpVarExp;
          tmpVarExp.Form(" + ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_%d_X.fEdep", sensDeadCount - deadLayerSensNb - 1);
          varExp += tmpVarExp;
        }
        cutToChooseStrip.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb==%d", stripNb);
        tmpCanvas->cd(1);
       // cutToChooseStrip += " && " + varExp + " > 0.006";
        tree->Draw(varExp+">>hTmpHistToGetStripMeanEdep", cutToChooseStrip, "goff");
        TH1F *tmpHistToGetStripMeanEdep = (TH1F*)gDirectory->Get("hTmpHistToGetStripMeanEdep");
        Double_t meanValue = tmpHistToGetStripMeanEdep->GetMean();
        Double_t maxBin = tmpHistToGetStripMeanEdep->GetMaximumBin();
        Double_t maxBinContent = tmpHistToGetStripMeanEdep->GetBinContent(maxBin);
        // cout << "mean for " << stripNb << " strip is " << meanValue << endl;
        // cout << "maxBin for " << stripNb << " strip is " << maxBin << endl;
        // cout << "maxBinContent for " << stripNb << " strip is " << maxBinContent << endl;
        histsMeanEdepByStrip.back()->SetBinContent(stripNb, meanValue * 1e3);
      }

    }
    // ifstream inFileDirect;
    // ifstream inFileRotate;
    // ofstream outFileWithB;
    // TString  outFileWithBName;
    // outFileWithBName.Form("/home/kmy/expertroot/er/macro/EXP1803/calibration/b_fix_a00214_d%d.txt", 4*(10-d));
    // Double_t    N_0;
    // Double_t    N_1;
    // Double_t    N_2;
    // Double_t    N_3;
    // inFileDirect.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/aculInspired/channelsDirect.txt");
    // inFileRotate.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/aculInspired/channelsRotate.txt");
    // outFileWithB.open(outFileWithBName.Data());
    // for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {
    //   inFileDirect >> N_0; inFileDirect >> N_1;        
    //   inFileRotate >> N_2; inFileRotate >> N_3;
    //   cout << "N_0 = " << N_0 << "; N_1 = " << N_1 << "; N_2 = " << N_2 << "; N_3 = " << N_3 << endl; 
    //   Double_t b0, b1, b2, b3;
    //   b0 = histsMeanEdepByStrip[0]->GetBinContent(stripNb) - N_0 * a;       
    //   b1 = histsMeanEdepByStrip[1]->GetBinContent(stripNb) - N_1 * a;       
    //   b2 = histsMeanEdepByStrip[2]->GetBinContent(stripNb) - N_2 * a;       
    //   b3 = histsMeanEdepByStrip[3]->GetBinContent(stripNb) - N_3 * a;   
    //   outFileWithB << fixed << setprecision(10) << b0 << setw(20) << b1 << setw(20) << b2 << setw(20) << b3 << setw(20) << endl;    
    // }
    // inFileDirect.close();
    // inFileRotate.close();
    // outFileWithB.close();
  // }

  pair<Int_t, Int_t> calibSystems;
  ifstream inFileDirect;
  ifstream inFileRotate;
  ofstream outFile;
  Double_t    N_1;
  Double_t    N_2;
  TString outFileName;
  ifstream outFile_1;
  ifstream outFile_2;
  ofstream outDiff;
  vector<TString> outFiles;

  calibSystems = {0,1};
  outFileName.Form("abCoefs_%d_%d_%d.cal", thicknessDead, calibSystems.first, calibSystems.second);
  // outFiles.push_back(outFileName);
  outFile.open(outFileName.Data());
  inFileDirect.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/aculInspired/channelsDirect.txt");
  inFileRotate.open("channelsRotate.txt");
  for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {
    inFileDirect >> N_1;
    inFileDirect >> N_2;
    Double_t E_1 = histsMeanEdepByStrip[calibSystems.first]->GetBinContent(stripNb);
    Double_t E_2 = histsMeanEdepByStrip[calibSystems.second]->GetBinContent(stripNb);
    cout << "N_1 = " << N_1 << "; N_2 = " << N_2 << "; E_1 = " << E_1 << "; E_2 = " << E_2 << endl; 
    Double_t a = (E_1 - E_2)
               / (N_1 - N_2);
    Double_t b = (N_1 * E_2 - N_2 * E_1) 
               / (N_1 - N_2);
    outFile << fixed << setprecision(10) << a << setw(20) << b << endl;
  }
  cout << endl;
  inFileDirect.close(); 
  outFile.close(); 
  // }
  calibSystems = {2,3};
  outFileName.Form("abCoefs_%d_%d_%d.cal", thicknessDead,calibSystems.first, calibSystems.second);
  outFile.open(outFileName.Data());
  inFileDirect.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/aculInspired/channelsRotate.txt");
  // inFileRotate.open("channelsRotate.txt");
  for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {
    inFileDirect >> N_1;
    inFileDirect >> N_2;
    Double_t E_1 = histsMeanEdepByStrip[calibSystems.first]->GetBinContent(stripNb);
    Double_t E_2 = histsMeanEdepByStrip[calibSystems.second]->GetBinContent(stripNb);
    cout << "N_1 = " << N_1 << "; N_2 = " << N_2 << "; E_1 = " << E_1 << "; E_2 = " << E_2 << endl; 
    Double_t a = (E_1 - E_2)
               / (N_1 - N_2);
    Double_t b = (N_1 * E_2 - N_2 * E_1) 
               / (N_1 - N_2);
    outFile << fixed << setprecision(10) << a << setw(20) << b << endl;
  }
  inFileDirect.close(); 
  outFile.close(); 


  // calibSystems = {0,2};
  // inFileDirect.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/aculInspired/channelsDirect.txt");
  // inFileRotate.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/aculInspired/channelsRotate.txt");
  // outFileName.Form("abCoefs_%d_%d_%d.cal", thicknessDead, calibSystems.first, calibSystems.second);
  // outFile.open(outFileName.Data());
  // for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {
  //   Double_t tmpTrash;    
  //   inFileDirect >> N_1;
  //   inFileDirect >> tmpTrash;
  //   inFileRotate >> N_2;
  //   inFileRotate >> tmpTrash;
  //   Double_t E_1 = histsMeanEdepByStrip[calibSystems.first]->GetBinContent(stripNb);
  //   Double_t E_2 = histsMeanEdepByStrip[calibSystems.second]->GetBinContent(stripNb);
  //   cout << "N_1 = " << N_1 << "; N_2 = " << N_2 << "; E_1 = " << E_1 << "; E_2 = " << E_2 << endl; 
  //   Double_t a = (E_1 - E_2)
  //              / (N_1 - N_2);
  //   Double_t b = (N_1 * E_2 - N_2 * E_1) 
  //              / (N_1 - N_2);
  //   outFile << fixed << setprecision(10) << a << setw(20) << b << endl;
  // }
  // inFileDirect.close(); 
  // inFileRotate.close(); 
  // outFile.close();

  // calibSystems = {1,3};
  // inFileDirect.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/aculInspired/channelsDirect.txt");
  // inFileRotate.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/aculInspired/channelsRotate.txt");
  // outFileName.Form("abCoefs_%d_%d_%d.cal", thicknessDead, calibSystems.first, calibSystems.second);
  // outFile.open(outFileName.Data());
  // for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {
  //   Double_t tmpTrash;    
  //   inFileDirect >> tmpTrash;
  //   inFileDirect >> N_1;
  //   inFileRotate >> tmpTrash;
  //   inFileRotate >> N_2;
  //   Double_t E_1 = histsMeanEdepByStrip[calibSystems.first]->GetBinContent(stripNb);
  //   Double_t E_2 = histsMeanEdepByStrip[calibSystems.second]->GetBinContent(stripNb);
  //   cout << "N_1 = " << N_1 << "; N_2 = " << N_2 << "; E_1 = " << E_1 << "; E_2 = " << E_2 << endl; 
  //   Double_t a = (E_1 - E_2)
  //              / (N_1 - N_2);
  //   Double_t b = (N_1 * E_2 - N_2 * E_1) 
  //              / (N_1 - N_2);
  //   outFile << fixed << setprecision(10) << a << setw(20) << b << endl;
  // }
  // inFileDirect.close(); 
  // inFileRotate.close(); 
  // outFile.close(); 

  outFile_1.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/abCoefs_12_0_1.cal");
  outFile_2.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/abCoefs_12_2_3.cal");
  outDiff.open("/home/kmy/expertroot/er/macro/EXP1803/calibration/calDiff_12_per_B");
  outDiff << fixed << "diff_a" << setw(20) << "diff_b" << endl;
  for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {
    Double_t a1, a2;
    Double_t b1, b2;
    outFile_1 >> a1; outFile_1 >> b1;    
    outFile_2 >> a2; outFile_2 >> b2;   
    cout << "a1 = " << a1 << "; a2 = " << a2 << "; b1 = " << b1 << "; b2 = " << b2 << endl; 
    outDiff << fixed << setprecision(10) << abs(a1 - a2) / abs(a1) * 1e2 << setw(20) << abs(b1 - b2) / abs(b1) * 1e2 << endl;
  }
  outFile_1.close();
  outFile_2.close();
  outDiff.close();
}
