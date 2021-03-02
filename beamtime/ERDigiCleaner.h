/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
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
#include "ERBeamTimeEventHeader.h"
#include "ERBeamTimeCommon.h"

class ERDigiCleaner : public ERTask {
  using MultiplicityRange = std::pair<Int_t, Int_t>;

  public:

    struct RecalibrationTask {
        TString fDetectorName;
        TString fStationName;
        TMatrixD* fPreviousTimeCalibration = nullptr;
        TMatrixD* fTimeCalibration = nullptr;
        TMatrixD* fPreviousAmpCalibration = nullptr;
        TMatrixD* fAmpCalibration = nullptr;
        TMatrixD* fPreviousTACCalibration = nullptr;
        TMatrixD* fTACCalibration = nullptr;
        ChannelMapping* fSim2RawChannelMapping = nullptr;
        RecalibrationTask(const TString& detectorName, const TString& stationName, 
                          TMatrixD* previousTimeCalibration, TMatrixD* timeCalibration,
                          TMatrixD* previousAmpCalibration, TMatrixD* ampCalibration,
                          TMatrixD* previousTACCalibration = nullptr, TMatrixD* TACCalibration = nullptr,
                          ChannelMapping* raw2SimChannelMapping = nullptr);
        RecalibrationTask() = default;
    };

    struct StationCuts {
        TString fDetectorName;
        TString fStationName;
        std::map<Int_t, TCutG*> fChannelGCuts;
        std::map<Int_t, Double_t> fChannelMinAmp;
        std::map<Int_t, Double_t> fChannelMaxAmp;
        std::map<Int_t, Double_t> fChannelMinTime;
        std::map<Int_t, Double_t> fChannelMaxTime;
        ChannelMapping* fSim2RawChannelMapping = nullptr;
        StationCuts(const TString& detectorName, const TString& stationName,
                    const std::map<Int_t, TCutG*>& channelGCuts,
                    const std::map<Int_t, Double_t>& channelMinAmp,
                    const std::map<Int_t, Double_t>& channelMaxAmp,
                    const std::map<Int_t, Double_t>& channelMinTime,
                    const std::map<Int_t, Double_t>& channelMaxTime,
                    const ChannelMapping* raw2SimChannelMapping = nullptr);
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
                   const TString& previousAmpCalFile, const TString& ampCalFile,
                   ChannelMapping* raw2SimChannelMapping = nullptr);
  void RecalibrateWithTAC(const TString& detectorName, const TString& stationName,
                          const TString& previousTimeCalFile, const TString& timeCalFile,
                          const TString& previousAmpCalFile, const TString& ampCalFile,
                          const TString& previousTACCalFile, const TString& TACCalFile,
                          ChannelMapping* raw2SimChannelMapping = nullptr);
  void SetLonelyMWPCClusterCondition() { fLonelyMWPCClusterCondition = true; }
  void SetStationMultiplicity(const TString& detectorName, const TString& stationName, Int_t minMultiplicity, Int_t maxMultiplicity) { 
    fStationsMultiplicities[std::make_pair(detectorName, stationName)] = std::make_pair(minMultiplicity, maxMultiplicity);
  }
  void SetChannelCuts(const TString& detectorName, const TString& stationName,
                      const std::map<Int_t, TCutG*>& GCuts, 
                      const std::map<Int_t, Double_t>& MinAmp,
                      const std::map<Int_t, Double_t>& MaxAmp,
                      const std::map<Int_t, Double_t>& MinTime,
                      const std::map<Int_t, Double_t>& MaxTime,
                      const ChannelMapping* raw2SimChannelMapping = nullptr);
  protected:
    bool AreFewClustersInMWPC();
    void Recalibration();
    void ApplyChannelCuts();
    void ApplyStationMultiplicities();
    void CopyEventHeader();
    std::pair<std::string, TClonesArray*> GetBranchNameAndDigis(
        const TString& detectorName, const TString& stationName);

    std::list<RecalibrationTask> fStationsRecalibrations;
    std::list<StationCuts> fStationsCuts;
    std::map<std::pair<TString, TString>, MultiplicityRange> fStationsMultiplicities;
    bool fLonelyMWPCClusterCondition = false;

    std::unordered_map<std::string, TClonesArray*> fInputDigis;
    ERBeamTimeEventHeader* fInputHeader = nullptr;

  private:
    ClassDef(ERDigiCleaner,1)
};

#endif
