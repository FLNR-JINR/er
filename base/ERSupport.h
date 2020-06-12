  

#ifndef ERSupport_H
#define ERSupport_H

#include <map>

#include "TMatrixD.h"

class G4ParticleDefinition;
class G4Material;

#define TempNorm 293.16 /* Normal temperature (K) */
#define AMU 931.494028 /* atomic mass unit (MeV) */

TMatrixD* ReadCalFile(TString fileName);
Int_t GetChannelNumber(const Int_t rawChannel, const std::map<Int_t, Int_t>* channelsMapping);
double EiEo(double tableER[][105],double Tp,double Rp);
int ReadRint(char* Fname,double Ranges[][105]);
int intrp4(double* x,double* y, double* c);

Double_t CalcElossIntegralVolStep(Double_t T, const G4ParticleDefinition& ion, 
                                   const G4Material& mat, const Double_t range, 
                                   const Double_t intStep=1e-4);

#endif
