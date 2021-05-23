void readCuts();
void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16);

TCutG *cutCsI[16],*cut3h[16],*cutX_C[32];
TCutG *cuthe3_1[16],*cutSQ20_1[16],*cutSQ1_1[16];
TCutG *cuthe3_2[16],*cutSQ20_2[16],*cutSQ1_2[16];
TCutG *cuthe3_3[16],*cutSQ20_3[16],*cutSQ1_3[16];
TCutG *cuthe3_4[16],*cutSQ20_4[16],*cutSQ1_4[16];
TCutG *cutHe3_total1;
TCutG *cut6Li_1;

Float_t pCsI_1[16],pCsI_2[16];

Int_t nCoins;

void createcut(){
  nCoins = 0;
  readPar("CsI_anh",pCsI_1,pCsI_2);
  readCuts();
  // return;

   TCutG *cutg1 = new TCutG("cutg1",9);
   cutg1->SetVarX("a1_2+a20_2_un");
   cutg1->SetVarY("a20_2");
   cutg1->SetTitle("Graph");
   cutg1->SetFillColor(1);
   cutg1->SetPoint(0,5.39676,3.97732);
   cutg1->SetPoint(1,9.69173,2.51989);
   cutg1->SetPoint(2,16.3206,1.63135);
   cutg1->SetPoint(3,26.2233,1.18707);
   cutg1->SetPoint(4,26.1423,0.742802);
   cutg1->SetPoint(5,10.1942,0.368963);
   cutg1->SetPoint(6,2.75494,1.85348);
   cutg1->SetPoint(7,2.85219,4.02608);
   cutg1->SetPoint(8,5.39676,3.97732);

   // TCutG *cutg2 = new TCutG("cutg2",29);
   // cutg2->SetVarX("a1_2+a20_2_un");
   // cutg2->SetVarY("a20_2");
   // cutg2->SetTitle("Graph");
   // cutg2->SetFillColor(1);
   // cutg2->SetPoint(0,3.69712,3.76605);
   // cutg2->SetPoint(1,4.74453,3.06945);
   // cutg2->SetPoint(2,5.72508,2.6257);
   // cutg2->SetPoint(3,6.83934,2.28514);
   // cutg2->SetPoint(4,7.95361,2.02714);
   // cutg2->SetPoint(5,9.58043,1.69174);
   // cutg2->SetPoint(6,11.0735,1.51115);
   // cutg2->SetPoint(7,13.3244,1.29443);
   // cutg2->SetPoint(8,16.266,1.09319);
   // cutg2->SetPoint(9,20.9682,0.90227);
   // cutg2->SetPoint(10,25.2692,0.778431);
   // cutg2->SetPoint(11,25.1132,0.716512);
   // cutg2->SetPoint(12,24.779,0.716512);
   // cutg2->SetPoint(13,24.6675,0.701032);
   // cutg2->SetPoint(14,24.4001,0.695872);
   // cutg2->SetPoint(15,23.4419,0.695872);
   // cutg2->SetPoint(16,22.4836,0.716512);
   // cutg2->SetPoint(17,20.4333,0.757792);
   // cutg2->SetPoint(18,18.4054,0.819711);
   // cutg2->SetPoint(19,16.266,0.91775);
   // cutg2->SetPoint(20,14.2158,1.01063);
   // cutg2->SetPoint(21,12.1432,1.18091);
   // cutg2->SetPoint(22,9.37986,1.46471);
   // cutg2->SetPoint(23,7.79761,1.68658);
   // cutg2->SetPoint(24,6.10393,2.07358);
   // cutg2->SetPoint(25,4.87824,2.5225);
   // cutg2->SetPoint(26,3.76398,3.10041);
   // cutg2->SetPoint(27,3.02857,3.70929);
   // cutg2->SetPoint(28,3.69712,3.76605);



  TChain *ch = new TChain("tree");

  ch->Add("/media/ivan/data/exp1904/analysed/novPars/selected/temp/trigger3/h7_ct_*");
  cout << ch->GetEntries() << endl;

  TString cut,hdraw;

  TString canName;
  TCanvas *c_arr[16];
  for(Int_t i=0;i<16;i++){
    canName.Form("c%d",i+1);
    c_arr[i] = new TCanvas(canName.Data(),"title",1800,1000);
  }
  
  for(Int_t i=0;i<16;i++) {
    // c_arr->cd(i+1);
    c_arr[i]->cd();
    ch->SetMarkerStyle(1);
    ch->SetMarkerColor(kBlack);      
    cut.Form("flag2 && trigger==3 && a20_2<4 && a1_2+a20_2_un<30 && n20_2==%d",i);
    // cut += cutg1->GetName();
    hdraw.Form("a20_2:a1_2+a20_2_un>>h%d1",i);
    ch->Draw(hdraw.Data(),cut.Data(),"");
    c_arr[i]->Update();

    ch->SetMarkerStyle(20);
    ch->SetMarkerSize(0.7);   
    ch->SetMarkerColor(kGreen);      
    // cut.Form("n20_1==%d && nhe3_1",i);
    cut += " && nh3 && flagCent";
    hdraw.Form("a20_2:a1_2+a20_2_un>>h%d_triton1",i);
    ch->Draw(hdraw.Data(),cut.Data(),"same");
    c_arr[i]->Update();

    cuthe3_2[i]->SetLineColor(kRed);
    cuthe3_2[i]->Draw("same");
    c_arr[i]->Update();

    canName.Form("pdf/canvas_%d.png",i+1);
    c_arr[i]->SaveAs(canName.Data());
  }


}

void readCuts() {

  TFile *f;
  TString cutName;

  cutName.Form("/home/ivan/work/macro/h7_1904/cuts/T1/li6_cut.root");
  f = new TFile(cutName.Data());
  cut6Li_1 = (TCutG*)f->Get("CUTG");
  if (!cut6Li_1) {
    cout << "no cut " << cutName.Data() << endl;
    exit(-1);
  }    
  delete f;

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/C_T/triton/h3_%d.root",i);
    f = new TFile(cutName.Data());
    cut3h[i] = (TCutG*)f->Get("CUTG");
    if (!cut3h[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }    
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/C_T/tCsI/tCsI_full_%d.root",i);
    f = new TFile(cutName.Data());
    cutCsI[i] = (TCutG*)f->Get("CUTG");
    if (!cutCsI[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<32;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/C_T/tX_C/tX_C_%d.root",i);
    f = new TFile(cutName.Data());
    cutX_C[i] = (TCutG*)f->Get("CUTG");
    if (!cutX_C[i]) {
      cout << i  << " no cut"<< endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/T1/he3/cut_he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_1[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_1[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cuts/T2/he3/canvas/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_2[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_2[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/T3/he3/cut_he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_3[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_3[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/T4/he3/cut_he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_4[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_4[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  cout << " CUTS are read" << endl;
}


void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16){

  TString line;
  ifstream myfile;
  Int_t count=0;
  TString file = "/home/ivan/work/macro/h7_1904/parameters/" + fileName + ".cal";
  myfile.open(file.Data());
  while (! myfile.eof() ){
    line.ReadLine(myfile);
    if(count < 0){
      count++;
      continue;
    }
    if(line.IsNull()) break;
    sscanf(line.Data(),"%g %g", par1+count,par2+count);
    count++;
  }
  cout << endl << fileName.Data() << endl;
  for(Int_t i=0;i<size;i++) cout << par1[i] << " " << par2[i] << endl;

  return;
}

