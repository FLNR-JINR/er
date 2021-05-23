void drawCut(){
  gStyle->SetOptStat(0);
  Bool_t tritium = 0;
  Bool_t he3 = 0;
  Bool_t tCsI_s = 0;
  Bool_t target = 0;
  Bool_t centtimes = 0;
  Bool_t sq20times = 0;
  Bool_t sq20Rtimes = 0;
  Bool_t tSQ1_1 = 1;
  Bool_t Y_Lcuts = 0;
  Bool_t Y_Rcuts = 0;

  TChain *ch = new TChain("tree");
  // ch->Add("/media/user/work/data/Analysed1811/siParTests/analysed/he8_full_cut.root");
  ch->Add("/home/oem/work/data/exp1904/analysed/h7/h7_all_cut.root");
  // ch->Add("/media/user/work/data/Analysed1811/selected/he8_full_cut_emptyTarget.root");
  cout << ch->GetEntries() << endl;

  TCutG *cutCsI_s[16],*cut3h[16],*cutX_L[16],*cutY_L[16],*cutSQ20_L[16],*cuthe3[16],*cutSQ20_R[16],*cutY_R[16];
  TString cutName;
  TFile *f1;

  for(Int_t i=0;i<16;i++) {
    // cutName.Form("/media/user/work/macro/exp201810/draw/cuts/CsI_s_%d.root",i);
    cutName.Form("/home/oem/work/macro/he8_1811/tritonCuts/h3_%d.root",i);
    f1 = new TFile(cutName.Data());
    cut3h[i] = (TCutG*)f1->Get("CUTG");
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/CsItimeCuts/CsI_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutCsI_s[i] = (TCutG*)f1->Get("CUTG");
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/cutX_L/X_L_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutX_L[i] = (TCutG*)f1->Get("CUTG");
    if (!cutX_L[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/SQ20_Rcuts/sq20_R_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutSQ20_R[i] = (TCutG*)f1->Get("CUTG");
    if (!cutSQ20_R[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/cutY_L/Y_L_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutY_L[i] = (TCutG*)f1->Get("CUTG");
    if (!cutY_L[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/SQ20_Lcuts/sq20_L_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutSQ20_L[i] = (TCutG*)f1->Get("CUTG");
    if (!cutSQ20_L[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/helium3/he3_%d.root",i);
    f1 = new TFile(cutName.Data());
    cuthe3[i] = (TCutG*)f1->Get("CUTG");
    if (!cuthe3[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }
  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/cutY_R/Y_R_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutY_R[i] = (TCutG*)f1->Get("CUTG");
    if (!cutY_R[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }


  TString cut,hdraw;

  if (tritium) {
    TCanvas *c1 = new TCanvas("c1","",1800,1000);  
    c1->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      // {Int_t i=0;
      // ch->SetMarkerStyle(20);
      // ch->SetMarkerSize(0.2);

      c1->cd(i+1);
      ch->SetMarkerColor(kBlack);      
      cut.Form("nCsI==%d",i);
      // hdraw.Form("X_C.:aCsI>>h%d(300,0,4000,800,0,60)",i);
      hdraw.Form("X_C.:aCsI>>h%d(500,0,4048,500,1,30)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"col");
      c1->Update();

      ch->SetMarkerStyle(1);
      ch->SetMarkerColor(kGreen);      
      cut.Form("nCsI==%d && nh3",i);
      // hdraw.Form("X_C.:aCsI>>h%d(300,0,4000,800,0,60)",i);
      hdraw.Form("X_C.:aCsI>>h_red%d(500,0,4048,500,1,30)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");
      c1->Update();
     
      ch->SetMarkerStyle(20);
      ch->SetMarkerColor(kRed); 
      ch->SetMarkerSize(0.4);          
      cut.Form("nCsI==%d && nh3 && nhe3_1",i);
      // hdraw.Form("X_C.:aCsI>>h%d(300,0,4000,800,0,60)",i);
      hdraw.Form("X_C.:aCsI>>h_coin%d(500,0,4048,500,1,30)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");
      c1->Update();

    }
    c1->Print("/home/oem/Desktop/pics/exp1904/de_E_cent.pdf");

      // cut.Form("nh3==1 && trigger==2 && nCsI==%d && flagCent==1",i);
      // // tree->Draw("DSDX_C:aCsI_s","nCsI_s==0","", 1004737, 0);

      // hdraw.Form("X_C.:aCsI>>h_h3_%d(300,0,5000,300,0,20)",i);

      // ch->SetMarkerColor(kRed);
      // ch->SetMarkerStyle(20);
      // ch->SetMarkerSize(0.7);
      // cut.Form("nhe3==1 && nh3==1 && trigger==2 && nCsI==%d && flagCent==1",i);
      // hdraw.Form("X_C.:aCsI>>h%d_1(300,0,5000,300,0,20)",i);
      // ch->Draw(hdraw.Data(),cut.Data(),"same");

      // cut3h[i]->SetLineColor(kRed);
      // cut3h[i]->Draw("same");
      
    // // c1->Print("/home/user/Desktop/redmine/triton.png");
    // TCanvas *c2 = new TCanvas("c2","",1800,1000);  
    // c2->cd();
    // ch->Draw("X_C.:aCsI_s>>h(300,0,5000,300,0,100)","nCsI_s==2","col");
    // cut3h[2]->Draw("same");
    // c2->Update();
  }


  if (he3) {
    TCanvas *c1_h3 = new TCanvas("c1_h3","",1800,1000);  
    c1_h3->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
    // Int_t i=0;
      c1_h3->cd(i+1);
      cut.Form("n20_L==%d && flagLeft",i);
      // tree->Draw("DSDX_C:aCsI_s","nCsI_s==0","", 1004737, 0);
      hdraw.Form("a20_L.:X_L>>h_%d(200,0,70,200,0,5)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"col");

      cut.Form("n20_L==%d && flagLeft && nhe3_1",i);
      // tree->Draw("DSDX_C:aCsI_s","nCsI_s==0","", 1004737, 0);
      ch->SetMarkerStyle(20);
      hdraw.Form("a20_L.:X_L>>h_he3%d(200,0,70,200,0,5)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");


      cuthe3[i]->SetLineColor(kRed);
      cuthe3[i]->Draw("same");
      c1_h3->Update();
    } 

    // TCanvas *c1_h3_single = new TCanvas("c1_h3_single","",1800,1000);  

    // // for(Int_t i=0;i<16;i++) {
    // {Int_t i=13;

    //   ch->SetMarkerStyle(20);
    //   ch->SetMarkerSize(1);
    //   ch->SetMarkerColor(kBlack);

    //   cut.Form("trigger==2 && n20_L==%d && flagLeft==1",i);
    //   // tree->Draw("DSDX_C:aCsI_s","nCsI_s==0","", 1004737, 0);
    //   hdraw.Form("a20_L.:X_L>>h_he3_single_%d(200,0,70,200,0,5)",i);
    //   ch->Draw(hdraw.Data(),cut.Data(),"");

    //   ch->SetMarkerColor(kRed);
    //   cut.Form("trigger==2 && n20_L==%d && flagLeft==1 && nhe3",i);
    //   // tree->Draw("DSDX_C:aCsI_s","nCsI_s==0","", 1004737, 0);
    //   hdraw.Form("a20_L.:X_L>>h_he3_singlehe3_%d(200,0,70,200,0,5)",i);
    //   ch->Draw(hdraw.Data(),cut.Data(),"same");

    //   ch->SetMarkerColor(kBlue);
    //   cut.Form("trigger==2 && n20_L==%d && flagLeft==1 && nhe4",i);
    //   // tree->Draw("DSDX_C:aCsI_s","nCsI_s==0","", 1004737, 0);
    //   hdraw.Form("a20_L.:X_L>>h_he3_singlehe4_%d(200,0,70,200,0,5)",i);
    //   ch->Draw(hdraw.Data(),cut.Data(),"same");

    //   ch->SetMarkerColor(kMagenta);
    //   cut.Form("trigger==2 && n20_L==%d && flagLeft==1 && nhe8",i);
    //   // tree->Draw("DSDX_C:aCsI_s","nCsI_s==0","", 1004737, 0);
    //   hdraw.Form("a20_L.:X_L>>h_he3_singlehe8_%d(200,0,70,200,0,5)",i);
    //   ch->Draw(hdraw.Data(),cut.Data(),"same");

    //   // cuthe3[i]->SetLineColor(kRed);
    //   // cuthe3[i]->Draw("same");
    //   c1_h3_single->Update();
    // } 


  }

  if (tCsI_s) {
    TCanvas *c3 = new TCanvas("c3","",1800,1000);  
    c3->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      c3->cd(i+1);
      // {Int_t i=6;
      ch->SetMarkerStyle(1);
      ch->SetMarkerColor(kBlack);      
      cut.Form("nCsI==%d",i);
      hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_1_%d",i);
      ch->Draw(hdraw.Data(),cut.Data(),"");

      ch->SetMarkerColor(kRed);
      cut.Form("nCsI==%d && flagCent",i);
      hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_h3_%d",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");

      // ch->SetMarkerColor(kGreen);
      // cut.Form("nCsI==%d && trigger!=1 && nheIsotope && flagCent",i);
      // hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_alpha_%d",i);
      // ch->Draw(hdraw.Data(),cut.Data(),"same");

      // ch->SetMarkerColor(kBlack);
      // cutCsI_s[i]->Draw("same");
      // cutCsI_s[i]->SetLineColor(kBlack);
      // cutCsI_s[i]->SetLineWidth(2);
      // ch->SetMarkerColor(kRed);
      // ch->SetMarkerStyle(20);
      // ch->SetMarkerSize(0.1); 


      c3->Update();
    }

    // TCanvas *c3_1 = new TCanvas("c3_1","",1800,1000);  
    // c3_1->Divide(4,4);
    // for(Int_t i=0;i<16;i++) {
    //   c3_1->cd(i+1);
    //   cut.Form("nCsI==%d && trigger==1",i);
    //   // tree->Draw("DSDX_C:aCsI_s","nCsI_s==0","", 1004737, 0);
    //   hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_1_%d_trigg1(700,-400,300,3000,0,3000)",i);
    //   ch->Draw(hdraw.Data(),cut.Data(),"col",1000000,0);
    //   cutCsI_s[i]->Draw("same");
    //   cutCsI_s[i]->SetLineColor(kBlack);
    //   cutCsI_s[i]->SetLineWidth(2);
    //   ch->SetMarkerColor(kRed);
    //   ch->SetMarkerStyle(20);
    //   ch->SetMarkerSize(0.1); 

    //   // cut.Form("nCsI==%d && flagCent",i);
    //   // hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_2_%d(200,-400,300,200,0,3000)");
    //   // ch->Draw(hdraw.Data(),cut.Data(),"same",,1000000,0);

    //   c3_1->Update();
    // }

    // c3->Print("/home/user/Desktop/redmine/CsI_cuts.png");
  }


  if (target) {
    TCanvas *c4 = new TCanvas("c4","target cut",1000,1000);  
    cout << ch->Draw("fYt:fXt>>h_target(300,-30,30,300,-30,30)","","col") << endl;
  } 

  if (centtimes) {

    TCanvas *c5 = new TCanvas("c5","",1800,1000);
    c5->Divide(4,4);

    for(Int_t i=0;i<16;i++) {
      // Int_t i = 15;
      c5->cd(i+1);

      cutName.Form("nX_C==%d && flagCent && tX_C - tF5<-130 && X_C<30",i);
      ch->SetMarkerColor(kBlack);  
      ch->Draw("X_C:tX_C - tF5",cutName.Data(),"");

      ch->SetMarkerColor(kRed);
      cutName = cutName + TString(" && nh3");
      ch->Draw("X_C:tX_C - tF5",cutName.Data(),"same");

      c5->Update();
    }

//     TCanvas *c5_1 = new TCanvas("c5_1","",1800,1000);
//     c5_1->Divide(4,4);

//     for(Int_t i=0;i<16;i++) {
//       c5_1->cd(i+1);
// // hdraw cutName
//       cutName.Form("trigger && nX_C==%d && tX_C - tF5>80 && tX_C - tF5<135",i);
//       ch->SetMarkerColor(kBlack);  
//       ch->Draw("X_C:tX_C - tF5",cutName.Data(),"");
//       ch->SetMarkerColor(kRed);
//       cutName = cutName + TString(" && flagCent==1");
//       ch->Draw("X_C:tX_C - tF5",cutName.Data(),"same");

//       c5_1->Update();
//     }


    // c5->cd(1);
    // ch->SetMarkerColor(kBlack);  
    // ch->Draw("X_C:tX_C - tF5","","");
    // ch->SetMarkerColor(kRed);
    // ch->Draw("X_C:tX_C - tF5","flagCent_arr==1","same");
    // ch->SetMarkerColor(kBlack);

    // c5->cd(2);
    // ch->SetMarkerColor(kBlack);  
    // ch->Draw("Y_C:tY_C - tF5","","", 1004737, 0);
    // ch->SetMarkerColor(kRed);
    // ch->Draw("Y_C:tY_C - tF5","flagCent_arr==1","same");
    // ch->SetMarkerColor(kBlack);
  }

  if (sq20times) {

    TCanvas *c6 = new TCanvas("c6","sq20_L times",1800,1000);
    c6->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
    // { 
      // Int_t i = 15;
      c6->cd(i+1);
      ch->SetMarkerStyle(1);      
      ch->SetMarkerColor(kBlack);
      cut.Form("n20_1==%d && flag1",i);
      hdraw.Form("a20_1:t20_1-tF5 >> h_tsq20_%d(300,0,120,300,0,6)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"");

      ch->SetMarkerStyle(20);
      ch->SetMarkerColor(kRed);
      cut.Form("n20_1==%d && nhe3_1",i);
      hdraw.Form("a20_1:t20_1-tF5 >> h_tsq20_red_%d(300,0,120,300,0,20)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");

      c6->Update();
    }
    // c6->Print("/home/user/Desktop/pictures/t20_L_all.png");

  }

if (sq20Rtimes) {

    TCanvas *c6_R = new TCanvas("c6_R","sq20_L times",1800,1000);
    // c6_R->Divide(4,4);
    // ch->SetMarkerStyle(20);
    // for(Int_t i=0;i<16;i++) {
    //   cout << "EVENT" << endl;
    //   c6_R->cd(i+1);
    { Int_t i =7;
      ch->SetMarkerColor(kBlack);
      cut.Form("n20_R==%d",i);
      // hdraw.Form("a20_R_uncorr:t20_R-tF5 >> h_tsq20_%d(300,-60,60,300,0,20)",i);
      hdraw.Form("a20_R_uncorr:t20_R-tF5 >> h_tsq20_%d",i);
      cout << ch->Draw(hdraw.Data(),cut.Data(),"") << endl;

      ch->SetMarkerColor(kRed);
      cut.Form("n20_R==%d && flagRight",i);
      // hdraw.Form("a20_R_uncorr:t20_R-tF5 >> h_tsq20_red_%d(300,-60,60,300,0,20)",i);
      hdraw.Form("a20_R_uncorr:t20_R-tF5 >> h_tsq20_red_%d",i);
      cout << ch->Draw(hdraw.Data(),cut.Data(),"same") << endl;

      // cutSQ20_R[i]->SetLineColor(kBlack);
      // cutSQ20_R[i]->Draw("same");

      // c6_R->Update();
    }
    // c6->Print("/home/user/Desktop/pictures/t20_L_all.png");

  }

  if (tSQ1_1) {
    TCanvas *c7 = new TCanvas("c7","",1800,1000);  
    // c7->Divide(4,4);
    // for(Int_t i=0;i<16;i++) {
    //   c7->cd(i+1);
    { Int_t i=15;
      // ch->SetMarkerStyle(20);
      // ch->SetMarkerSize(1.);

      cut.Form("n1_1==%d && flag1",i);
      hdraw.Form("a1_1:t1_1-tF5 >> h_X_L_%d(2000,0,100,2000,-10,60)",i);
      ch->SetMarkerColor(kBlack);
      ch->Draw(hdraw.Data(),cut.Data(),"col");

      ch->SetMarkerStyle(20);
      ch->SetMarkerSize(0.5);
      cut.Form("n1_1==%d && flag1 && nhe3_1",i);
      hdraw.Form("a1_1:t1_1-tF5 >> h_X_Lhe3_%d(2000,0,100,2000,-10,60)",i);
      ch->SetMarkerColor(kRed);
      ch->Draw(hdraw.Data(),cut.Data(),"same");

      c7->Update();
    }
  }

  if (Y_Lcuts) {
    TCanvas *c8 = new TCanvas("c8","",1800,1000);  
    c8->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      c8->cd(i+1);
      ch->SetMarkerColor(kBlack);
      cut.Form(" nY_L==%d && tY_L-tF5<200",i);
      hdraw.Form("Y_L:tY_L-tF5>>h_Y_L_%d(300,-70,100,500,0,100)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"col");

      ch->SetMarkerColor(kRed);
      cut.Form("nY_L==%d && tY_L-tF5<200 && flagLeft",i);
      hdraw.Form("Y_L:tY_L-tF5>>h_Y_L_red_%d(300,-70,100,500,0,100)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");

      cutY_L[i]->SetLineColor(kBlack);
      cutY_L[i]->Draw("same");
      c8->Update();
    }

  }

if (Y_Rcuts) {
    // TCanvas *c10 = new TCanvas("c10","",1800,1000);  
    // c10->Divide(4,4);
  TString canName;
  TCanvas *c_arr[16];
  for(Int_t i=0;i<16;i++){
    canName.Form("c%d",i+1);
    c_arr[i] = new TCanvas(canName.Data(),"title",1800,1000);
  }

    for(Int_t i=0;i<16;i++) {
      c_arr[i]->cd();

      ch->SetMarkerColor(kBlack);
      ch->SetMarkerStyle(1);
      cut.Form("nY_R==%d && flagRight",i);
      hdraw.Form("Y_R:tY_R-tF5>>h_Y_R_%d(300,-70,100,500,0,50)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"");

      ch->SetMarkerColor(kRed);
      ch->SetMarkerStyle(20);
      ch->SetMarkerSize(1);
      cut.Form("nY_R==%d && flagRight && nhe3_R",i);
      hdraw.Form("Y_R:tY_R-tF5>>h_Y_R_red_%d(300,-70,100,500,0,50)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");

      ch->SetMarkerColor(kMagenta);
      ch->SetMarkerStyle(20);
      ch->SetMarkerSize(2);
      cut.Form("nY_R==%d && flagRight && nhe3_R && nh3",i);
      hdraw.Form("Y_R:tY_R-tF5>>h_Y_R_mag_%d(300,-70,100,500,0,50)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");


      c_arr[i]->Update();
      // cutY_R[i]->SetLineColor(kBlack);
      // cutY_R[i]->Draw("same");
      // c10->Update();
    }

  c_arr[0]->Print("/home/oem/Desktop/pics/Y_Rtimes_all.pdf");
  c_arr[0]->Print("/home/oem/Desktop/pics/Y_Rtimes_all.pdf[");
  c_arr[0]->Print("/home/oem/Desktop/pics/Y_Rtimes_all.pdf");
  for(Int_t i=1;i<15;i++) {
    c_arr[i]->Print("/home/oem/Desktop/pics/Y_Rtimes_all.pdf");
  }
  c_arr[15]->Print("/home/oem/Desktop/pics/Y_Rtimes_all.pdf");
  c_arr[15]->Print("/home/oem/Desktop/pics/Y_Rtimes_all.pdf]");


  }


}