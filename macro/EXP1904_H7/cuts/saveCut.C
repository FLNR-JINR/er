void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16);

Float_t pCsI_1[16],pCsI_2[16];

void saveCut(){
	
	 readPar("CsI_anh",pCsI_1,pCsI_2);

	for(Int_t i =0;i<16;i++) {

		TString fileName;
		fileName.Form("/home/muzalevskii/work/macro/h7_1904/cutsNovPars/C_T/6He/he6_%d.root",i);
		TFile *f = new TFile(fileName.Data());
		TCutG *g = (TCutG*)f->Get("CUTG");

		TCutG *g1 = (TCutG*)g->Clone();
		Double_t *x = g1->GetX();
		Double_t *y = g1->GetY();
		
		for (Int_t j=0;j<g1->GetN();j++) {
			// cout << y[j] << " " << pCsI_2[i] << " ";
			x[j] = x[j]*pCsI_2[i] + pCsI_1[i];
			// cout << y[i] << endl;
		}

		// g1->Draw();
		TString fileName1;
		fileName1.Form("/home/muzalevskii/work/macro/h7_1904/cutsNovPars/C_T/6He/scripts/he6_%d.C",i);
		g1->SaveAs(fileName1.Data());
		delete g1;
	}

}

void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16){

  TString line;
  ifstream myfile;
  Int_t count=-2;
  TString file = "/home/muzalevskii/work/macro/h7_1904/parameters/" + fileName + ".cal";
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
