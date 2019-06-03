void digi(Int_t nEvents = 1000, Int_t index = 0, TString inputDir = "../output", TString outputDir = "output_digi")
{
    // input and output files name form
    TString inputFileName;
    TString outputFileName;
    TString parFileName;
    inputFileName.Form("%s/sim_%d.root", inputDir.Data(), index);
    outputFileName.Form("%s/digi_%d.root", outputDir.Data(), index);
    parFileName.Form("%s/par_%d.root", inputDir.Data(), index);

    TFile* inputFile = new TFile(inputFileName, "READ");
    if (inputFile->IsZombie())
    {
        cerr << "Could not open file '" << inputFileName << endl;
        return;
    }
    else
    {
        cout << inputFileName << " has been opened successfull" << endl;
    }

    TFile* inputParFile = new TFile(parFileName, "READ");
    if (inputParFile->IsZombie())
    {
        cerr << "Could not open file '" << parFileName << endl;
        return;
    }
    else
    {
        cout << parFileName << " has been opened successfull" << endl;
    }

    // Timer
    TStopwatch timer;
    timer.Start();

    // Digitization run
    ERRunAna*run = ERRunAna::Instance();
    run->SetInputFile(inputFileName);
    run->SetOutputFile(outputFileName);

    // Add Digitaizer AddTask
    ERN15B11Digitizer* digitizer = new ERN15B11Digitizer();
    run->AddTask(digitizer);

    // Runtime DataBase info
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    FairParRootFileIo* parInput = new FairParRootFileIo();
    parInput->open(parFileName.Data(), "READ");

    rtdb->setFirstInput(parInput);

    // Set Logger Level
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    // Initialization and run
    run->Init();
    run->Run(0, nEvents);

    rtdb->setOutput(parInput);
    rtdb->saveOutput();

    // Finish
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file writen:  "    << outputFileName << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
    cout << endl;
}
