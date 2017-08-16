 {
 TString inFilename1="out.root";

    TFile* f2 = new TFile(inFilename1);
    cout << "File: " << f2->GetName() << endl;

    TTree* t = (TTree*)f2->Get("cbmsim");
    cout << "Tree: " << t->GetName() << endl;
   t->SetMarkerStyle(7);
//   t->SetMarkerSize(1);

   TCutG *cutg = new TCutG("CUTG",8);
   cutg->SetVarY("TelescopeRecoEvent.dep21");
   cutg->SetVarX("TelescopeRecoEvent.dep2sum");

   cutg->SetPoint(0,87.9733,29.008);
   cutg->SetPoint(1,142.623,19.0977);
   cutg->SetPoint(2,154.635,16.9571);
   cutg->SetPoint(3,160.209,20.7029);
   cutg->SetPoint(4,87.6267,35.4985);
   cutg->SetPoint(5,87.8578,29.0778);
   cutg->SetPoint(6,87.8578,29.0778);
   cutg->SetPoint(7,87.9733,29.008);

  
   TString cut = "RTelescopeCalEvent.mC12==0 && BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l>400 && BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l<1200 && BeamDetCalEvent.tF4r-BeamDetCalEvent.tF3r>45 && BeamDetCalEvent.tF4r-BeamDetCalEvent.tF3r<54 && BeamDetCalEvent.tF4l > 50 && BeamDetCalEvent.tF4l < 100 && BeamDetCalEvent.tF4r > 50 && BeamDetCalEvent.tF4r < 100 && CUTG && TelescopeRecoEvent.dep11>2.5 && TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12<0.15 &&  TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12 > -0.35&& BeamDetRecoEvent.xbt*BeamDetRecoEvent.xbt+BeamDetRecoEvent.ybt*BeamDetRecoEvent.ybt<0.81 ";
   
   t->Draw("RTelescopeCalEvent.nC11[0]:RTelescopeCalEvent.nC12[0] >> RTmap(20, 1., 21.,16, 1., 17 )");
   t->Draw("BeamDetRecoEvent.ybt:BeamDetRecoEvent.xbt >> TARmap(16, -3., 3., 16, -3., 3.)");
   //,"RTelescopeCalEvent.mC12==0"
    TCanvas* canvasmis = new TCanvas("mis tests"); 
   
   canvasmis->Divide(4,3);
   canvasmis->cd(1); t->Draw("BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l>>h1",cut);
   TH1F *histo = (TH1F*)gROOT->FindObject("h1");
   histo->GetXaxis()->SetTitle("My x-axis");
   histo->GetYaxis()->SetTitle("My y-axis");
   histo->SetNameTitle("aF3r+aF3l","aF3r+aF3l");
   gPad->Update();
   //t->Draw("BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l");
   canvasmis->cd(2);   t->Draw("BeamDetCalEvent.tF4r-BeamDetCalEvent.tF3r",cut);
   canvasmis->cd(3);   t->Draw("BeamDetCalEvent.tF4l",cut);
   canvasmis->cd(4);   t->Draw("BeamDetCalEvent.tF4r",cut);
   canvasmis->cd(5);   t->Draw("TelescopeRecoEvent.dep11",cut);
   canvasmis->cd(6);   t->Draw("TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12",cut);

   canvasmis->cd(7);   int n = t->Draw("TelescopeRecoEvent.dep21:TelescopeRecoEvent.dep2sum",cut,"goff");
   TGraph *g = new TGraph(n,t->GetV1(),t->GetV2());
   g->SetTitle("dep21 vs dep2sum");
   g->GetXaxis()->SetTitle("dep21");
   g->GetYaxis()->SetTitle("dep2sum");
   g->Draw("ap");

   canvasmis->cd(8);   t->Draw("TelescopeRecoEvent.mis11n.Excitation",cut);

   canvasmis->cd(9);   gDirectory->Get("RTmap")->Draw("colz");
   TH2F* histo2 = (TH2F*)gROOT->FindObject("RTmap");
   histo2->GetXaxis()->SetTitle("nC11");
   histo2->GetYaxis()->SetTitle("nC12");
   histo2->SetNameTitle("nC11 vs nC12","nC11 vs nC12");
   gPad->Update();

   canvasmis->cd(10);  gDirectory->Get("TARmap")->Draw("colz");
  

  canvasmis->cd(11);  t->Draw("BeamDetRecoEvent.Tb",cut);
    canvasmis->cd(12);  TPaveText *pt = new TPaveText(.05,.1,.95,.8);
  pt -> SetTextFont(53);
    pt-> SetTextSize(11); 
   pt->AddText("RTelescopeCalEvent.mC12==0 && BeamDetCalEvent.aF3r+");
   pt->AddText("BeamDetCalEvent.aF3l>400");
   pt->AddText("&& BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l<1200");
   pt->AddText("&& BeamDetCalEvent.tF4r");
   pt->AddText("-BeamDetCalEvent.tF3r>45 && BeamDetCalEvent.tF4r-");
   pt->AddText("BeamDetCalEvent.tF3r<54 ");
   pt->AddText("&& BeamDetCalEvent.tF4l > 50 && BeamDetCalEvent.tF4l < 100");
   pt->AddText("&& BeamDetCalEvent.tF4r > 50 ");
   pt->AddText("&& BeamDetCalEvent.tF4r < 100 && CUTG && ");
   pt->AddText("TelescopeRecoEvent.dep11>2.5 && ");
   pt->AddText("TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12<0.15 ");
   pt->AddText("&&  TelescopeRecoEvent.dep11");
   pt->AddText("-TelescopeRecoEvent.dep12 > -0.35&& ");
   pt->AddText("BeamDetRecoEvent.xbt*BeamDetRecoEvent.xbt+");
   pt->AddText("BeamDetRecoEvent.ybt*BeamDetRecoEvent.ybt<0.81");
   pt->Draw();
   //t->Draw("EventHeader.HE8Event_nevent",cut);
   //t->Draw("TelescopeRecoEvent.mis11n.Excitation",cut);
  
}



