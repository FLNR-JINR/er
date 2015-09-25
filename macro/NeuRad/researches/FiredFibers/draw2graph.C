void draw2graph(){
	
	int save=1;
	int draw=1;

	
	TString f1name = "AlightFibers_1m.root";

	TFile *f1 = new TFile(f1name);
	
	TString histName1 = "500MeV. Point light. 1Fiber.";
    TString histName2 = "500MeV. Point edeps. 1Fiber.";
	TString newHistName = "Sum Energy deposit and Light yield in 1-fiber events. E_{kin}^{n}=500 MeV,Geant4,BERT,rCut = 0.7 mm, Cut=100KeV";
	
	TH1F * h1 = (TH1F*)f1->Get(histName1);
	TH1F * h2 = (TH1F*)f1->Get(histName2);
	h1->SetTitle(newHistName);
     
     cout<<"  Draw two 1D hist in one canvas"<<endl<<endl;
   	 TImage *img[1];
     TCanvas *Canv[1];
     TString canvnamebuf = "canv";
     TString canvtitlebuf = "Canv";
     TString filepng = "";
     TH1D* h_buf;
     int nhist=0;
     
    Canv[nhist] = new TCanvas(canvnamebuf + Form("%d",nhist), canvtitlebuf + Form("%d",nhist), 0, 0, 1900, 1000);
	Canv[nhist]->SetFillColor(10);
	Canv[nhist]->SetBorderMode(0);
	Canv[nhist]->SetBorderSize(2);
	Canv[nhist]->SetLogy();
	Canv[nhist]->SetGridx();
	Canv[nhist]->SetGridy();
	 Canv[nhist]->SetLeftMargin(0.06102476);
   	 Canv[nhist]->SetRightMargin(0.01842257);
	Canv[nhist]->SetFrameFillColor(0);
	Canv[nhist]->SetFrameBorderMode(0);
	Canv[nhist]->cd();
    
    h_buf = (TH1D*)h1->Clone(h1->GetName());
	TString buftitle = h1->GetTitle();
	h_buf->SetTitle(buftitle);
    
    		 h_buf->GetXaxis()->SetTitle("MeV");
		     h_buf->GetXaxis()->CenterTitle(true);
		     h_buf->GetXaxis()->SetNdivisions(520);
		     //h_buf->GetXaxis()->SetTitleOffset(1.15);
		     //h_buf->GetXaxis()->SetLabelSize(0.03);
		     //if(draw3d==1) h_buf->GetXaxis()->SetTitleOffset(1.6);
		     h_buf->GetYaxis()->SetTitle("Counts/0.1MeV");
		     h_buf->GetYaxis()->CenterTitle(true);
		     //h_buf->GetYaxis()->SetNdivisions(509);
		     h_buf->GetYaxis()->SetTitleOffset(0.8);
		     //h_buf->GetYaxis()->SetLabelSize(0.03);
		     h_buf->SetStats(kFALSE);
		     h_buf->SetLineWidth(2);
		     h_buf->Draw("");
    
    		 h2->SetLineColor(2);
    		 h2->SetLineWidth(2);
    		 h1->SetLineWidth(2);
		     h2->Draw("same");
    
   //h1->GetYaxis()->SetRangeUser(1,10000);
   //h2->GetYaxis()->SetRangeUser(1,10000);
   
   //h2->SetLineColor(6);
   
   //h1->Draw();
   //h2->Draw("sames");
   
   Canv[nhist]->Update(); //this will force the generation of the "stats" box
   //TPaveStats *ps1 = (TPaveStats*)h1->GetListOfFunctions()->FindObject("stats");
   //TPaveStats *ps2 = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
   
   //ps1->SetX1NDC(0.4); ps1->SetX2NDC(0.55);
   //ps2->SetX1NDC(0.6); ps2->SetX2NDC(0.75);
   //ps2->SetTextColor(6);
   //ps3->SetX1NDC(0.8); ps3->SetX2NDC(0.95);
   //ps3->SetTextColor(8);
   
   int entr[2] = {0, 0};
   int j=0;
   entr[j]=h1->GetEntries();
   entr[j+1]=h2->GetEntries();
   TString entries[2] = {" ", " "};
   j=0;
   entries[j] = entries[j] + ": " + Form("%d", entr[j]);
   entries[j+1] = entries[j+1] + ": " + Form("%d", entr[j+1]);
   j=0;
   TLegend *l =new TLegend(0.6,0.7,0.99,0.9);
   			 l->SetTextFont(62);
			 l->SetTextSize(0.03868472);
			 l->SetMargin(0.4);
			 l->SetFillColor(0);
   l->AddEntry(h1, "Light yield"+entries[j], "l");
   l->AddEntry(h2, "Energy deposit"+entries[j+1], "l");
   l->Draw();
   
   Canv[nhist]->Modified();
   
             if(save==1) {
		     	 filepng = dir + "test1" + ".png";
			     cout<<"  Save file: "<<filepng<<endl;
			     img[nhist] = TImage::Create(); 
			     img[nhist]->FromPad(Canv[nhist]); 
			     img[nhist]->WriteImage(filepng);
			 }
}