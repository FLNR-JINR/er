{
	TString inFilelab = "sim_digi.root";
	TString inFilereco = "reco/sim_digi.target.root";

	TFile *f_sim,*f_reco;
	f_sim = new TFile(inFilelab,"READ");
	TTree *t_sim = (TTree*)f_sim -> Get("er");
	f_reco = new TFile(inFilereco,"READ");
	TTree *t_reco = (TTree*)f_reco -> Get("er");
	
	t_sim->AddFriend(t_reco);
	
	Double_t w = 1500;
	Double_t h = 1000;	
	TString name;
	
	
	TCut cut_mwpc1X = "BeamDetMWPCDigiX1@.GetEntries()==1";
	TCut cut_mwpc1Y = "BeamDetMWPCDigiY1@.GetEntries()==1";
	TCut cut_mwpc2X = "BeamDetMWPCDigiX2@.GetEntries()==1";
	TCut cut_mwpc2Y = "BeamDetMWPCDigiY2@.GetEntries()==1";
	TCut cut_tof1 = "BeamDetToFDigi1@.GetEntries()==1";
	TCut cut_tof2 = "BeamDetToFDigi2@.GetEntries()==1";
	TCut cut_ndtrack = "BeamDetTrack@.GetEntries()==1";
	TCut cut_ndparticle = "BeamDetParticle@.GetEntries()==1";	
	TCut cut_E8 = "MCEventHeader.fHe8b.fE>0";
	TCut cut = cut_mwpc1X && cut_mwpc1Y && cut_mwpc2X && cut_mwpc2Y && cut_tof1 && cut_tof2 && cut_E8 ;
	
	
	name.Form("BeamDetTrack position on target");	
	TCanvas c1("c1", name.Data(), w, h);
	c1.Divide(3,2);
	c1.cd(1);
	t_sim->Draw("MCEventHeader.fReactionPos.fX>>hXtgt_sim(50,-1.4,1.1.5)");
	hXtgt_sim->GetXaxis()->SetTitle("X(target)_sim, cm");
	c1.cd(2);
	t_sim->Draw("BeamDetTrack.fXt>>hXtgt_reco(50,-1.4,1.1.5)");
	hXtgt_reco->GetXaxis()->SetTitle("X(target)_reco, cm");
	c1.cd(3);
	t_sim->Draw("(MCEventHeader.fReactionPos.fX-BeamDetTrack.fXt)*10>>hdXtgt(100,-6.,6.)");	//in mm
	TF1 *g1 = new TF1("g1", "gaus", -2,2);
	hdXtgt->Fit(g1);
	gStyle->SetOptFit(1);
	hdXtgt->GetXaxis()->SetTitle("X(target)_sim - X(target)_reco, cm");
	c1.cd(4);
	t_sim->Draw("MCEventHeader.fReactionPos.fY>>hYtgt_sim(50,-1.4,1.1.5)");
	hYtgt_sim->GetXaxis()->SetTitle("Y(target)_sim, cm");
	c1.cd(5);
	t_sim->Draw("BeamDetTrack.fYt>>hYtgt_reco(50,-1.4,1.1.5)");
	hYtgt_reco->GetXaxis()->SetTitle("Y(target)_reco, cm");
	c1.cd(6);
	t_sim->Draw("(MCEventHeader.fReactionPos.fY-BeamDetTrack.fYt)*10>>hdYtgt(100,-6.,6.)");
	hdYtgt->Fit(g1);
	hdYtgt->GetXaxis()->SetTitle("Y(target)_sim - Y(target)_reco, cm");
	

	name.Form("BeamDetTrack unit vector on target");	
	TCanvas c2("c2", name.Data(), w, h);
	c2.Divide(3,2);
	c2.cd(1);
	t_sim->Draw("MCEventHeader.fHe8b.Vect().Unit().X()>>hUxtgt_sim(50,-0.12,0.06)");
	hUxtgt_sim->GetXaxis()->SetTitle("Unit vector_x(target) sim");	
	c2.cd(2);
	t_sim->Draw("BeamDetTrack.fVectorOnTarget.Unit().X()>>hUxtgt_reco(50,-0.12,0.06)");
	hUxtgt_reco->GetXaxis()->SetTitle("Unit vector_x(target) reco");		
	c2.cd(3);
	t_sim->Draw("MCEventHeader.fHe8b.Vect().Unit().X() - BeamDetTrack.fVectorOnTarget.Unit().X()>>hdUxtgt(200,-0.06,0.06)");	//>>hdUxtgt(50,-0.06,0.06)
	TF1 *g2 = new TF1("g2", "gaus", -0.01,0.01);
	hdUxtgt->Fit(g2);
	hdUxtgt->GetXaxis()->SetTitle("Unit vector_x(target)_sim - Unit vector_x(target)_reco");
	c2.cd(4);
	t_sim->Draw("MCEventHeader.fHe8b.Vect().Unit().Y()>>hUytgt_sim(50,-0.12,0.06)");
	hUytgt_sim->GetXaxis()->SetTitle("Unit vector_y(target) sim");	
	c2.cd(5);
	t_sim->Draw("BeamDetTrack.fVectorOnTarget.Unit().Y()>>hUytgt_reco(50,-0.12,0.06)");
	hUytgt_reco->GetXaxis()->SetTitle("Unit vector_y(target) reco");
	c2.cd(6);
	t_sim->Draw("MCEventHeader.fHe8b.Vect().Unit().Y() - BeamDetTrack.fVectorOnTarget.Unit().Y()>>hdUytgt(200,-0.06,0.06)");	//>>hdUytgt(50,-0.06,0.06)
	hdUytgt->Fit(g2);
	hdUytgt->GetXaxis()->SetTitle("Unit vector_y(target)_sim - Unit vector_y(target)_reco");
	
	
	//mass of He8 beam is 7482.5396 MeV
	name.Form("BeamDetParticle energy on target");	
	TCanvas c3("c3", name.Data(), w, h);
	c3.Divide(2,2);
	c3.cd(1);
	t_sim->Draw("MCEventHeader.fHe8b.fE*1000-7482.5396>>hE_sim(50,100,240)",cut);	
	hE_sim->GetXaxis()->SetTitle("Simulated beam energy, MeV");
	c3.cd(2);
	t_sim->Draw("MCEventHeader.fHe8b.fE*1000-BeamDetParticle.fTargetState.fE>>hdE_bdt(100,-2,2)",cut);	//>>hdE_bdt(50,-2,2)
	TF1 *g3 = new TF1("g3", "gaus", -0.5,0.5);
	hdE_bdt->Fit(g3);
	hdE_bdt->GetXaxis()->SetTitle("Sim - reco beam energy, MeV");	
	c3.cd(3);
	t_sim->Draw("BeamDetParticle.fTargetState.fE-7482.5396>>hE_reco(50,100,240)",cut);
	hE_reco->GetXaxis()->SetTitle("Reconstructed beam energy, MeV");
	c3.cd(4);
	t_sim->Draw("MCEventHeader.fHe8b.fE*1000-7482.5396:BeamDetParticle.fTargetState.fE-7482.5396>>hEsimVSreco(50,100,240,50,100,240)",cut,"cont z");
	hEsimVSreco->GetXaxis()->SetTitle("Reconstructed beam energy, MeV");
	hEsimVSreco->GetYaxis()->SetTitle("Simulated beam energy, MeV");
	
}
