#include "ERN15B11Digitizer.h"
/*
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
*/
#include <TClonesArray.h>

#include "ERDigi.h"
#include "ERPoint.h"

//UInt_t iEvent = 0;
ERN15B11Digitizer::ERN15B11Digitizer()
  : FairTask()
{
    //cout << "ERN15B11Digitizer::ERN15B11Digitizer()" << endl;
}

ERN15B11Digitizer::~ERN15B11Digitizer()
{
    //cout << "ERN15B11Digitizer::~ERN15B11Digitizer()" << endl;
}

InitStatus ERN15B11Digitizer::Init()
{
//    cout << "ERN15B11Digitizer::Init" << endl;
    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");

    TList* BranchsNamesList = ioman->GetBranchNameList();
    TIter next(BranchsNamesList);
    TObjString* branchName;
    while (branchName = (TObjString*)next()) {
        TString curBranchName = branchName->GetString();
//        cout << "Current Branch Name is: " << curBranchName << endl;
        if (curBranchName.Contains("Point")) {
//            cout << "Current Branch Name is: " << curBranchName << endl;
            // Input
            fPoints[curBranchName] = (TClonesArray*) ioman->GetObject(curBranchName); // input branch name
            if (!fPoints[curBranchName]) {
                Fatal("Init", "Can't find input collection DetectorvDetGasPartPoint!");
            }

            // Output
            fDigis[curBranchName] = new TClonesArray("ERDigi"); // class name
            ioman->Register(curBranchName(0, curBranchName.Length()-5)+"Digi", curBranchName, fDigis[curBranchName], kTRUE);
        }
    }

    return kSUCCESS;
}

void ERN15B11Digitizer::Exec(Option_t* option)
{
//    cout << "ERN15B11Digitizer::Exec" << endl;
    for(const auto &itPoints: fPoints) {
        TClonesArray* curBranchArray = itPoints.second;
        Double_t curEdep = 0.;
        Double_t curTime = 0.;
        Int_t curEntries = curBranchArray->GetEntries();
//        cout << "iEvent: " << iEvent << "\t" << "Entries: " << curEntries << endl;
        for (UInt_t i = 0; i < curEntries; i++) {
            ERPoint* curPoint = (ERPoint*)curBranchArray->At(i);
            curEdep = curEdep + curPoint->GetEnergyLoss();
            curTime = curTime + curPoint->GetTime();
/*
            if (curPoint->GetCharge() == 5) {
                cout << "B11 has been found" << endl;
                cout << "Event: " << iEvent << "\t" << "Branch: " << itPoints.first << endl;
                cout << "Time: " << curTime << endl;
                cout << "VolNb: " << curPoint->GetVolNb() << endl;
                cout << "curEdep: " << curEdep << endl;
            }
*/
        } // for end
        TClonesArray* curOutBra = fDigis[itPoints.first];
        AddOutputDigi(*curOutBra, curEdep, curTime);
    } // for end
    //iEvent++;
}

void ERN15B11Digitizer::AddOutputDigi(TClonesArray& clref, Double_t Edep, Double_t Time)
{
/*    cout << "ERN15B11Digitizer::AddOutputDigi(Edep = " << Edep << ", "
           << "Time = " << Time << ")" << endl;
*/
    UInt_t size = clref.GetEntries();
    new(clref[size])ERDigi(1, Edep, Time, 1); // id = 1, Edep = Edep, time = Time, volNb = 1
}

void ERN15B11Digitizer::FinishEvent()
{
//    cout << "ERN15B11Digitizer::FinishEvent()" << endl;
    for (const auto &itfDigis: fDigis) {
        itfDigis.second->Clear();
    }
}

ClassImp(ERN15B11Digitizer)
