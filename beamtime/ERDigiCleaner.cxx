#include "ERDigiCleaner.h"

#include <set>

#include "FairLogger.h"

#include "ERSupport.h"
#include "ERBeamDetMWPCDigi.h"
#include "ERBeamDetTOFDigi.h"
#include "ERQTelescopeSiDigi.h"

//--------------------------------------------------------------------------------------------------
ERDigiCleaner::ERDigiCleaner()
  : ERTask("ERDigiCleaner") {
    fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::Recalibrate(
        const TString& detectorName, const TString& stationName,
        const TString& previousTimeCalFile, const TString& timeCalFile,
        const TString& previousAmpCalFile, const TString& ampCalFile) {
    fStationsRecalibrations.emplace_back(
        detectorName, stationName, 
        previousTimeCalFile != "" ? ReadCalFile(previousTimeCalFile) : nullptr,
        timeCalFile != "" ? ReadCalFile(timeCalFile) : nullptr, 
        previousAmpCalFile != "" ? ReadCalFile(previousAmpCalFile) : nullptr, 
        ampCalFile != "" ? ReadCalFile(ampCalFile) : nullptr);          
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::SetChannelCuts(
        const TString& detectorName, const TString& stationName,
        const std::map<Int_t, TCutG*>& channelCuts) {
    fStationsCuts.emplace_back(detectorName, stationName, channelCuts);
}
//--------------------------------------------------------------------------------------------------
InitStatus ERDigiCleaner::Init() {
    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");
    TList* allBranchNames = ioman->GetBranchNameList();
    TIter nextBranch(allBranchNames);
    while (TObjString* branchNameObj = (TObjString*)nextBranch()) {
        std::string branchName = std::string(branchNameObj->GetString().Data());
        fInputDigis[branchName] = static_cast<TClonesArray*>(ioman->GetObject(TString(branchName)));
        ioman->Register(TString(branchName), "Digis", fInputDigis[branchName], kTRUE);
    }
    return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::Exec(Option_t*) {
    if (fLonelyMWPCClusterCondition && AreFewClustersInMWPC())
        fRun->MarkFill(kFALSE);
    Recalibration();
    ApplyChannelCuts();
    ApplyStationMultiplicities();
}
//--------------------------------------------------------------------------------------------------
bool ERDigiCleaner::AreFewClustersInMWPC(){
    const auto isMWPCBranch = [](const TString& branchName) {
        const std::list<std::string> mwpcBranches = {
            "BeamDetMWPCDigiX1", "BeamDetMWPCDigiX2", "BeamDetMWPCDigiY1", "BeamDetMWPCDigiY2"};
        return std::find(mwpcBranches.begin(), mwpcBranches.end(), branchName) != mwpcBranches.end();
    };
    for (const auto& digiBranchNameAndCollection : fInputDigis) {
        const auto branchName = digiBranchNameAndCollection.first;
        if (!isMWPCBranch(branchName))
            continue;
        const auto& digis = digiBranchNameAndCollection.second;
        std::set<Int_t> wireNumbers; // ordered 
        for (Int_t iDigi(0); iDigi < digis->GetEntriesFast(); iDigi++) {
            const auto wireNumber = static_cast<ERBeamDetMWPCDigi*>(digis->At(iDigi))->GetWireNb();
            wireNumbers.insert(wireNumber);
        }
        const auto wireCount = wireNumbers.size();
        if (wireCount < 2)
            continue;
        const auto& firstNumber = *wireNumbers.begin();
        const auto& lastNumber = *wireNumbers.rbegin();
        if (lastNumber - firstNumber >= wireCount)
            return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::Recalibration() {
    for (auto& recalibrationTask : fStationsRecalibrations) {
        auto branchNameAndDigis = GetBranchNameAndDigis(recalibrationTask.fDetectorName, 
                                                        recalibrationTask.fStationName);
        if (!branchNameAndDigis.second) {
            LOG(FATAL) << "Digi branch for station " << recalibrationTask.fStationName 
                         << " of detector " << recalibrationTask.fDetectorName
                         << " not found." << FairLogger::endl;
        }
        const auto& branchName = branchNameAndDigis.first;
        if (!TString(branchName).Contains("ERQTelescopeSiDigi")) {
            LOG(FATAL) << "Recalibration is available only for silicon telescope station now." << FairLogger::endl;
        }
        auto& digis = branchNameAndDigis.second;
        auto& prevTimeCalibration = recalibrationTask.fPreviousTimeCalibration;
        auto& timeCalibration = recalibrationTask.fTimeCalibration;
        auto& prevAmpCalibration = recalibrationTask.fAmpCalibration;
        auto& ampCalibration = recalibrationTask.fAmpCalibration;
        for (Int_t iDigi(0); iDigi < digis->GetEntriesFast(); iDigi++) {
            // linear calibration: res = table[channel][0] + table[channel][1] * raw
            auto digi = static_cast<ERQTelescopeSiDigi*>(digis->At(iDigi));
            const auto stripNb = digi->GetStripNb();
            if (prevTimeCalibration && timeCalibration) {
                if (stripNb >= prevTimeCalibration->GetNrows() || stripNb >= timeCalibration->GetNrows()) {
                    LOG(FATAL) << "Channel " << stripNb << " not found time calibration tables of station " 
                        << recalibrationTask.fStationName << " of detector " 
                        << recalibrationTask.fDetectorName <<  FairLogger::endl;
                }
                const auto rawTime = (digi->GetTime() - (*prevTimeCalibration)[stripNb][0]) 
                    / (*prevTimeCalibration)[stripNb][1];
                digi->SetTime((*timeCalibration)[stripNb][0] + (*timeCalibration)[stripNb][1] * rawTime);
            }
            if (prevAmpCalibration && ampCalibration) {
                if (stripNb >= prevAmpCalibration->GetNrows() || stripNb >= ampCalibration->GetNrows()) {
                    LOG(FATAL) << "Channel " << stripNb << " not found amp calibration tables of station " 
                        << recalibrationTask.fStationName << " of detector " 
                        << recalibrationTask.fDetectorName <<  FairLogger::endl;
                }
                const auto rawEdep = (digi->GetEdep() - (*prevAmpCalibration)[stripNb][0]) 
                    / (*prevAmpCalibration)[stripNb][1];
                digi->SetEdep((*ampCalibration)[stripNb][0] + (*ampCalibration)[stripNb][1] * rawEdep);
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::ApplyChannelCuts() {
    if (fStationsCuts.empty())
        return;
    auto tofBranchAndDigi = GetBranchNameAndDigis("BeamDet", "ToFDigi2");
    if (!tofBranchAndDigi.second)
        LOG(FATAL) << "Digi branch for TOF2 station not found." << FairLogger::endl;
    const auto tofTime = static_cast<ERBeamDetTOFDigi*>(tofBranchAndDigi.second->At(0))->GetTime();
    for (const auto& stationCuts : fStationsCuts) {
        auto branchNameAndDigis = GetBranchNameAndDigis(stationCuts.fDetectorName, 
                                                        stationCuts.fStationName);
        auto& digis = branchNameAndDigis.second;
        if (!digis) {
            LOG(FATAL) << "Digi branch for station " << stationCuts.fStationName 
                         << " of detector " << stationCuts.fDetectorName
                         << " not found." << FairLogger::endl;
        }
        const auto branchName = branchNameAndDigis.first;
        if (!TString(branchName).Contains("ERQTelescopeSiDigi")) {
            LOG(FATAL) << "ApplyChannelCuts is available only for silicon telescope station now." << FairLogger::endl;
        }
        std::list<TObject*> digisToRemove;
        for (Int_t iDigi(0); iDigi < digis->GetEntriesFast(); iDigi++) {
            auto* digi = static_cast<ERQTelescopeSiDigi*>(digis->At(iDigi));
            const auto stripNb = digi->GetStripNb();
            const auto& channelsCuts = stationCuts.fChannelCuts;
            if (channelsCuts.find(stripNb) != channelsCuts.end()) {
                if (channelsCuts.at(stripNb)->IsInside(digi->GetTime() - tofTime, digi->GetEdep()))
                    continue;
                digisToRemove.push_back(digi);
            }
        }
        for (auto* digi : digisToRemove) {
            digis->Remove(digi);
        }
        digis->Compress();
    }
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::ApplyStationMultiplicities() {
    for (const auto detStationMultiplicity : fStationsMultiplicities) {
        const auto detectorAndStation = detStationMultiplicity.first;
        const auto detectorName = detectorAndStation.first;
        const auto stationName = detectorAndStation.second;
        const auto multiplicity = detStationMultiplicity.second;
        const auto branchNameAndDigis = GetBranchNameAndDigis(detectorName, stationName);
        const auto* digi = branchNameAndDigis.second;
        if (!digi) {
            LOG(FATAL) << "Digi branch for station " << detectorName 
                       << " of detector " << stationName << " not found." << FairLogger::endl;
        }
        if (digi->GetEntriesFast() > multiplicity) {
            fRun->MarkFill(kFALSE);
            return;
        }
    }
}
//--------------------------------------------------------------------------------------------------
std::pair<std::string, TClonesArray*> ERDigiCleaner::GetBranchNameAndDigis(
    const TString& detectorName, const TString& stationName) {
    for (const auto& digiBranchNameAndCollection : fInputDigis) {
        const auto branchName = digiBranchNameAndCollection.first;
        if (TString(branchName).Contains(detectorName) && TString(branchName).Contains(stationName))
            return digiBranchNameAndCollection;
    }
    return std::make_pair(std::string(), nullptr);
}
//-------------------------------------------------------------------------------------------------- 
void ERDigiCleaner::Reset() {
    for (const auto& digiBranchNameAndCollection : fInputDigis) {
        digiBranchNameAndCollection.second->Clear();
    }
}
//--------------------------------------------------------------------------------------------------