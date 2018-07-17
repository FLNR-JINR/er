#ifndef ERN15B11Digitizer_H
#define ERN15B11Digitizer_H

#include "FairTask.h" // mother class

#include <map>

class TClonesArray;

class ERN15B11Digitizer : public FairTask
{
public:
    /** Default constructor **/
    ERN15B11Digitizer();

    /** Destructor **/
    virtual ~ERN15B11Digitizer();

    /** Virtual method Init **/
    virtual InitStatus Init();

    virtual void Exec(Option_t* option = "");

    virtual void FinishEvent();

protected:

    // Input
    std::map<TString,TClonesArray*> fPoints;

    // Output
    std::map<TString,TClonesArray*> fDigis;

    //TClonesArray* fPoints;
    //TClonesArray* fDigis;

private:
    /** Write data to output file
        clref - output branch adress for current detector
        Edep - energy loss for current detector
        Time - current time interval
    **/
    void AddOutputDigi(TClonesArray& clref, Double_t Edep = 0., Double_t Time = 0.);

    ClassDef(ERN15B11Digitizer, 1);
};

#endif // ERN15B11Digitizer_H
