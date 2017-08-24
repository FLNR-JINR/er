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
    fSiPoints(NULL),
    fRTelescopeSiDigi(NULL),
    fCsIPoints(NULL),
    fRTelescopeCsIDigi(NULL),
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
    fSiPoints(NULL),
    fCsIPoints(NULL),
    fRTelescopeSiDigi(NULL),
    fRTelescopeCsIDigi(NULL),
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

    fSiPoints = (TClonesArray*) ioman->GetObject("RTelescopeSiPoint");
    if ( ! fSiPoints) Fatal("Init", "Can`t find collection RTelescopeSiPoint!");

    fCsIPoints = (TClonesArray*) ioman->GetObject("RTelescopeCsIPoint");
    if ( ! fCsIPoints) Fatal("Init", "Can`t find collection RTelescopeCsiPoint!");

    // Register output array fRTelescopeHits
    fRTelescopeSiDigi = new TClonesArray("ERRTelescopeSiDigi", 1000);
    ioman->Register("RTelescopeSiDigi", "RTelescope Si Digi", fRTelescopeSiDigi, kTRUE);
    fRTelescopeCsIDigi = new TClonesArray("ERRTelescopeCsIDigi", 1000);
    ioman->Register("RTelescopeCsIDigi", "RTelescope CsI Digi", fRTelescopeCsIDigi, kTRUE);

  /*fRTelescopeSetup = ERRTelescopeSetup::Instance();
    fRTelescopeSetup->Print();*/

    return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERRTelescopeDigitizer::Exec(Option_t *opt)
{
    Reset();
    //Sort the points by sensors and sectors
    //=====================================================
    // СТОИТ ЛИ ВЫНОСИТЬ ЦИКЛ В ОТДЕЛЬНУЮ ФУНКЦИЮ ?
    //=====================================================

    map<Int_t, map<Int_t, map<Int_t, vector<Int_t>>>> SiPoints_Sector;
    map<Int_t, map<Int_t, map<Int_t, vector<Int_t>>>> SiPoints_Ring;
  
    for (Int_t iPoint = 0; iPoint < fSiPoints->GetEntriesFast(); iPoint++)
    {
        ERRTelescopeSiPoint *point = (ERRTelescopeSiPoint *)fSiPoints->At(iPoint);
        SiPoints_Ring[point->GetTelescopeNb()][point->GetDetectorNb()][point->GetSensorNb()].push_back(iPoint);
        SiPoints_Sector[point->GetTelescopeNb()][point->GetDetectorNb()][point->GetSectorNb()].push_back(iPoint);
    }

    map<Int_t, map<Int_t, map<Int_t, vector<Int_t>>>>::iterator  itTelescope;
    map<Int_t, map<Int_t, vector<Int_t>>>::iterator              itDetector;
    map<Int_t, vector<Int_t>>::iterator                          iterNb;  //itSector or itRing ot itCrystall
    vector<Int_t>::iterator                                      itPoint;

    Int_t detectorNb = -1;
    Int_t telescopeNb = -1;
    Int_t side = 0; //  0 - ring; 1 - sector

    for(itTelescope = SiPoints_Ring.begin(); itTelescope != SiPoints_Ring.end(); ++itTelescope) 
    {
        for (itDetector = itTelescope->second.begin(); itDetector != itTelescope->second.end(); ++itDetector)
        {
            for(iterNb = itDetector->second.begin(); iterNb != itDetector->second.end(); ++iterNb) 
            {
                Float_t edep = 0.;
                Float_t time = numeric_limits<float>::max();
                ERRTelescopeSiPoint *Sipoint = NULL;

                for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                {
                    Sipoint = (ERRTelescopeSiPoint*)(fSiPoints->At(*itPoint));
                    edep += Sipoint->GetEnergyLoss();

                    if (Sipoint->GetTime() < time) 
                        time = Sipoint->GetTime();
                }

                if (edep < fElossThreshold)
                    continue;

                time = gRandom->Gaus(time, fTimeSigma);
                edep = gRandom->Gaus(edep, fElossSigma);
                telescopeNb = Sipoint->GetTelescopeNb();  // itTelescope->first ????
                detectorNb = Sipoint->GetDetectorNb(); // itDetector->first ????
                ERRTelescopeSiDigi  *si_digi = AddSiDigi(side, iterNb->first, telescopeNb, detectorNb, time, edep);

            for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                si_digi->AddLink(FairLink("RTelescopeSiPoint", *itPoint));
            }
        }
    }

    side = 1;

    for(itTelescope = SiPoints_Sector.begin(); itTelescope != SiPoints_Sector.end(); ++itTelescope) 
    {
        for (itDetector = itTelescope->second.begin(); itDetector != itTelescope->second.end(); ++itDetector)
        {
            for(iterNb = itDetector->second.begin(); iterNb != itDetector->second.end(); ++iterNb) 
            {
                Float_t edep = 0.;
                Float_t time = numeric_limits<float>::max();
                ERRTelescopeSiPoint *Sipoint = NULL;

                for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                {
                    Sipoint = (ERRTelescopeSiPoint*)(fSiPoints->At(*itPoint));
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
                ERRTelescopeSiDigi  *si_digi = AddSiDigi(side, iterNb->first, telescopeNb, detectorNb, time, edep);

            for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                si_digi->AddLink(FairLink("RTelescopeSiPoint", *itPoint));
            }
        }
    }

// CSI CSI CSI
    
    map<Int_t, map<Int_t, map<Int_t, vector<Int_t>>>> CsIpoints;
    
    for (Int_t iPoint = 0; iPoint < fCsIPoints->GetEntriesFast(); iPoint++)
    {
        ERRTelescopeCsIPoint *CsIpoint = (ERRTelescopeCsIPoint*)fCsIPoints->At(iPoint);
        CsIpoints[CsIpoint->GetTelescopeNb()][CsIpoint->GetDetectorNb()][CsIpoint->GetCrystallNb()].push_back(iPoint);
    }

    for(itTelescope = CsIpoints.begin(); itTelescope != CsIpoints.end(); ++itTelescope) 
    {
        for (itDetector = itTelescope->second.begin(); itDetector != itTelescope->second.end(); ++itDetector)
        {
            for(iterNb = itDetector->second.begin(); iterNb != itDetector->second.end(); ++iterNb) 
            {
                Float_t edep = 0.;
                Float_t time = numeric_limits<float>::max();
                ERRTelescopeCsIPoint *CsIpoint = NULL;

                for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                {
                    CsIpoint = (ERRTelescopeCsIPoint*)(fCsIPoints->At(*itPoint));
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
                telescopeNb = CsIpoint->GetTelescopeNb(); // itTelescope->first ????
                detectorNb = CsIpoint->GetDetectorNb(); // itDetector->first ????
                ERRTelescopeCsIDigi *csi_digi = AddCsIDigi(telescopeNb, detectorNb, edep, 2000 , iterNb->first);

                for (itPoint = iterNb->second.begin(); itPoint != iterNb->second.end(); ++itPoint)
                    csi_digi->AddLink(FairLink("RTelescopeCsIPoint", *itPoint));
            }
        }
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeDigitizer::Reset()
{
    if (fRTelescopeSiDigi) 
        fRTelescopeSiDigi->Delete();

    if (fRTelescopeCsIDigi) 
        fRTelescopeCsIDigi->Delete();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeDigitizer::Finish() 
{ 
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeSiDigi* ERRTelescopeDigitizer::AddSiDigi(Int_t side, Int_t Nb, Int_t telescopeNb, Int_t detectorNb, Double_t time, Float_t edep)
{
    ERRTelescopeSiDigi *si_digi = new((*fRTelescopeSiDigi)[fRTelescopeSiDigi->GetEntriesFast()])
            ERRTelescopeSiDigi(fRTelescopeSiDigi->GetEntriesFast(), side, Nb, telescopeNb, detectorNb, time, edep);  // Side = 0 => ring

    return si_digi;
}
// ----------------------------------------------------------------------------
ERRTelescopeCsIDigi* ERRTelescopeDigitizer::AddCsIDigi(Int_t telescopeNb, Int_t detectorNb, Float_t edep, Double_t time, Int_t crystall)
{
    ERRTelescopeCsIDigi *csi_digi = new((*fRTelescopeCsIDigi)[fRTelescopeCsIDigi->GetEntriesFast()])
        ERRTelescopeCsIDigi(fRTelescopeCsIDigi->GetEntriesFast(), telescopeNb, detectorNb, edep,time, crystall);
    
    return csi_digi;
}
//-----------------------------------------------------------------------------
ClassImp(ERRTelescopeDigitizer)
