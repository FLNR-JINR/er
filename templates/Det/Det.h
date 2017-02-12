
#ifndef Det_H
#define Det_H


//#include "DetPoint.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"

#include "FairDetector.h"
#include "DetPoint.h"


class TClonesArray;

class Det : public FairDetector
{
  
public:
  Det();

  Det(const char* name, Bool_t active, Int_t verbose);

  virtual ~Det();

  virtual Bool_t ProcessHits(FairVolume* vol = 0);
  
  virtual void BeginEvent();

  virtual void EndOfEvent();

  virtual void Register();

  virtual TClonesArray* GetCollection(Int_t iColl) const;

  virtual void Print(Option_t *option="") const;

  virtual void Reset();
  
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2,
			  Int_t offset);

  virtual void ConstructGeometry();
  
  virtual void Initialize();

  virtual Bool_t CheckIfSensitive(std::string name);
  
private:
  TClonesArray*  fPoints;
  DetPoint* AddPoint(Int_t eventID, Int_t trackID,
        Int_t mot0trackID,
        Double_t mass,
        TVector3 posIn,
        TVector3 pos_out, TVector3 momIn,
        TVector3 momOut, Double_t time,
        Double_t length, Double_t eLoss);
  ClassDef(Det,1);
};

#endif 
