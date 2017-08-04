{
	TFile *f = new TFile("out10.root");
	TTree* t = (TTree*)f->Get("cbmsim");

   TCutG *cutg = new TCutG("CUTG",8);
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
   //with center
   
   cutg->SetPoint(0,87.9733,29.008);
   cutg->SetPoint(1,142.623,19.0977);
   cutg->SetPoint(2,180.635,15.9571);
   cutg->SetPoint(3,178.209,20.7029);
   cutg->SetPoint(4,87.6267,35.4985);
   cutg->SetPoint(5,87.8578,29.0778);
   cutg->SetPoint(6,87.8578,29.0778);
   cutg->SetPoint(7,87.9733,29.008);
   
   /*
      cutg->SetPoint(0,84.829,30.7777);
   cutg->SetPoint(1,141.938,19.2124);
   cutg->SetPoint(2,157.288,18.8136);
   cutg->SetPoint(3,161.91,20.9073);
   cutg->SetPoint(4,90.2758,34.4666);
   cutg->SetPoint(5,84.829,30.678);
   cutg->SetPoint(6,84.829,30.678);
   cutg->SetPoint(7,84.829,30.7777);
   */
	t->SetMarkerStyle(22);
	t->SetMarkerSize(1);
	t->Draw("QTelescopeCalEvent.eC21[0]:QTelescopeCalEvent.eC2sum");
	//t->Draw("QTelescopeCalEvent.eC2sum:QTelescopeCalEvent.eC21[0]","CUTG");
	//t->Draw("QTelescopeCalEvent.eC21[0]:QTelescopeCalEvent.eC2sum", "TelescopeRecoEvent.exmis11 > 1 && TelescopeRecoEvent.exmis11 < 7 ");
	//t->Draw("TelescopeRecoEvent.tmis11","CUTG");
    //t->Draw("TelescopeRecoEvent.exmis11","CUTG");
    //t->Draw("TelescopeRecoEvent.exmis11","QTelescopeCalEvent.eC2sum>160&&QTelescopeCalEvent.eC2sum<180");
    //t->Draw("TelescopeRecoEvent.t22","CUTG");

}