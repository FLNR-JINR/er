void merge()
{
   	TChain ch("cbmsim");

	ch.Add("/home/muzalevsky/work/dataER/simNeuRad/6000_3.2_noCross/analyze2_100_200.root");
    ch.Add("/home/muzalevsky/work/dataER/simNeuRad/6000_3.2_noCross/analyze1_100_200.root");

	ch.Merge("/home/muzalevsky/work/dataER/simNeuRad/6000_3.2_noCross/analyze_100_200.root");
} 
