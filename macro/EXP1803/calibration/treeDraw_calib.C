void treeDraw_calib() {
  TString directInFileName_1="sim_digi_direct1.root";
  TString directInFileName_4="sim_digi_calib_direct4.root";
  TString rotateInFileName_1="sim_digi_rotate1.root";
  TString rotateInFileName_4="sim_digi_rotate4.root";

  std::vector<TString> files = {
                                "sim_digi_calib_direct4.root",
                                "sim_digi_calib_direct4.root"
                                "sim_digi_calib_rot4.root"
                                "sim_digi_calib_rot4.root"
                               };


  for (auto itFile : files) {
    TFile* inFile = new TFile(itFile);
    TTree* tree = (TTree*)directInFile_1->Get("er");
  }


  // TFile* directInFile_1 = new TFile(directInFileName_1);
  // // cout << "File: " << directInFile_1->GetName() << endl;

  // TTree* directTree_1 = (TTree*)directInFile_1->Get("er");
  // // cout << "Tree: " << t->GetName() << endl;

  TCanvas* canvasMeanEdepByStrip = new TCanvas("MeanEdepByStrip");
  canvasMeanEdepByStrip->cd(1);

  Int_t calibrateStripCount = 32;
  TH1F *hDirectMeanEdepByStrip_1 = new TH1F("hDirectMeanEdepByStrip_1",
                                            "DirectMeanEdepByStrip_1",
                                            calibrateStripCount, 0, calibrateStripCount);
  TString cutToChooseStrip = "";
  TString varExp = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fEdep";
  Double_t maxMean = 1;
  Int_t    sensDeadCount = 10;
  Int_t    deadLayersLikeSensCount = 2;
  // for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {  
  //   for (Int_t deadLayerSensNb = 0; deadLayerSensNb < deadLayersLikeSensCount; deadLayerSensNb++) {
  //     TString tmpVarExp;
  //     tmpVarExp.Form("+ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_%d_X.fEdep", sensDeadCount - deadLayerSensNb - 1);
  //     varExp += tmpVarExp;
  //   }
  //   cutToChooseStrip.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb==%d", stripNb);
  //   directTree_1->Draw(varExp+">>hTmpHistToGetStripMeanEdep",cutToChooseStrip);
  //   TH1F *tmpHistToGetStripMeanEdep = (TH1F*)gDirectory->Get("hTmpHistToGetStripMeanEdep");
  //   Double_t meanValue = tmpHistToGetStripMeanEdep->GetMean();
  //   Double_t maxValue = tmpHistToGetStripMeanEdep->GetMaximum();
  //   cout << "mean for " << stripNb << " strip is " << meanValue << endl;
  //   cout << "max for " << stripNb << " strip is " << maxValue << endl;
  //   hDirectMeanEdepByStrip_1->SetBinContent(stripNb, meanValue / maxMean);
  // }
  
  TFile* directInFile_4 = new TFile(directInFileName_4);
  // cout << "File: " << directInFile_4->GetName() << endl;

  TTree* directTree_4 = (TTree*)directInFile_4->Get("er");
  // cout << "Tree: " << t->GetName() << endl;

  // Int_t calibrateStripCount = 32;
  TH1F *hDirectMeanEdepByStrip_4 = new TH1F("hDirectMeanEdepByStrip_4",
                                            "DirectMeanEdepByStrip_4",
                                             calibrateStripCount, 0, calibrateStripCount);
  cutToChooseStrip = "";
  // Double_t maxMean = 0.0048;
  // Int_t    sensDeadCount = 10;
  // Int_t    deadLayersLikeSensCount = 1;
  for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {  
    varExp = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fEdep";
    for (Int_t deadLayerSensNb = 0; deadLayerSensNb < deadLayersLikeSensCount; deadLayerSensNb++) {
      TString tmpVarExp;
      tmpVarExp.Form(" + ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_%d_X.fEdep", sensDeadCount - deadLayerSensNb - 1);
      varExp += tmpVarExp;
    }
    cutToChooseStrip.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb==%d", stripNb);
    cutToChooseStrip += " && " + varExp + " > 0.006";
    directTree_4->Draw(varExp+">>hTmpHistToGetStripMeanEdep",cutToChooseStrip);
    TH1F *tmpHistToGetStripMeanEdep = (TH1F*)gDirectory->Get("hTmpHistToGetStripMeanEdep");
    Double_t meanValue = tmpHistToGetStripMeanEdep->GetMean();
    Double_t maxBin = tmpHistToGetStripMeanEdep->GetMaximumBin();
    Double_t maxBinContent = tmpHistToGetStripMeanEdep->GetBinContent(maxBin);
    cout << "mean for " << stripNb << " strip is " << meanValue << endl;
    cout << "maxBin for " << stripNb << " strip is " << maxBin << endl;
    cout << "maxBinContent for " << stripNb << " strip is " << maxBinContent << endl;
    hDirectMeanEdepByStrip_4->SetBinContent(stripNb, meanValue / maxMean);
  }

  // canvasMeanEdepByStrip->Divide(2,1);
  // canvasMeanEdepByStrip->cd(1);
  // directTree_4->Draw("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb : ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb","");
  // canvasMeanEdepByStrip->cd(2);
  hDirectMeanEdepByStrip_4->Draw(/*"same"*/);
  hDirectMeanEdepByStrip_4->SetLineColor(kBlue);
  // directTree_4->Draw("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fEdep","ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fEdep>0.007 && ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb == 15");
  // hDirectMeanEdepByStrip_1->SetLineColor(kRed);
  // hDirectMeanEdepByStrip_1->Draw("same");
}
