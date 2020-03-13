#ifndef ERDigiCleaner_H
#define ERDigiCleaner_H

#include <list>
#include <map>
#include <unordered_map>
#include <string>

#include "TClonesArray.h"
#include "TString.h"
#include "TMatrixD.h"
#include "TCutG.h"

#include "ERTask.h"

class ERDigiCleaner : public ERTask {

  public:

    struct RecalibrationTask {
        TString fDetectorName;
        TString fStationName;
        TMatrixD* fPreviousTimeCalibration = nullptr;
        TMatrixD* fTimeCalibration = nullptr;
        TMatrixD* fPreviousAmpCalibration = nullptr;
        TMatrixD* fAmpCalibration = nullptr;
        RecalibrationTask(const TString& detectorName, const TString& stationName, 
                          TMatrixD* previousTimeCalibration, TMatrixD* timeCalibration,
                          TMatrixD* previousAmpCalibration, TMatrixD* ampCalibration)
                          : fDetectorName(detectorName), fStationName(stationName),
                            fPreviousAmpCalibration(previousAmpCalibration),
                            fAmpCalibration(ampCalibration),
                            fPreviousTimeCalibration(previousTimeCalibration),
                            fTimeCalibration(timeCalibration) {}
        RecalibrationTask() = default;
    };

    struct StationCuts {
        TString fDetectorName;
        TString fStationName;
        std::map<Int_t, TCutG*> fChannelCuts;
        StationCuts(const TString& detectorName, const TString& stationName,
                    const std::map<Int_t, TCutG*>& channelCuts)
                    : fDetectorName(detectorName), fStationName(stationName),
                      fChannelCuts(channelCuts) {}
        StationCuts() = default;
    };

  /** Default constructor **/
  ERDigiCleaner();

  /** Destructor **/
  ~ERDigiCleaner() = default;

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Reset **/
  virtual void Reset();

  void Recalibrate(const TString& detectorName, const TString& stationName,
                   const TString& previousTimeCalFile, const TString& timeCalFile,
                   const TString& previousAmpCalFile, const TString& ampCalFile);
  void SetLonelyMWPCClusterCondition() { fLonelyMWPCClusterCondition = true; }
  void SetStationMultiplicity(const TString& detectorName, const TString stationName, Int_t multiplicity) { 
    fStationMultiplicities[std::make_pair(detectorName, stationName)] = multiplicity;
  }
  void SetChannelCuts(const TString& detectorName, const TString& stationName,
                      const std::map<Int_t, TCutG*>& channelCuts);

  protected:

    bool AreFewClustersInMWPC();
    void Recalibration();
    void ApplyChannelCuts();
    void ApplyStationMultiplicities();
    std::pair<std::string, TClonesArray*> GetBranchNameAndDigis(
        const TString& detectorName, const TString& stationName);

    std::list<RecalibrationTask> fStationsRecalibrations;
    std::list<StationCuts> fStationsCuts;
    std::map<std::pair<TString, TString>, Int_t> fStationsMultiplicities;
    bool fLonelyMWPCClusterCondition = false;

    std::unordered_map<std::string, TClonesArray*> fInputDigis;

  private:
    ClassDef(ERDigiCleaner,1)
};

#endif
