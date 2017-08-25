#include "ERRTelescopeDigitizer.h"

#include <vector>

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <iostream>
#include <algorithm>
using namespace std;

#include "ERDetectorList.h"

// ----------------------------------------------------------------------------
ERRTelescopeDigitizer::ERRTelescopeDigitizer()
    : FairTask("ER rtelescope digitization"),
    fSiPoints11(NULL),
    fSiPoints12(NULL),
    fSiPoints21(NULL),
    fSiPoints22(NULL),
    fCsIPoints1(NULL),
    fCsIPoints2(NULL),
    fRTelescope1Si1DigiS(NULL),
    fRTelescope1Si1DigiR(NULL),
    fRTelescope1Si2DigiS(NULL),
    fRTelescope1CsIDigi(NULL),
    fRTelescope2Si1DigiS(NULL),
    fRTelescope2Si1DigiR(NULL),
    fRTelescope2Si2DigiS(NULL),
    fRTelescope2CsIDigi(NULL),
    fElossSigma(0),
    fTimeSigma(0),
    fElossThreshold(0),
    fDigiEloss(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeDigitizer::ERRTelescopeDigitizer(Int_t verbose)
    : FairTask("ER rtelescope digitization ", verbose),
    fSiPoints11(NULL),
    fSiPoints12(NULL),
    fSiPoints21(NULL),
    fSiPoints22(NULL),
    fCsIPoints1(NULL),
    fCsIPoints2(NULL),
    fRTelescope1Si1DigiS(NULL),
    fRTelescope1Si1DigiR(NULL),
    fRTelescope1Si2DigiS(NULL),
    fRTelescope1CsIDigi(NULL),
    fRTelescope2Si1DigiS(NULL),
    fRTelescope2Si1DigiR(NULL),
    fRTelescope2Si2DigiS(NULL),
    fRTelescope2CsIDigi(NULL),
    fElossSigma(0),
    fTimeSigma(0),
    fElossThreshold(0),
    fDigiEloss(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeDigitizer::~ERRTelescopeDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeDigitizer::SetParContainers()
{
    // Get run and runtime database
    FairRunAna* run = FairRunAna::Instance();
    if ( ! run ) Fatal("SetParContainers", "No analysis run");

    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERRTelescopeDigitizer::Init()
{
    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");

    fSiPoints11 = (TClonesArray*) ioman->GetObject("RTelescope1Si1Point");
    if ( ! fSiPoints11) Fatal("Init", "Can`t find collection RTelescope1Si1Point!");

    fSiPoints12 = (TClonesArray*) ioman->GetObject("RTelescope1Si2Point");
    if ( ! fSiPoints12) Fatal("Init", "Can`t find collection fSiPoints12!");

    fSiPoints21 = (TClonesArray*) ioman->GetObject("RTelescope2Si1Point");
    if ( ! fSiPoints21) Fatal("Init", "Can`t find collection fSiPoints21!");

    fSiPoints22 = (TClonesArray*) ioman->GetObject("RTelescope2Si2Point");
    if ( ! fSiPoints22) Fatal("Init", "Can`t find collection fSiPoints22!");

    fCsIPoints1 = (TClonesArray*) ioman->GetObject("RTelescope1CsIPoint");
    if ( ! fCsIPoints1) Fatal("Init", "Can`t find collection fCsIPoints1!");

    fCsIPoints2 = (TClonesArray*) ioman->GetObject("RTelescope2CsIPoint");
    if ( ! fCsIPoints2) Fatal("Init", "Can`t find collection fCsIPoints2!");

    // Register output array fRTelescopeHits
    fRTelescope1Si1DigiS = new TClonesArray("ERRTelescopeSiDigi", 1000);
    ioman->Register("RTelescope1Si1DigiS", "RTelescope1 Si1 DigiS", fRTelescope1Si1DigiS, kTRUE);

    fRTelescope1Si1DigiR = new TClonesArray("ERRTelescopeSiDigi", 1000);
    ioman->Register("RTelescope1Si1DigiR", "RTelescope1 Si1 DigiR", fRTelescope1Si1DigiR, kTRUE);

    fRTelescope1Si2DigiS = new TClonesArray("ERRTelescopeSiDigi", 1000);
    ioman->Register("RTelescope1Si2DigiS", "RTelescope1 Si2 DigiS", fRTelescope1Si2DigiS, kTRUE);

    fRTelescope1CsIDigi = new TClonesArray("ERRTelescopeSiDigi", 1000);
    ioman->Register("RTelescope1CsIDigi", "RTelescope1 CsI Digi", fRTelescope1CsIDigi, kTRUE);

    fRTelescope2Si1DigiS = new TClonesArray("ERRTelescopeSiDigi", 1000);
    ioman->Register("RTelescop2Si1DigiS", "RTelescope2 Si1 DigiS", fRTelescope2Si1DigiS, kTRUE);

    fRTelescope2Si1DigiR = new TClonesArray("ERRTelescopeSiDigi", 1000);
    ioman->Register("RTelescope2Si1DigiR", "RTelescope2 Si1 DigiR", fRTelescope2Si1DigiR, kTRUE);

    fRTelescope2Si2DigiS = new TClonesArray("ERRTelescopeSiDigi", 1000);
    ioman->Register("RTelescope2Si2DigiS", "RTelescope2 Si2 DigiS", fRTelescope2Si2DigiS, kTRUE);

    fRTelescope2CsIDigi = new TClonesArray("ERRTelescopeSiDigi", 1000);
    ioman->Register("RTelescope2CsIDigi", "RTelescope2 CsI Digi", fRTelescope2CsIDigi, kTRUE);

  /*fRTelescopeSetup = ERRTelescopeSetup::Instance();
    fRTelescopeSetup->Print();*/

    return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERRTelescopeDigitizer::Exec(Option_t *opt)
{
    Reset();

    map<Int_t, vector<Int_t>>             SiPoints_Sector;
    map<Int_t, vector<Int_t>>             SiPoints_Ring;
    map<Int_t, vector<Int_t>>::iterator   iterNb;
    vector<Int_t>::iterator               itPoint;

    TClonesArray *SiBranch = NULL;

    for (Int_t i = 0; i < 4; ++i)
    {   
        switch (i)
        {
            case 0: SiBranch = fSiPoints11;
                    break;
            case 1: SiBranch = fSiPoints12;
                    break;
            case 2: SiBranch = fSiPoints21;
                    break;
            case 3: SiBranch = fSiPoints22;
                    break;
        }

        for (Int_t iPoint = 0; iPoint < SiBranch->GetEntriesFast(); iPoint++)
        {
            ERRTelescopeSiPoint *point = (ERRTelescopeSiPoint *)SiBranch->At(iPoint);
            SiPoints_Sector[point->GetSectorNb()].push_back(iPoint);

            if (i % 2 == 0)
                SiPoints_Ring[point->GetSensorNb()].push_back(iPoint);
        }

        Int_t detectorNb = -1;
        Int_t telescopeNb = -1;
        Int_t side = 1; //  0 - ring; 1 - sector

        for(iterNb = SiPoints_Sector.begin(); iterNb != SiPoints_Sector.end(); ++iterNb)
        {
            Float_t edep = 0.;
            Float_t time = numeric_limits<float>::max();
            ERRTelescopeSiPoint *Sipoint = NULL;

            for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
            {
                Sipoint = (ERRTelescopeSiPoint*)(SiBranch->At(*itPoint));
                edep += Sipoint->GetEnergyLoss();
                        
                if (Sipoint->GetTime() < time)
                    time = Sipoint->GetTime();
            }

            if (edep < fElossThreshold)
                continue;

            time = gRandom->Gaus(time, fTimeSigma);
            edep = gRandom->Gaus(edep, fElossSigma);
            telescopeNb = Sipoint->GetTelescopeNb(); // itTelescope->first ????
            detectorNb = Sipoint->GetDetectorNb(); // itDetector->first ????
            Int_t k = 3*(telescopeNb - 1) + 1*(detectorNb - 1) + (side == 1 ? 0 : 1);
            ERRTelescopeSiDigi  *si_digi = AddSiDigi( k, side, iterNb->first, telescopeNb, detectorNb, time, edep);

            for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                si_digi->AddLink(FairLink("ERRTelescopeSiDigi", *itPoint));
        }

        SiPoints_Sector.clear();
        side = 0;

        if (i % 2 == 0)
        {
            for(iterNb = SiPoints_Ring.begin(); iterNb != SiPoints_Ring.end(); ++iterNb)
            {
                Float_t edep = 0.;
                Float_t time = numeric_limits<float>::max();
                ERRTelescopeSiPoint *Sipoint = NULL;

                for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                {
                    Sipoint = (ERRTelescopeSiPoint*)(SiBranch->At(*itPoint));
                    edep += Sipoint->GetEnergyLoss();
                        
                    if (Sipoint->GetTime() < time)
                        time = Sipoint->GetTime();
                }

                if (edep < fElossThreshold)
                    continue;

                time = gRandom->Gaus(time, fTimeSigma);
                edep = gRandom->Gaus(edep, fElossSigma);
                telescopeNb = Sipoint->GetTelescopeNb(); // itTelescope->first ????
                detectorNb = Sipoint->GetDetectorNb(); // itDetector->first ????
                Int_t k = 3*(telescopeNb - 1) + 1*(detectorNb - 1) + (side == 1 ? 0 : 1);
                ERRTelescopeSiDigi  *si_digi = AddSiDigi( k, side, iterNb->first, telescopeNb, detectorNb, time, edep);

                for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                    si_digi->AddLink(FairLink("ERRTelescopeSiDigi", *itPoint));
            }

            SiPoints_Ring.clear();
        }
    }

    map<Int_t, vector<Int_t>>            CsIpoints;

    TClonesArray *CsIBranch = NULL;

    for(int i = 0; i < 2; ++i)
    {
        if (! i)
            CsIBranch = fCsIPoints1;
        else
            CsIBranch = fCsIPoints2;

        for (Int_t iPoint = 0; iPoint < CsIBranch->GetEntriesFast(); iPoint++)
        {
            ERRTelescopeCsIPoint *CsIpoint = (ERRTelescopeCsIPoint*)CsIBranch->At(iPoint);
            CsIpoints[CsIpoint->GetCrystallNb()].push_back(iPoint);
        }

        for(iterNb = CsIpoints.begin(); iterNb != CsIpoints.end(); ++iterNb)
        {
            Float_t edep = 0.;
            Float_t time = numeric_limits<float>::max();
            ERRTelescopeCsIPoint *CsIpoint = NULL;

            for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
            {
                CsIpoint = (ERRTelescopeCsIPoint*)(CsIBranch->At(*itPoint));
                edep += CsIpoint->GetEnergyLoss();

                if (CsIpoint->GetTime() < time)
                    time = CsIpoint->GetTime();
            }

            if (edep < fElossThreshold)
                continue;

            Float_t LC = 1.;
            Float_t a = 0.07;
            Float_t b = 0.02;
            Float_t disp = a*a*edep + b*b*edep*edep;
            edep = gRandom->Gaus(edep*LC, disp);
            time = gRandom->Gaus(time, fTimeSigma); // time = 2000 ???
            Int_t telescopeNb = CsIpoint->GetTelescopeNb(); // itTelescope->first ????
            ERRTelescopeCsIDigi *csi_digi = AddCsIDigi( i, telescopeNb, edep, 2000 , iterNb->first);

            for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                csi_digi->AddLink(FairLink( "ERRTelescopeCsIDigi", *itPoint));
        }

        CsIpoints.clear();
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeDigitizer::Reset()
{
    if (fRTelescope1Si1DigiS)
        fRTelescope1Si1DigiS->Delete();

    if (fRTelescope1Si1DigiR)
        fRTelescope1Si1DigiR->Delete();

    if (fRTelescope1Si2DigiS)
        fRTelescope1Si2DigiS->Delete();

    if (fRTelescope2Si1DigiS)
        fRTelescope2Si1DigiS->Delete();

    if (fRTelescope2Si1DigiR)
        fRTelescope2Si1DigiR->Delete();

    if (fRTelescope2Si2DigiS)
        fRTelescope2Si2DigiS->Delete();

    if (fRTelescope1CsIDigi)
        fRTelescope1CsIDigi->Delete();

    if (fRTelescope2CsIDigi)
        fRTelescope2CsIDigi->Delete();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeDigitizer::Finish()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeSiDigi* ERRTelescopeDigitizer::AddSiDigi( Int_t k, Int_t side, Int_t Nb, Int_t telescopeNb, Int_t detectorNb, Double_t time, Float_t edep)
{
    TClonesArray *SiBranch = NULL;

    switch(k)
        {
            case 0: SiBranch = fRTelescope1Si1DigiS;
                    break;
            case 1: SiBranch = fRTelescope1Si1DigiR;
                    break;
            case 2: SiBranch = fRTelescope1Si2DigiS;
                    break;
            case 3: SiBranch = fRTelescope2Si1DigiS;
                    break;
            case 4: SiBranch = fRTelescope2Si1DigiR;
                    break;
            case 5: SiBranch = fRTelescope2Si2DigiS;
                    break;
        }

    ERRTelescopeSiDigi *si_digi = new((*SiBranch)[SiBranch->GetEntriesFast()])
            ERRTelescopeSiDigi(SiBranch->GetEntriesFast(), side, Nb, telescopeNb, detectorNb, time, edep);  // Side = 0 => ring

    return si_digi;
}
// ----------------------------------------------------------------------------
ERRTelescopeCsIDigi* ERRTelescopeDigitizer::AddCsIDigi( Int_t k, Int_t telescopeNb, Float_t edep, Double_t time, Int_t crystall)
{
    TClonesArray *CsIBranch = NULL;

    switch(k)
        {
            case 0: CsIBranch = fRTelescope1CsIDigi;
                    break;
            case 1: CsIBranch = fRTelescope2CsIDigi;
                    break;
        }

    ERRTelescopeCsIDigi *csi_digi = new((*CsIBranch)[CsIBranch->GetEntriesFast()])
        ERRTelescopeCsIDigi(CsIBranch->GetEntriesFast(), telescopeNb, edep, time, crystall);

    return csi_digi;
}
//-----------------------------------------------------------------------------
ClassImp(ERRTelescopeDigitizer)
