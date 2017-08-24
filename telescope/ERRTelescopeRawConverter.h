// -------------------------------------------------------------------------
// -----               ERRTelescopeDigitizer header file               -----
// -----                 Created   by   BMSTU students                 -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeRawConverter_H
#define ERRTelescopeRawConverter_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "AculRaw.h"
#include "ERRTelescopeSiDigi.h"
#include "ERRTelescopeCsIDigi.h"


class ERRTelescopeRawConverter : public FairTask 
{
    public:
        /** Default constructor **/
        ERRTelescopeRawConverter();

        /** Constructor
        ** verbose: 1 - only standard log print, 2 - print digi information
        **/
        ERRTelescopeRawConverter(Int_t verbose);

        /** Destructor **/
        ~ERRTelescopeRawConverter();

        /** Virtual method Init **/
        virtual InitStatus Init();

        /** Virtual method Converter **/
        virtual void Exec(Option_t* opt);

        /** Virtual method Finish **/
        virtual void Finish();

        /** Virtual method Reset **/
        virtual void Reset();

        void GetParameters();

        Float_t parameters_Si[160][4];
        Float_t parameters_CsI[32][5];

    protected:
        //Input arrays
        TClonesArray *fRTelescope1Si1DigiS;
        TClonesArray *fRTelescope1Si1DigiR;        
        TClonesArray *fRTelescope1Si2DigiS;
        TClonesArray *fRTelescope1CsIDigi;

        TClonesArray *fRTelescope2Si1DigiS;
        TClonesArray *fRTelescope2Si1DigiR;        
        TClonesArray *fRTelescope2Si2DigiS;
        TClonesArray *fRTelescope2CsIDigi;

        //Output array
        TClonesArray *fAculRaw;

    private:
        virtual void SetParContainers();
        ClassDef(ERRTelescopeRawConverter,1)
};

#endif