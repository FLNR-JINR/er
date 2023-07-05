{
	TString inFilelab = "sim_digi.root";
	TString inFilereco = "./reco/reco_sim_digi.root";
	
	TFile *f_sim,*f_reco;
	f_sim = new TFile(inFilelab,"READ");
	TTree *t_sim = (TTree*)f_sim -> Get("er");
	f_reco = new TFile(inFilereco,"READ");
	TTree *t_reco = (TTree*)f_reco -> Get("er");
	
	t_sim->AddFriend(t_reco);
	
	Double_t w = 1500;
	Double_t h = 1000;	
	TString name;
	
	TCut cut = "TelescopeParticle_Telescope_he8_SingleSi_SSD20_XTelescope_he8_SingleSi_SSD20_1_Y_1000020080@.GetEntries()==1 && TelescopeParticle_Telescope_proton_DoubleSi_R_XY_1000010010@.GetEntries()==1 && MCEventHeader.fTrigger >0";
	TCut mult = "NDDigi@.GetEntries()==1";
	
	name.Form("ndtrack position on target");
	TCanvas c1("c1", name.Data(), w, h);
	c1.Divide(3,2);
	c1.cd(1);
	t_sim->Draw("MCEventHeader.fReactionPos.X()>>hX_sim_n(50,-1.1,1.3)",cut&&mult);
		hX_sim_n->GetXaxis()->SetTitle("X(target)_sim, cm");
		hX_sim_n->GetYaxis()->SetTitle("counts");
	c1.cd(2);	
	t_sim->Draw("NDTrack.fTargetVertex.fX>>hX_reco_n(50,-1.1,1.3)", cut&&mult);	
		hX_reco_n->GetXaxis()->SetTitle("X(target)_reco, cm");
		hX_reco_n->GetYaxis()->SetTitle("counts");
	c1.cd(3);
	t_sim->Draw("MCEventHeader.fReactionPos.X()-NDTrack.fTargetVertex.fX>>hX_diff_n(50,-1,1)",cut&&mult);
	TF1 *g1 = new TF1("g1", "gaus", -0.2,0.2);
	hX_diff_n->Fit(g1);
	gStyle->SetOptFit(1);
		hX_diff_n->GetXaxis()->SetTitle("X(target)_sim - X(target)_reco, cm");
		hX_diff_n->GetYaxis()->SetTitle("counts");
	c1.cd(4);
	t_sim->Draw("MCEventHeader.fReactionPos.Y()>>hY_sim_n(50,-1.1,1.3)",cut&&mult);
		hY_sim_n->GetXaxis()->SetTitle("Y(target)_sim, cm");
		hY_sim_n->GetYaxis()->SetTitle("counts");
	c1.cd(5);
	t_sim->Draw("NDTrack.fTargetVertex.fY>>hY_reco_n(50,-1.1,1.3)", cut&&mult);	
		hY_reco_n->GetXaxis()->SetTitle("Y(target)_reco, cm");
		hY_reco_n->GetYaxis()->SetTitle("counts");
	c1.cd(6);
	t_sim->Draw("MCEventHeader.fReactionPos.Y()-NDTrack.fTargetVertex.fY>>hY_diff_n(50,-1,1)",cut&&mult);
	hY_diff_n->Fit(g1);
		hY_diff_n->GetXaxis()->SetTitle("Y(target)_sim - Y(target)_reco, cm");
		hY_diff_n->GetYaxis()->SetTitle("counts");


	name.Form("ndparticle unit vector on target");
	TCanvas c2("c2", name.Data(), w, h);
	c2.Divide(3,2);
	c2.cd(1);
	t_sim->Draw("MCEventHeader.fn1.Vect().Unit().X()>>hUx_sim_n(50,-0.06,0.06)", cut&&mult);
		hUx_sim_n->GetXaxis()->SetTitle("Unit vector_x(target) sim");
		hUx_sim_n->GetYaxis()->SetTitle("counts");
	c2.cd(2);
	t_sim->Draw("NDParticle.fLV.fP.Unit().x()>>hUx_reco_n(50,-0.06,0.06)", cut&&mult);
		hUx_reco_n->GetXaxis()->SetTitle("Unit vector_x(target) reco");
		hUx_reco_n->GetYaxis()->SetTitle("counts");
	c2.cd(3);
	t_sim->Draw("MCEventHeader.fn1.Vect().Unit().X()-NDParticle.fLV.fP.Unit().x()>>hUx_diff_n(50,-0.05,0.05)", cut&&mult); 
	TF1 *g2 = new TF1("g2", "gaus", -0.01,0.01);
	hUx_diff_n->Fit(g2);
		hUx_diff_n->GetXaxis()->SetTitle("Unit vector_x(target)_sim - Unit vector_x(target)_reco");
		hUx_diff_n->GetYaxis()->SetTitle("counts");
	c2.cd(4);
	t_sim->Draw("MCEventHeader.fn1.Vect().Unit().Y()>>hUy_sim_n(50,-0.06,0.06)", cut&&mult);
		hUy_sim_n->GetXaxis()->SetTitle("Unit vector_y(target) sim");
		hUy_sim_n->GetYaxis()->SetTitle("counts");
	c2.cd(5);
	t_sim->Draw("NDParticle.fLV.fP.Unit().y()>>hUy_reco_n(50,-0.06,0.06)", cut&&mult);
		hUy_reco_n->GetXaxis()->SetTitle("Unit vector_y(target) reco");
		hUy_reco_n->GetYaxis()->SetTitle("counts");
	c2.cd(6);
	t_sim->Draw("MCEventHeader.fn1.Vect().Unit().Y()-NDParticle.fLV.fP.Unit().y()>>hUy_diff_n(50,-0.05,0.05)", cut&&mult);
	hUy_diff_n->Fit(g2);
		hUy_diff_n->GetXaxis()->SetTitle("Unit vector_y(target)_sim - Unit vector_y(target)_reco");
		hUy_diff_n->GetYaxis()->SetTitle("counts");
		
		
	name.Form("ndparticle energy on target");
	TCanvas c3("c3",  name.Data(), w, h);
	c3.Divide(2,2);
	c3.cd(1);
	t_sim->Draw("MCEventHeader.fn1.T()*1000 - MCEventHeader.fn1.Mag()*1000 >> hT_sim_n1(50,0,30)", cut&&mult); 	 
		hT_sim_n1->GetXaxis()->SetTitle("Simulated neutron energy, MeV");
		hT_sim_n1->GetYaxis()->SetTitle("counts");
	c3.cd(2);	
	t_sim->Draw("(MCEventHeader.fn1.T() - MCEventHeader.fn1.Mag()) * 1000. - (NDParticle.fLV.fE - NDParticle.fLV.Mag()) >> hTn1_compare(50,-2,2)", cut&&mult);
	TF1 *g3 = new TF1("g3", "gaus(0)", -0.5, 0.5);
	hTn1_compare->Fit(g3); 
		hTn1_compare->GetXaxis()->SetTitle("Sim - reco neutron energy, MeV");
		hTn1_compare->GetYaxis()->SetTitle("counts");
	c3.cd(3);
	t_sim->Draw("NDParticle.fLV.fE - NDParticle.fLV.Mag()>> hT_reco_n1(50,0.1,30)",cut&&mult);
		hT_reco_n1->GetXaxis()->SetTitle("Reconstructed neutron energy, MeV");
		hT_reco_n1->GetYaxis()->SetTitle("counts");
	c3.cd(4);
	t_sim->Draw("MCEventHeader.fn1.T()*1000 - MCEventHeader.fn1.Mag() * 1000.:(NDParticle.fLV.fE - NDParticle.fLV.Mag())>>hTn1_simVsReco(50,0.1,30,50,0,30.)",cut&&mult);
		hTn1_simVsReco->GetXaxis()->SetTitle("Reconstructed neutron energy, MeV");
		hTn1_simVsReco->GetYaxis()->SetTitle("Simulated neutron energy, MeV");
		hTn1_simVsReco->Draw("colz");	
		
		
}
