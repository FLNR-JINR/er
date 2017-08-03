{
	TFile *f = new TFile("out.root");
	TTree* t = (TTree*)f->Get("cbmsim");

   TCutG *cutg = new TCutG("CUTG",7);
   cutg->SetVarY("QTelescopeCalEvent.eC21[0]");
   cutg->SetVarX("QTelescopeCalEvent.eC2sum");
   /*
   cutg->SetPoint(0,15.6596,172.958);
   cutg->SetPoint(1,30.6353,75.7496);
   cutg->SetPoint(2,39.5209,43.9139);
   cutg->SetPoint(3,36.975,107.342);
   cutg->SetPoint(4,17.7063,185.352);
   cutg->SetPoint(5,15.5098,173.687);
   cutg->SetPoint(6,15.5598,174.173);
   cutg->SetPoint(7,15.6596,172.958);
   */
   /*
   cutg->SetPoint(0,21.5501,155.947);
   cutg->SetPoint(1,19.4035,138.449);
   cutg->SetPoint(2,34.8784,58.0091);
   cutg->SetPoint(3,37.8237,72.1043);
   cutg->SetPoint(4,34.7786,118.035);
   cutg->SetPoint(5,21.4003,155.947);
   cutg->SetPoint(6,21.4003,155.947);
   cutg->SetPoint(7,21.5501,155.947);
	*/
   /*
   cutg->SetPoint(0,80.9913,28.2849);
   cutg->SetPoint(1,101.117,27.3627);
   cutg->SetPoint(2,145.448,17.0336);
   cutg->SetPoint(3,147.421,14.4514);
   cutg->SetPoint(4,124.401,15.6503);
   cutg->SetPoint(5,80.4651,28.1927);
   cutg->SetPoint(6,81.2544,28.2849);
   cutg->SetPoint(7,81.2544,28.2849);
   cutg->SetPoint(8,80.9913,28.2849);
	*/
      cutg->SetPoint(0,83.2029,31.2762);
   cutg->SetPoint(1,140.542,18.4148);
   cutg->SetPoint(2,154.439,22.5025);
   cutg->SetPoint(3,91.2979,35.9621);
   cutg->SetPoint(4,83.2029,31.2762);
   cutg->SetPoint(5,83.2029,31.2762);
   cutg->SetPoint(6,83.2029,31.2762);
	t->SetMarkerStyle(22);
	t->SetMarkerSize(1);
	//t->Draw("QTelescopeCalEvent.eC21[0]:QTelescopeCalEvent.eC2sum","TelescopeRecoEvent.t22>120 && TelescopeRecoEvent.t22<140");
	//t->Draw("QTelescopeCalEvent.eC2sum:QTelescopeCalEvent.eC21[0]","CUTG");
	//t->Draw("QTelescopeCalEvent.eC21[0]:QTelescopeCalEvent.eC2sum", "TelescopeRecoEvent.exmis11 > 1 && TelescopeRecoEvent.exmis11 < 7 ");
	//t->Draw("TelescopeRecoEvent.tmis11","CUTG");
    //t->Draw("TelescopeRecoEvent.exmis11","CUTG");
}