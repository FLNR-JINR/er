#include "ERRTelescopeRawConverter.h"

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
ERRTelescopeRawConverter::ERRTelescopeRawConverter()
    : FairTask("ER rtelescope convertation"),
    fRTelescope1Si1DigiR(NULL),
    fRTelescope1Si1DigiS(NULL),
    fRTelescope1Si2DigiS(NULL),
    fRTelescope1CsIDigi(NULL),
    fRTelescope2Si1DigiR(NULL),
    fRTelescope2Si1DigiS(NULL),
    fRTelescope2Si2DigiS(NULL),
    fRTelescope2CsIDigi(NULL),
    fAculRaw(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeRawConverter::ERRTelescopeRawConverter(Int_t verbose)
    : FairTask("ER rtelescope convertation ", verbose),
    fRTelescope1Si1DigiR(NULL),
    fRTelescope1Si1DigiS(NULL),
    fRTelescope1Si2DigiS(NULL),
    fRTelescope1CsIDigi(NULL),
    fRTelescope2Si1DigiR(NULL),
    fRTelescope2Si1DigiS(NULL),
    fRTelescope2Si2DigiS(NULL),
    fRTelescope2CsIDigi(NULL),
    fAculRaw(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeRawConverter::~ERRTelescopeRawConverter()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeRawConverter::SetParContainers()
{
    // Get run and runtime database
    FairRunAna* run = FairRunAna::Instance();
    if ( ! run ) Fatal("SetParContainers", "No analysis run");

    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERRTelescopeRawConverter::Init()
{
    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");

    fRTelescope1Si1DigiS = (TClonesArray*) ioman->GetObject("RTelescope1Si1DigiS");
    if ( ! fRTelescope1Si1DigiS) Fatal("Init", "Can`t find collection RTelescope1Si1DigiS!");

    fRTelescope1Si1DigiR = (TClonesArray*) ioman->GetObject("RTelescope1Si1DigiR");
    if ( ! fRTelescope1Si1DigiR) Fatal("Init", "Can`t find collection RTelescope1Si1DigiR!");
    
    fRTelescope1Si2DigiS = (TClonesArray*) ioman->GetObject("RTelescope1Si2DigiS");
    if ( ! fRTelescope1Si2DigiS) Fatal("Init", "Can`t find collection RTelescope1Si2DigiS!");
    
    fRTelescope1CsIDigi = (TClonesArray*) ioman->GetObject("RTelescope1CsIDigi");
    if ( ! fRTelescope1CsIDigi) Fatal("Init", "Can`t find collection RTelescope1CsIDigi!");

    
    fRTelescope2Si1DigiS = (TClonesArray*) ioman->GetObject("RTelescope2Si1DigiS");
    if ( ! fRTelescope2Si1DigiS) Fatal("Init", "Can`t find collection RTelescope2Si1DigiS!");

    fRTelescope2Si1DigiR = (TClonesArray*) ioman->GetObject("RTelescope2Si1DigiR");
    if ( ! fRTelescope2Si1DigiR) Fatal("Init", "Can`t find collection RTelescope2Si1DigiR!");
    
    fRTelescope2Si2DigiS = (TClonesArray*) ioman->GetObject("RTelescope2Si2DigiS");
    if ( ! fRTelescope2Si2DigiS) Fatal("Init", "Can`t find collection RTelescope2Si2DigiS!");
    
    fRTelescope2CsIDigi = (TClonesArray*) ioman->GetObject("RTelescope2CsIDigi");
    if ( ! fRTelescope2CsIDigi) Fatal("Init", "Can`t find collection RTelescope2CsIDigi!");

    // Register output array fRTelescopeHits
    fAculRaw = new TClonesArray("AculRaw", 1);
    ioman->Register("RTelescopeAculRaw", "RTelescope Acul Raw", fAculRaw, kTRUE);

  /*fRTelescopeSetup = ERRTelescopeSetup::Instance();
    fRTelescopeSetup->Print();*/

    return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERRTelescopeRawConverter::Exec(Option_t *opt)
{
    Reset();

    GetParameters();

    vector<Int_t>               SiDigi;
    vector<Int_t>::iterator     it_Nb;

    TClonesArray *SiBranche = NULL;

    for(Int_t i = 0; i < 6; ++i)
    {
        switch(i)
        {
            case 0: SiBranche = fRTelescope1Si1DigiS;
                    break;
            case 1: SiBranche = fRTelescope1Si1DigiR;
                    break;
            case 2: SiBranche = fRTelescope1Si2DigiS;
                    break;
            case 3: SiBranche = fRTelescope2Si1DigiS;
                    break;                   
            case 4: SiBranche = fRTelescope2Si1DigiR;
                    break;
            case 5: SiBranche = fRTelescope2Si2DigiS;
                    break;
        }

        for(Int_t iDigi = 0; iDigi < SiBranche->GetEntriesFast(); ++iDigi)
        {
            ERRTelescopeSiDigi *digi = (ERRTelescopeSiDigi *)SiBranche->At(iDigi);
            SiDigi.push_back(iDigi);
        }

        for(it_Nb = SiDigi.begin(); it_Nb < SiDigi.end(); ++it_Nb)
        {
            ERRTelescopeSiDigi *si_digi = (ERRTelescopeSiDigi*)(SiBranche->At(*it_Nb));
            Int_t telescopeNb = si_digi->TelescopeNb();
            Int_t detectorNb = si_digi->DetectorNb();
            Int_t Side = si_digi->Side();                   // 1 - sector, 0 - ring
            Int_t Nb = si_digi->Nb(); 
            Int_t edep = si_digi->Edep();

            Int_t j = 80 * (telescopeNb - 1) + (16 * detectorNb * (Side * detectorNb == 1 ? 0 : 1)) + Nb;
            Int_t StationNb = parameters_Si[j][0];
            Int_t ChanelNb = parameters_Si[j][1];
            Int_t a = parameters_Si[j][2];
            Int_t b = parameters_Si[j][3];
            Int_t ACP = (edep - b) / a;

            ((AculRaw *)fAculRaw->At(0))->C3[StationNb][ChanelNb] = ACP;
        }
    }

    vector<Int_t>    CsIDigi;

    TClonesArray *CsIBranche = NULL;

    for(Int_t i = 0; i < 2; ++i)
    {
        switch(i)
        {
            case 0: CsIBranche = fRTelescope1CsIDigi;
                    break;
            case 1: CsIBranche = fRTelescope2CsIDigi;
                    break;
        }

        for(Int_t iDigi = 0; iDigi < CsIBranche->GetEntriesFast(); ++iDigi)
        {
            ERRTelescopeCsIDigi *digi = (ERRTelescopeCsIDigi *)CsIBranche->At(iDigi);
            CsIDigi.push_back(iDigi);
        }

        for(it_Nb = CsIDigi.begin(); it_Nb < CsIDigi.end(); ++it_Nb)
        {
            ERRTelescopeCsIDigi *csi_digi = (ERRTelescopeCsIDigi*)(CsIBranche->At(*it_Nb));
            Int_t telescopeNb = csi_digi->TelescopeNb();
            Int_t Nb = csi_digi->CrystallNB(); 
            Int_t edep = csi_digi->Edep();

            Int_t j = 16 * (telescopeNb - 1) + Nb;
            Int_t StationNb = parameters_CsI[j][0];
            Int_t ChanelNb = parameters_CsI[j][1];
            Int_t a = parameters_CsI[j][2];
            Int_t b = parameters_CsI[j][3];
            Int_t ACP = (edep - b) / a;

            ((AculRaw *)fAculRaw->At(0))->C3[StationNb][ChanelNb] = ACP;
        }
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeRawConverter::GetParameters()
{
    Int_t     i = 0;
    Int_t     fi;    // first int
    Int_t     si;     // second int
    Int_t     ti;
    Double_t  df;  // double first
    Double_t  ds;  // second
    Double_t  dt;  // third
    Double_t  dfo;
    FILE     *pSiFile;
    FILE     *pCsIFile;

  pSiFile = fopen ("beSi1.cal","a+");
  pCsIFile = fopen ("beCsIp1.cal","a+");  //"beCsIa1.cal"

  while ( fscanf (pSiFile, "%d %d %d %lf %lf %lf \n", &fi, &si, &ti, &df, &ds, &dt) == 6  )
  {
    parameters_Si[i][0]= si;
    parameters_Si[i][1]= ti;
    parameters_Si[i][2]= df;
    parameters_Si[i][3]= ds;
    ++i;
  }

  i = 0;
 
  while ( fscanf (pCsIFile, "%d %d %d %lf %lf %lf %lf \n", &fi, &si, &ti, &df, &ds, &dt, &dfo) == 7   )
  {
    parameters_CsI[i][0]= si;
    parameters_CsI[i][1]= ti;
    parameters_CsI[i][2]= df;
    parameters_CsI[i][3]= ds;
    parameters_CsI[i][4]= dt;
    parameters_CsI[i][5]= dfo;
    ++i;
  }

  fclose (pSiFile);
  fclose (pCsIFile);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeRawConverter::Reset()
{
    if (fAculRaw) 
        fAculRaw->Delete();
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void ERRTelescopeRawConverter::Finish() 
{ 
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERRTelescopeRawConverter)
