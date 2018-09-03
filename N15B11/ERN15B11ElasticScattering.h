#ifndef ERN15B11ElasticScattering_H
#define ERN15B11ElasticScattering_H

#include <ERDecay.h> // mother class

// ROOT
#include <TString.h>

class TF1;
class TParticlePDG;

class ERN15B11ElasticScattering : public ERDecay
{
public:
    ERN15B11ElasticScattering(TString name);
    ~ERN15B11ElasticScattering();

    void SetTargetIon(Int_t A, Int_t Z, Int_t Q);
    void SetThetaCDF(TString fileName) { fThetaFileName = fileName; }
    void SetPhiRange(Double_t phi1, Double_t phi2) { fPhi1 = phi1; fPhi2 = phi2; }

    void SetDetAngle(Double_t angle) { fDetPos = angle; }
    void SetIonMass(Double_t mass) { fIonMass = mass; }
    void SetTargetMass(Double_t mass) { fTargetMass = mass; }

    Double_t GetIonMass() const { return fIonMass; }
    Double_t GetTargetMass() const { return fTargetMass; }
    Double_t GetdPhi() const {return fPhi2 - fPhi1; }

    Int_t GetInteractNumInTarget() const { return fInteractNumInTarget; }

public:
    Bool_t Init();
    Bool_t Stepping();

protected:
    TString         fThetaFileName;
    Double_t        fTheta1,fTheta2;
    Double_t        fCDFmin,fCDFmax;
    Double_t        fPhi1, fPhi2;

    TString         fTargetIonName;
    TParticlePDG*   fTargetIonPDG;

    TF1*            fThetaCDF;
    TF1*            fThetaInvCDF;

protected:
    Double_t        fTheta1B11;
    Double_t        fTheta2B11;
    Double_t        fCDFminB11;
    Double_t        fCDFmaxB11;
    Double_t        fDetPos;
    Double_t        fIonMass;
    Double_t        fTargetMass;

    Int_t           fInteractNumInTarget;

    Double_t ThetaGen();
    Double_t PhiGen(Double_t theta);
    void RangeCalculate(Double_t iM, Double_t tM);

    ClassDef(ERN15B11ElasticScattering, 1);
};

#endif // ERN15B11ElasticScattering_H
