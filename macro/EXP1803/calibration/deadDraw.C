void deadDraw() {
  TString directInFileName_4="sim_digi_calib_direct4.root";


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
  Int_t    deadLayersLikeSensCount = 0;

  
  TFile* directInFile_4 = new TFile(directInFileName_4);
  // cout << "File: " << directInFile_4->GetName() << endl;

  TTree* directTree_4 = (TTree*)directInFile_4->Get("er");
  // cout << "Tree: " << t->GetName() << endl;

 for (Int_t stripNb = 0; stripNb < calibrateStripCount; stripNb++) {  
    TString varExp12 = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fEdep";
    TString varExp32 = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fEdep";
    TString cutExp12; 
    TString cutExp32; 
    TString cutExp;
    // cutExp.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_0_X.fStripNb==%d"); 
    cutExp12.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fStripNb==%d", stripNb);
    cutExp32.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fStripNb==%d", stripNb);
    for (Int_t deadLayerSensNb = 1; deadLayerSensNb < 3; deadLayerSensNb++) {
      TString tmpVarExp;
      TString tmpCut;
      tmpCut.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_%d_X.fStripNb==%d", deadLayerSensNb, stripNb);
      tmpVarExp.Form(" + ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_%d_X.fEdep", deadLayerSensNb);
      varExp12 += tmpVarExp;
      cutExp12 += " && " + tmpCut;
    }
    for (Int_t deadLayerSensNb = 1; deadLayerSensNb < 8; deadLayerSensNb++) {
      TString tmpVarExp;
      TString tmpCut;
      tmpCut.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_%d_X.fStripNb==%d", deadLayerSensNb, stripNb);
      tmpVarExp.Form(" + ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_%d_X.fEdep", deadLayerSensNb);
      varExp32 += tmpVarExp;
      cutExp32 += " && " + tmpCut;
    }
    TString histName12;
    TString histName32;
    histName12.Form("h_1.2_StripNb%d", stripNb);
    histName32.Form("h_3.2_StripNb%d", stripNb);
    directTree_4->Draw(varExp12 + ">>" + histName12, cutExp12.Data());
    directTree_4->Draw(varExp32 + ">>" + histName32, cutExp32.Data());
    TH1F *tmpHistToGetStripMeanEdep = (TH1F*)gDirectory->Get("hTmpHistToGetStripMeanEdep");
  }

  // canvasMeanEdepByStrip->Divide(2,1);
  // canvasMeanEdepByStrip->cd(1);
  // directTree_4->Draw("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb : ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb","");
  // canvasMeanEdepByStrip->cd(2);
  // hDirectMeanEdepByStrip_4->Draw(/*"same"*/);
  // hDirectMeanEdepByStrip_4->SetLineColor(kBlue);
  // directTree_4->Draw("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fEdep","ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fEdep>0.007 && ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb==15");
  // hDirectMeanEdepByStrip_1->SetLineColor(kRed);
  // hDirectMeanEdepByStrip_1->Draw("same");


}
