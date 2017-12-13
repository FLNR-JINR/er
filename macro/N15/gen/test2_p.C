// Global declarations
TGraph* gr, *igr;

// C++ defined function
Double_t f(Double_t *x, Double_t *par)
{
	return igr->Eval(x[0]);
}

ofstream out("CMom_B11_N15.txt");

void test2_p()
{
	// Read the text file into the tree
	// TFile file("myfile.root","RECREATE");
	TTree* MyTree = new TTree("MyTree", "MyTree");
	Double_t tetta, crossSection;
	Double_t CMmom,phi, px,py,pz,pz_B,px_B,py_B;
	MyTree->ReadFile("cos_tetta_cross.txt", "tetta/D:crossSection/D");
 TBranch *bpt = MyTree->Branch("px",&px,"px/F");
	
	MyTree->SetBranchAddress("tetta", &tetta);
	MyTree->SetBranchAddress("crossSection", &crossSection);
	/*
	MyTree->SetBranchAddress("pz", &pz);
	MyTree->SetBranchAddress("px", &px);
	MyTree->SetBranchAddress("py", &py);
	MyTree->SetBranchAddress("pz_B", &pz_B);
	MyTree->SetBranchAddress("px_B", &px_B);
	MyTree->SetBranchAddress("py_B", &py_B);
	*/
	// Create two vectors to be filled from the tree
	const Int_t nEntries = MyTree->GetEntries(); // number of entries in the input file
	TVectorD tet(nEntries);
	TVectorD sigma(nEntries);

	// Fill two vectors from the tree
	for (int i=0; i<nEntries; i++) {
		MyTree->GetEntry(i);
		tet(i) = tetta;
		sigma(i) = crossSection;
	}

	// Create the graph
	gr = new TGraph(tet, sigma);
	igr = new TGraph(sigma,tet);
	// Draw the graph
	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
	c1->cd();
	igr->Draw();
	igr->SetLineColor(kRed);
	igr->SetLineWidth(3);
	gPad->SetGrid(1, 1);

	// ROOT-TF1 object with the reference to the C++ defined function
	TF1* func = new TF1("func", f, 0, 1, 0);

	// Draw the func
	TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
	c2->cd();
	func->Draw();
	func->SetLineColor(kGreen);
	func->SetLineWidth(3);
	gPad->SetGrid(1, 1);

	// Uniform random number generator
	TRandom3* rndGen = new TRandom3();

	TH1D* histo1 = new TH1D("histo1", "histo1", 1800, 0., 1.);
	TH1D* histo2 = new TH1D("histo2", "histo2", 1800, 0., 180.);
	TH1D* histo3 = new TH1D("histo3", "histo3", 1800, 0., 180.);
	TH1D* histo4 = new TH1D("histo4", "histo4", 1800, 0., 180.);
	TH1D* histo5 = new TH1D("histo5", "histo5", 1800, 0., 180.);


	//write header
	out << "Energy\t15N Px\t15N Py\t15N Pz\t11B Px\t11B Py\t11B Pz\tAngle" << endl;

	UInt_t nIterations = 5000;
	// Loop over all iterations

	for (UInt_t iIter=0; iIter<nIterations; iIter++)
	{
		if (iIter%100000 == 0) cout << "Iteration " << iIter << endl;

		Double_t x = rndGen->Rndm();	// from 0.0 to 1.0
		Double_t y = func->Eval(x);
   //___________fill px py pz -----------------
	
Double_t 	invariant;
		Double_t AMU=931.5 ;
		Double_t m11B=AMU*11.;
		Double_t m15N=AMU*15;
		Double_t T_N15=43;
Double_t CMmom,phi, px,py,pz;
		invariant = (m11B+m15N)*(m11B+m15N)+2*m11B*T_N15;

Double_t shorty=(invariant-m11B*m11B-m15N*m15N)*(invariant-m11B*m11B-m15N*m15N);
CMmom = sqrt((shorty-4*m15N*m15N*m11B*m11B)/(4*invariant));
		//phi= CLHEP::RandFlat::shoot(0.0,2*double(CLHEP::pi));
		Double_t PI = 3.14159265358;
		phi = rand()% 2*PI;
		pz=CMmom*cos(y);
		px=CMmom*sin(y)*cos(phi*180/PI) ;//cos(phi);
		py=CMmom*sin(y)*sin(phi*180/PI); //sin(phi);
		pz_B=-pz;
		px_B=-px;
		py_B=-py;
		
		out<< 0 <<"\t" <<  pz*10e-3<<"\t"<<px*10e-3<<"\t"<< py*10e-3<<"\t"<<-pz*10e-3<<"\t"<<-px*10e-3<<"\t"<< -py*10e-3<< "\t"<< 0 << endl;
		
		// Histograms filling
		histo1->Fill(x);
		histo2->Fill(y);
		histo3->Fill(pz);
		histo4->Fill(px);
		histo5->Fill(py);
		//file.Write();
		
	}

	// Draw
	TCanvas* canv1 = new TCanvas("canv1", "canv1");
	canv1->cd();
	histo1->Draw();
	gStyle->SetOptStat(111111);
	gPad->SetGrid(1, 1);

	// Draw
	TCanvas* canv2 = new TCanvas("canv2", "canv2");
	canv2->cd();
	histo2->Draw();
	gStyle->SetOptStat(111111);
	gPad->SetGrid(1, 1);
	
		TCanvas* canv3 = new TCanvas("canv3", "canv3");
	canv3->cd();
	histo3->Draw();
	gStyle->SetOptStat(111111);
	gPad->SetGrid(1, 1);
	
		TCanvas* canv4 = new TCanvas("canv4", "canv4");
	canv3->cd();
	histo4->Draw();
	gStyle->SetOptStat(111111);
	gPad->SetGrid(1, 1);
	
		TCanvas* canv5 = new TCanvas("canv5", "canv5");
	canv3->cd();
	histo5->Draw();
	gStyle->SetOptStat(111111);
	gPad->SetGrid(1, 1);
	
}