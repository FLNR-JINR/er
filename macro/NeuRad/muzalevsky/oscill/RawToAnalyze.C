//using namespace std;
void RawToAnalyze(Int_t nEvents = 1000){
  //---------------------Files-----------------------------------------------
  /*TString inFile = "/home/muzalevsky/work/dataER/simNeuRad/simRaw1_8000_20.root";
  TString outFile = "/home/muzalevsky/work/dataER/simNeuRad/analyze1_8000_20.root";
  TString parFile = "/home/muzalevsky/work/dataER/simNeuRad/parColl1.root";*/


  TString inFile = "/home/muzalevsky/work/dataER/simNeuRad/tests/simraw6000_0.root";
  TString outFile = "/home/muzalevsky/work/dataER/simNeuRad/tests/analyze6000_0_200.root";
  TString parFile = "/home/muzalevsky/work/dataER/simNeuRad/parColl2.root";

  //TString inFile = "/home/muzalevsky/work/dataER/drs4.out.root";
  
//  TString outFile = "/home/muzalevsky/work/dataER/simNeuRad/analyzeEXP.root";
  // ------------------------------------------------------------------------
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
  
  // -----   Digitization run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(inFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  // ---------------------------------------------------------
  Int_t verbose = 1;
 // Int_t sm_flag = 1; //1 - smooth, 0 - no smooth
  ERRawToAnalyzeConverter* converter = new ERRawToAnalyzeConverter(verbose);
  converter->SetNPoints(1000);
  converter->SetNChanels(2);
  converter->SetSmooth(0);  //1 - smooth, 0 - no smooth

  fRun->AddTask(converter);
  // ------------------------------------------------------------------------
  
  // -----------Runtime DataBase info -------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parInput = new FairParRootFileIo();
  parInput->open(parFile.Data(), "UPDATE");
  
  rtdb->setFirstInput(parInput);  
  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------
  rtdb->setOutput(parInput);
  rtdb->saveOutput();
  
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file writen:  "    << outFile << endl;
  cout << "Parameter file writen " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------
}
