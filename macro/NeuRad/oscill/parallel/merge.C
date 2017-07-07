{
   	TChain ch("cbmsim");
	ch.Add("sim1.root");
	ch.Add("sim2.root");
	ch.Add("sim3.root");
	ch.Add("sim4.root");
	ch.Add("sim5.root");
	ch.Add("sim6.root");
	ch.Add("sim7.root");
	ch.Add("sim8.root");

	ch.Merge("sim.root");
}