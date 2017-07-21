// -----                   ERIonMixGenerator header file                 -----
// -----          		   Created   by            -----
// -------------------------------------------------------------------------

#ifndef ERIonMixGenerator_H
#define ERIonMixGenerator_H

#include "FairGenerator.h"              // for FairGenerator
#include "ERIonGenerator.h"

#include "Rtypes.h"                     // for Double_t, Int_t, etc
#include <map>
#include <algorithm>

class FairIon;
class FairPrimaryGenerator;

class ERIonMixGenerator : public ERIonGenerator
{

  public:

    /** Default constructor **/
    ERIonMixGenerator();


    /** Constructor with ion name
      ** For the generation of ions with pre-defined FairIon
     ** By default, the  excitation energy is zero. This can be changed with the
     ** respective modifiers.
     **@param ionName  Ion name
     **@param mult      Number of ions per event
     **@param px,py,pz  Momentum components [GeV] per nucleon!
     **@param vx,vy,vz  Vertex coordinates [cm]
     **/
    ERIonMixGenerator(const Char_t* ionName, Int_t mult);


    /** Default constructor
     ** For the generation of ions with atomic number z and mass number a.
     ** By default, the mass equals a times the proton mass and the
     ** excitation energy is zero. This can be changed with the
     ** respective modifiers.
     **@param z         Atomic number
     **@param a         Atomic mass
     **@param q         Electric charge [e]
     **@param mult      Number of ions per event
     **@param px,py,pz  Momentum components [GeV] per nucleon!
     **@param vx,vy,vz  Vertex coordinates [cm]
     **/
    ERIonMixGenerator(TString name, Int_t z, Int_t a, Int_t q, Int_t mult);

    /** Destructor **/
    virtual ~ERIonMixGenerator();

    Bool_t ReadEvent(FairPrimaryGenerator* primGen);
    void AddBackgroundIon(TString name, Int_t z, Int_t a, Int_t q, Double_t probability);

  private:

    std::map<Double_t, TString>  fBgIons;   // Background ions names with emegence probability
    
    ERIonMixGenerator(const ERIonMixGenerator&);
    ERIonMixGenerator& operator=(const ERIonMixGenerator&);

    ClassDef(ERIonMixGenerator,1)
};


#endif

