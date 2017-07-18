void merge()
{
   	TChain ch("cbmsim");

	ch.Add("/store/ivan/simParallel/analyze1.root");
        ch.Add("/store/ivan/simParallel/analyze2.root");
        ch.Add("/store/ivan/simParallel/analyze3.root");
        ch.Add("/store/ivan/simParallel/analyze4.root");
        ch.Add("/store/ivan/simParallel/analyze5.root");
        ch.Add("/store/ivan/simParallel/analyze6.root");
        ch.Add("/store/ivan/simParallel/analyze7.root");
        ch.Add("/store/ivan/simParallel/analyze8.root");
        ch.Add("/store/ivan/simParallel/analyze9.root");
        ch.Add("/store/ivan/simParallel/analyze10.root");

	ch.Merge("/store/ivan/simGSIJULY/analyze4.root");
} 
