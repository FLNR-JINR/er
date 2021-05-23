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

   TCutG *cutg1 = new TCutG("cutg1",11);
   cutg1->SetVarX("a1_3+a20_3_un");
   cutg1->SetVarY("a20_3");
   cutg1->SetTitle("Graph");
   cutg1->SetFillColor(1);
   cutg1->SetPoint(0,4.94652,3.76231);
   cutg1->SetPoint(1,7.9503,2.66865);
   cutg1->SetPoint(2,12.1426,1.89543);
   cutg1->SetPoint(3,28.6094,1.03166);
   cutg1->SetPoint(4,30.9649,0.899303);
   cutg1->SetPoint(5,30.403,0.530108);
   cutg1->SetPoint(6,13.4176,0.35596);
   cutg1->SetPoint(7,7.77742,1.05255);
   cutg1->SetPoint(8,2.11561,1.99296);
   cutg1->SetPoint(9,2.59103,3.92252);
   cutg1->SetPoint(10,4.94652,3.76231);


  TChain *ch = new TChain("tree");

  ch->Add("/media/ivan/data/exp1904/analysed/novPars/selected/temp/trigger4/h7_ct_*");
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
    cut.Form("flag3 && trigger==4 && a20_3<5 && a1_3+a20_3_un<30 && n20_3==%d",i);
    hdraw.Form("a20_3:a1_3+a20_3_un>>h%d1",i);
    ch->Draw(hdraw.Data(),cut.Data(),"");
    c_arr[i]->Update();

    ch->SetMarkerStyle(20);
    ch->SetMarkerSize(0.7);   
    ch->SetMarkerColor(kGreen);      
    // cut.Form("n20_1==%d && nhe3_1",i);
    cut += " && nh3 && flagCent";
    hdraw.Form("a20_3:a1_3+a20_3_un>>h%d_triton1",i);
    ch->Draw(hdraw.Data(),cut.Data(),"same");
    c_arr[i]->Update();

    cuthe3_3[i]->SetLineColor(kRed);
    cuthe3_3[i]->Draw("same");
    c_arr[i]->Update();

    canName.Form("pdf/id_%d.png",i+1);
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
    cutName.Form("/home/ivan/work/macro/h7_1904/cutsNovPars/T2/he3/cut_he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_2[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_2[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/h7_1904/cuts/T3/he3/canvas/he3_%d.root",i);
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

