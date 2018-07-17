void calculate_all();

void calc_parallel(Int_t index = 0)
{
    TString workDir = "output_parallel";
    // Input file name formation
    TString simFilename;
    simFilename.Form("%s/sim_%d.root", workDir.Data(), index);

    // File reading
    TFile* simF = new TFile(simFilename, "READ");
    if (simF->IsZombie()) {
        cerr << "File " << simFilename << " " << "was not opened" << endl;
	return;
    } else {
        cout << "File " << simFilename << " " << "was opened" << endl;
    }

    // Tree reading
    TTree* simT = (TTree*)simF->Get("er");
    if (simT == NULL) {
        cerr << "Tree 'er' was not found" << endl;
	return;
    } else {
        cout << "Tree 'er' was found" << endl;
    }

    // Input branchs
    TBranch* gasBr = simT->GetBranch("Gas_Layer_0_Det_1_Point");
        TBranch* semiBr = simT->GetBranch("Semi_Layer_0_Det_1_Point");
    if (gasBr == NULL) {
        cerr << "Branch 'Gas_Layer_0_Det_1_Point' was not found" << endl;
        return;
    }
    if (semiBr == NULL) {
        cerr << "Branch 'Semi_Layer_0_Det_1_Point' was not found" << endl;
        return;
    }

    // Form and set adress to data arrays
    TClonesArray* gasArr = new TClonesArray("ERPoint");
    gasBr->SetAddress(&gasArr);
    TClonesArray* semiArr = new TClonesArray("ERPoint");
    semiBr->SetAddress(&semiArr);

    UInt_t nEvents = simT->GetEntries();
    cout << "Number of events is: " << nEvents << endl;

    // B11 and N15 numbers in gas
    UInt_t gasB11Points = 0;
    UInt_t gasN15Points = 0;

    // B11 and N15 numbers in semi
    UInt_t semiB11Points = 0;
    UInt_t semiN15Points = 0;

    // Loop over events
    for (UInt_t iEvent=0; iEvent<nEvents; iEvent++) {

        gasBr->GetEntry(iEvent);
        semiBr->GetEntry(iEvent);

        UInt_t nGasPoints = gasBr->GetEntries();
        UInt_t nSemiPoints = semiBr->GetEntries();

        ERPoint* gasPoint;
        TIter gasEventPointsIter(gasArr);

        // Loop over points in gas
        while ((gasPoint = (ERPoint*)gasEventPointsIter.Next())) {

                Double_t curCharge = gasPoint->GetCharge();

                if (curCharge == 5) {
                    gasB11Points++;  // must added only one from one event
                    break;
                }
                else if (curCharge == 7) {
                    gasN15Points++; // must added only one from one event
                    break;
                }
                //else break;
        } // loop over points in gas end

        ERPoint* semiPoint;
        TIter semiEventPointsIter(semiArr);
        // Loop over points in semi
        while ((semiPoint = (ERPoint*)semiEventPointsIter.Next())) {

            Double_t curCharge = semiPoint->GetCharge();

                if (curCharge == 5) {
                    semiB11Points++;  // must added only one from one event
                    break;
                }
                else if (curCharge == 7) {
                    semiN15Points++; // must added only one from one event
                    break;
                }
                else break;
        } // loop over points in semi end
    } // for end

    TString outFileName;
    outFileName.Form("%s/numbers.txt", workDir.Data());
    FILE* file = freopen(outFileName, "a", stdout);
    cout << gasB11Points << "\t" << semiB11Points << endl;
    cout << gasN15Points << "\t" << semiN15Points << endl;
    fclose(file);

    calculate_all();
}
//-------------------------------------------------------------------------------------------------------------------
void calculate_all()
{
    UInt_t numB11gas = 0;
    UInt_t numB11semi = 0;
    UInt_t numN15gas = 0;
    UInt_t numN15semi = 0;


    FILE* file = fopen("output_parallel/numbers.txt", "r");
    if (file == 0) {
        cerr << "numbers.txt was not found" << endl;
    return;
    } else {

        UInt_t iNum = 0;
        UInt_t iT = 0;
        while (fscanf(file, "%d", &iNum) > 0) {
            if (iT == 0) {
                numB11gas += iNum;
                iT++;
            }
            else if (iT == 1) {
                numB11semi += iNum;
                iT++;
            }
            else if (iT == 2) {
                numN15gas += iNum;
                iT++;
            }
            else if (iT == 3) {
                numN15semi += iNum;
                iT = 0;
            }

        }
        fclose(file);
    }
/*
    cout << "nB11Gas: " << numB11gas << "\t" << "nB11Semi: " << numB11semi << endl;
    cout << "nN15Gas: " << numN15gas << "\t" << "nN15Semi: " << numN15semi << endl;
*/
    cout << "nB11Gas:" << numB11gas << endl;
    cout << "nN15Gas:" << numN15gas << endl;
}
//-----------------------------------------------------------------------------------------------------------------
