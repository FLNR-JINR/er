//using namespace std;

void RawToAnalyze(Int_t nEvents = 10593){
  //---------------------Files-----------------------------------------------
  //TString inFile = "tektronix.out.root";
  TString inFile = "/home/ivanm/er/IvanM/NeuRad/oscill/data/simRaw1.root";
  TString outFile = "/home/ivanm/er/IvanM/NeuRad/oscill/data/analize1.root";
  TString parFile = "../NeuRad/oscill/par1.root";
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
  converter->SetNPoints(1024);
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
