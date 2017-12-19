void merge()
{
   	TChain ch("cbmsim");

	ch.Add("/home/muzalevsky/work/dataER/simNeuRad/analyze2_8000_20.root");
    ch.Add("/home/muzalevsky/work/dataER/simNeuRad/analyze1_8000_20.root");

	ch.Merge("/home/muzalevsky/work/dataER/simNeuRad/file8000_20.root");
} 
