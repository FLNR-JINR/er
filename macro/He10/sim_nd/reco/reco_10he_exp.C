#include <string>

// int fTriggerNum = 2;
int fTriggerNum = 1;
void reco_10he_exp(){
  //---------------------Files-----------------------------------------------
TString inFile = "sim_digi.root";
TString outFile = inFile;
inFile.Prepend("../");

auto file = TFile::Open(inFile.Data());
auto tree = (TTree*)file->Get("er");
Int_t nEvents = tree->GetEntriesFast();

TString parFile = "../par.root";
TString geoFile = inFile;
Ssiz_t p1 = inFile.First("/");
geoFile.Insert(p1+1,"setup_");

  // -----   Timer   --------------------------------------------------------
TStopwatch timer;
timer.Start();  


  // -----   Digitization run   ---------------------------------------------
ERRunAna *run = ERRunAna::Instance();
run->HoldEventsCount(); //forbid different entry number in the input and output file
TString userCut;
  
Ssiz_t p2 = outFile.First(".");
outFile.Replace(p2,5,".target.root");

run->SetGeomFile(geoFile);
run->SetInputFile(inFile);
run->SetOutputFile(outFile);


  // ------------------------BeamDetTrackFinger--------------------------------
Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
trackFinder->SetTargetVolume("target3HVol");   
run->AddTask(trackFinder);


   // -----------------------BeamDetTrackPID------------------------------------
Int_t Z = 2, A = 8, Q = 2;
TString ionName = "8He";
ERBeamDetPID* beamdetPid = new ERBeamDetPID(verbose);
beamdetPid->SetBoxPID(0., 1000., 0., 1000.);
beamdetPid->SetProbabilityThreshold(0);
beamdetPid->SetIonMass(7482.5396);
beamdetPid->SetPID(1000020080);
run->AddTask(beamdetPid);
  
  
  // ------- QTelescope TrackFinder -------------------------------------------
ERTelescopeTrackFinder* qtelescopeTrackFinder = new ERTelescopeTrackFinder(verbose);
qtelescopeTrackFinder->SetHitStation("Telescope_he8", "Telescope_he8_SingleSi_SSD20_X", "Telescope_he8_SingleSi_SSD20_1_Y");
qtelescopeTrackFinder->SetHitStation("Telescope_proton", "Telescope_proton_DoubleSi_R_XY");
run->AddTask(qtelescopeTrackFinder);
  
  
  // ------   QTelescope TrackPID -----------------------------------------
ERTelescopePID* qtelescopePID = new ERTelescopePID(verbose);
TString deStation1 = "SSD20_1";
TString eStation1 = "SSD_1";
Double_t normalizedThickness = 0.002; // [cm]
Double_t DSD_thickness = 0.15; // [cm]

qtelescopePID->SetParticle("Telescope_proton_DoubleSi_R_XY", 1000010010, "R", "Si", DSD_thickness);	//geant перестал понимать обозначение протона
qtelescopePID->SetEdepAccountingStrategy("R", ERTelescopePID::EdepAccountingStrategy::EdepFromXChannel);
qtelescopePID->SetParticle("Telescope_he8_SingleSi_SSD20_XTelescope_he8_SingleSi_SSD20_1_Y", 1000020080, 
                             "SSD20_1", {"SSD20_1","SSD20_2","SSD20_3", "SSD20_4", "SSD20_5"}, normalizedThickness, {});
run->AddTask(qtelescopePID);
  
  
   // ------------------------ND track finder --------------------------------
ERNDTrackFinder* trackFinderND = new ERNDTrackFinder();
run->AddTask(trackFinderND);

  
  // ------------------------ND track finder --------------------------------
ERNDPID* pid = new ERNDPID();
run->AddTask(pid);


  
  // -----------Runtime DataBase info ---------------------------------------
FairRuntimeDb* rtdb = run->GetRuntimeDb();
FairParRootFileIo*  parIO = new FairParRootFileIo();
parIO->open(parFile.Data(), "UPDATE");
rtdb->print();
  
run->Init();
run->Run(0, nEvents);


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
