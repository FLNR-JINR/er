#ifndef ERElasticScattering_H
#define ERElasticScattering_H

#include <ERDecay.h> // mother class

// ROOT
#include <TString.h>

class TF1;
class TParticlePDG;

class ERElasticScattering : public ERDecay
{
public:
    ERElasticScattering(TString name);
    ~ERElasticScattering();

    void SetTargetIon(Int_t A, Int_t Z, Int_t Q);
    void SetThetaCDF(TString fileName) { fThetaFileName = fileName; }
    void SetThetaRange(Double_t th1, Double_t th2) { fTheta1 = th1; fTheta2 = th2; }
    void SetPhiRange(Double_t phi1, Double_t phi2) { fPhi1 = phi1; fPhi2 = phi2; }

    void SetDetAngle(Double_t angle)  { fDetPos = angle; }
    void SetIonMass(Double_t mass)    { fIonMass = mass; }
    void SetTargetMass(Double_t mass) { fTargetMass = mass; }

    Double_t GetIonMass()          const { return fIonMass; }
    Double_t GetTargetMass()       const { return fTargetMass; }
    Double_t GetdPhi()             const { return (fPhi2 - fPhi1); }
    Double_t GetCDFRangesSumm()    const { return fCDFRangesSumm; }

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
    Double_t        fThetaTargetIon1;
    Double_t        fThetaTargetIon2;
    Double_t        fCDFminTargetIon;
    Double_t        fCDFmaxTargetIon;
    Double_t        fDetPos;              // Detector position in Lab
    Double_t        fIonMass;
    Double_t        fTargetMass;
    Double_t        fCDFRangesSumm;       // The CDF ranges summ of the primary ion and target ion

    Int_t           fInteractNumInTarget; // Interaction counter for target

    Bool_t ionMassTrueOrFalseTester;      // It's to identify a difference between the PDG and Monte-Carlo ion mass

    Double_t ThetaGen();
    // The method is to calculate range for It's drawing of the ion scattering angle
    void RangesCalculate(Double_t iM, Double_t tM);

    ClassDef(ERElasticScattering, 1);
};

#endif // ERElasticScattering_H
