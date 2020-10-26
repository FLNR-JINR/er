  

#ifndef ERSupport_H
#define ERSupport_H

#include <map>
#include <limits>

#include "TMatrixD.h"
#include "TString.h"

#include "FairLogger.h"

using ERChannel = unsigned short;
using ChannelsMapping = std::map<ERChannel, ERChannel>;
namespace consts {
  const unsigned short undefined_channel = std::numeric_limits<ERChannel>::max();
}
enum ERDataObjectType {Point, Digi, Track, Particle};
inline TString ERDataObjectTypeStr(ERDataObjectType object) {
    switch(object) {
        case Point : return "Point";
        case Digi  : return "Digi";
        case Track : return "Track";
        case Particle : return "Particle";
        default : LOG(FATAL) << "Unknown ERDataObject type\n";
    }
    return "";
}
enum ChannelSide {First, Second, None};
enum SensetiveType {Si, CsI, LaBr};
enum OrientationAroundZ {X, Y, Default};
inline TString SensetiveTypeStr(SensetiveType type) {
  switch(type) {
    case Si : return "Si";
    case CsI : return "CsI";
    case LaBr : return "LaBr";
    default : LOG(FATAL) << "Unknown telescope sensetive type\n"; 
  }
  return "";
}
inline TString ChannelSideStr(ChannelSide side) {
  switch(side) {
    case First : return "1";
    case Second : return "2";
    case None : LOG(FATAL) 
        << "String representation for None channel side is not available\n"; 
    default : LOG(FATAL) << "Unknown telescope channel side\n"; 
  }
  return "";
}
inline TString OrientationAroundZStr(OrientationAroundZ type) {
  switch(type) {
    case X : return "X";
    case Y : return "Y";
    case Default : LOG(FATAL) 
        << "String representation for Default orientation around Z is not available\n"; 
    default : LOG(FATAL) << "Unknown telescope orientation around z\n"; 
  }
  return "";
}

class G4ParticleDefinition;
class G4Material;

#define TempNorm 293.16 /* Normal temperature (K) */
#define AMU 931.494028 /* atomic mass unit (MeV) */

TMatrixD* ReadCalFile(TString fileName);
ERChannel GetChannelNumber(const ERChannel rawChannel, const ChannelsMapping* channelsMapping);
double EiEo(double tableER[][105],double Tp,double Rp);
int ReadRint(char* Fname,double Ranges[][105]);
int intrp4(double* x,double* y, double* c);

Double_t CalcElossIntegralVolStep(Double_t T, const G4ParticleDefinition& ion, 
                                   const G4Material& mat, const Double_t range, 
                                   const Double_t intStep=1e-4);

#endif
