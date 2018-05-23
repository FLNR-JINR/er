{
  TChain ch("er");
	ch.Add("sim_digi1.root");
	ch.Add("sim_digi2.root");
	ch.Add("sim_digi3.root");
	ch.Add("sim_digi4.root");
	ch.Merge("sim_digidigi.root");
}