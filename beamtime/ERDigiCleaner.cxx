/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERDigiCleaner.h"

#include <set>

#include "FairLogger.h"

#include "ERSupport.h"
#include "ERDigi.h"
#include "ERBeamDetMWPCDigi.h"
#include "ERBeamDetToFDigi.h"

//--------------------------------------------------------------------------------------------------
ERDigiCleaner::ERDigiCleaner()
  : ERTask("ERDigiCleaner") {
    fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::Recalibrate(
        const TString& detectorName, const TString& stationName,
        const TString& previousTimeCalFile, const TString& timeCalFile,
        const TString& previousAmpCalFile, const TString& ampCalFile,
        std::map<Int_t, Int_t>* raw2SimChannelsMapping/* = nullptr*/) {
    fStationsRecalibrations.emplace_back(
        detectorName, stationName, 
        previousTimeCalFile != "" ? ReadCalFile(previousTimeCalFile) : nullptr,
        timeCalFile != "" ? ReadCalFile(timeCalFile) : nullptr, 
        previousAmpCalFile != "" ? ReadCalFile(previousAmpCalFile) : nullptr, 
        ampCalFile != "" ? ReadCalFile(ampCalFile) : nullptr,
        raw2SimChannelsMapping);          
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::SetChannelCuts(
        const TString& detectorName, const TString& stationName,
        const std::map<Int_t, TCutG*>& channelGCuts, const std::map<Int_t, Double_t>& channelMinAmp,
        const std::map<Int_t, Double_t>& channelMaxAmp, const std::map<Int_t, Double_t>& channelMinTime,
        const std::map<Int_t, Double_t>& channelMaxTime) {
    fStationsCuts.emplace_back(detectorName, stationName, channelGCuts, channelMinAmp, channelMaxAmp,
                               channelMinTime, channelMaxTime);
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
            const auto wireNumber = static_cast<ERBeamDetMWPCDigi*>(digis->At(iDigi))->Channel();
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
        auto& digis = branchNameAndDigis.second;
        auto& prevTimeCalibration = recalibrationTask.fPreviousTimeCalibration;
        auto& timeCalibration = recalibrationTask.fTimeCalibration;
        auto& prevAmpCalibration = recalibrationTask.fAmpCalibration;
        auto& ampCalibration = recalibrationTask.fAmpCalibration;
        for (Int_t iDigi(0); iDigi < digis->GetEntriesFast(); iDigi++) {
            // linear calibration: res = table[channel][0] + table[channel][1] * raw
            auto digi = dynamic_cast<ERDigi*>(digis->At(iDigi));
            if (!digi) {
                LOG(FATAL) << "Recalibration is not available for branch " << branchName << FairLogger::endl;
            }
            const auto channel = GetChannelNumber(digi->Channel(), recalibrationTask.fSim2RawChannelsMapping);
            if (prevTimeCalibration && timeCalibration) {
                if (channel >= prevTimeCalibration->GetNrows() || channel >= timeCalibration->GetNrows()) {
                    LOG(FATAL) << "Channel " << channel << " not found time calibration tables of station " 
                        << recalibrationTask.fStationName << " of detector " 
                        << recalibrationTask.fDetectorName <<  FairLogger::endl;
                }
                const auto rawTime = (digi->Time() - (*prevTimeCalibration)[channel][0]) 
                    / (*prevTimeCalibration)[channel][1];
                digi->SetTime((*timeCalibration)[channel][0] + (*timeCalibration)[channel][1] * rawTime);
            }
            if (prevAmpCalibration && ampCalibration) {
                if (channel >= prevAmpCalibration->GetNrows() || channel >= ampCalibration->GetNrows()) {
                    LOG(FATAL) << "Channel " << channel << " not found amp calibration tables of station " 
                        << recalibrationTask.fStationName << " of detector " 
                        << recalibrationTask.fDetectorName <<  FairLogger::endl;
                }
                const auto rawEdep = (digi->Edep() - (*prevAmpCalibration)[channel][0]) 
                    / (*prevAmpCalibration)[channel][1];
                digi->SetEdep((*ampCalibration)[channel][0] + (*ampCalibration)[channel][1] * rawEdep);
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
    const auto tofTime = static_cast<ERBeamDetTOFDigi*>(tofBranchAndDigi.second->At(0))->Time();
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
        std::list<TObject*> digisToRemove;
        for (Int_t iDigi(0); iDigi < digis->GetEntriesFast(); iDigi++) {
            auto* digi = dynamic_cast<ERDigi*>(digis->At(iDigi));
            if (!digi) {
                LOG(FATAL) << "Recalibration is not available for branch " << branchName << FairLogger::endl;
            }
            const auto channel = digi->Channel();
            const auto time = digi->Time() - tofTime;
            const auto edep = digi->Edep();
            const auto& channelsGCuts = stationCuts.fChannelGCuts;
            if (channelsGCuts.find(channel) != channelsGCuts.end()) {
                if (!channelsGCuts.at(channel)->IsInside(time, edep))
                    digisToRemove.push_back(digi);
            }
            const auto& channelsMinAmp = stationCuts.fChannelMinAmp;
            if (channelsMinAmp.find(channel) != channelsMinAmp.end()) {
                if (channelsMinAmp.at(channel) > edep)
                    digisToRemove.push_back(digi);
            }            
            const auto& channelsMaxAmp = stationCuts.fChannelMaxAmp;
            if (channelsMaxAmp.find(channel) != channelsMaxAmp.end()) {
                if (channelsMaxAmp.at(channel) < edep)
                    digisToRemove.push_back(digi);
            }
            const auto& channelsMinTime = stationCuts.fChannelMinTime;
            if (channelsMinTime.find(channel) != channelsMinTime.end()) {
                if (channelsMinTime.at(channel) > time)
                    digisToRemove.push_back(digi);
            }
            const auto& channelsMaxTime = stationCuts.fChannelMaxTime;
            if (channelsMaxTime.find(channel) != channelsMaxTime.end()) {
                if (channelsMaxTime.at(channel) < time)
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
ERDigiCleaner::RecalibrationTask::RecalibrationTask(const TString& detectorName, const TString& stationName, 
                                    TMatrixD* previousTimeCalibration, TMatrixD* timeCalibration,
                                    TMatrixD* previousAmpCalibration, TMatrixD* ampCalibration,
                                    std::map<Int_t, Int_t>* raw2SimChannelsMapping/* = nullptr*/)
: fDetectorName(detectorName), fStationName(stationName),
fPreviousAmpCalibration(previousAmpCalibration),
fAmpCalibration(ampCalibration),
fPreviousTimeCalibration(previousTimeCalibration),
fTimeCalibration(timeCalibration) {
    if (raw2SimChannelsMapping) {
        fSim2RawChannelsMapping = new std::map<Int_t, Int_t>();
        for (const auto raw2sim : *raw2SimChannelsMapping) {
            (*fSim2RawChannelsMapping)[raw2sim.second] = raw2sim.first;
        }
    }
}