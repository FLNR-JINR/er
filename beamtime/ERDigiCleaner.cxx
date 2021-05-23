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
#include "FairRun.h"

#include "ERSupport.h"
#include "ERRunAna.h"
#include "ERDigi.h"
#include "ERNDDigi.h"

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
        ChannelMapping* raw2SimChannelsMapping/* = nullptr*/) {
    fStationsRecalibrations.emplace_back(
        detectorName, stationName, 
        previousTimeCalFile != "" ? ReadCalFile(previousTimeCalFile) : nullptr,
        timeCalFile != "" ? ReadCalFile(timeCalFile) : nullptr, 
        previousAmpCalFile != "" ? ReadCalFile(previousAmpCalFile) : nullptr, 
        ampCalFile != "" ? ReadCalFile(ampCalFile) : nullptr,
        nullptr, nullptr,
        raw2SimChannelsMapping);          
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::RecalibrateWithTAC(
        const TString& detectorName, const TString& stationName,
        const TString& previousTimeCalFile, const TString& timeCalFile,
        const TString& previousAmpCalFile, const TString& ampCalFile,
        const TString& previousTACCalFile, const TString& TACCalFile,
        ChannelMapping* raw2SimChannelsMapping/* = nullptr*/) {
    fStationsRecalibrations.emplace_back(
        detectorName, stationName, 
        previousTimeCalFile != "" ? ReadCalFile(previousTimeCalFile) : nullptr,
        timeCalFile != "" ? ReadCalFile(timeCalFile) : nullptr, 
        previousAmpCalFile != "" ? ReadCalFile(previousAmpCalFile) : nullptr, 
        ampCalFile != "" ? ReadCalFile(ampCalFile) : nullptr,
        previousTACCalFile != "" ? ReadCalFile(previousTACCalFile) : nullptr,
        TACCalFile != "" ? ReadCalFile(TACCalFile) : nullptr,
        raw2SimChannelsMapping);
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::SetChannelCuts(
        const TString& detectorName, const TString& stationName,
        const std::map<ERChannel, TCutG*>& channelGCuts, const std::map<ERChannel, Double_t>& channelMinAmp,
        const std::map<ERChannel, Double_t>& channelMaxAmp, const std::map<ERChannel, Double_t>& channelMinTime,
        const std::map<ERChannel, Double_t>& channelMaxTime, const ChannelMapping* raw2SimChannelsMapping /*= nullptr*/) {
    fStationsCuts.emplace_back(detectorName, stationName, channelGCuts, channelMinAmp, channelMaxAmp,
                               channelMinTime, channelMaxTime, raw2SimChannelsMapping);
}
//--------------------------------------------------------------------------------------------------
InitStatus ERDigiCleaner::Init() {
    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");
    if (dynamic_cast<ERBeamTimeEventHeader*>(ioman->GetObject("EventHeader."))) {
        fInputHeader = new ERBeamTimeEventHeader();
        ioman->GetInTree()->SetBranchAddress("EventHeader.",&fInputHeader);
    }
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
    if (fInputHeader)
        CopyEventHeader();
    if (fLonelyMWPCClusterCondition && AreFewClustersInMWPC()) {
        dynamic_cast<ERRunAna*>(fRun)->MarkFill(kFALSE);
        Reset();
        return;
    }
    Recalibration();
    ApplyChannelCuts();
    ApplyStationMultiplicities();
}
//--------------------------------------------------------------------------------------------------
void ERDigiCleaner::CopyEventHeader() {
    FairRun* run = FairRun::Instance();
    ERBeamTimeEventHeader* header = dynamic_cast<ERBeamTimeEventHeader*>(run->GetEventHeader());
    if (!header)
        return;
    header->SetTrigger(fInputHeader->GetTrigger());
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
            const auto wireNumber = static_cast<ERDigi*>(digis->At(iDigi))->Channel();
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
            LOG(FATAL) << "[Recalibration] Digi branch for station " << recalibrationTask.fStationName 
                         << " of detector " << recalibrationTask.fDetectorName
                         << " not found." << FairLogger::endl;
        }
        LOG(DEBUG) << "[Recalibration] Station " << recalibrationTask.fStationName << " of detector "
                   << recalibrationTask.fDetectorName << FairLogger::endl;
        const auto& branchName = branchNameAndDigis.first;
        auto& digis = branchNameAndDigis.second;
        auto& prevTimeCalibration = recalibrationTask.fPreviousTimeCalibration;
        auto& timeCalibration = recalibrationTask.fTimeCalibration;
        auto& prevAmpCalibration = recalibrationTask.fPreviousAmpCalibration;
        auto& ampCalibration = recalibrationTask.fAmpCalibration;
        auto& prevTACCalibration = recalibrationTask.fPreviousTACCalibration;
        auto& TACCalibration = recalibrationTask.fTACCalibration;
        for (Int_t iDigi(0); iDigi < digis->GetEntriesFast(); iDigi++) {
            // linear calibration: res = table[channel][0] + table[channel][1] * raw
            auto digi = dynamic_cast<ERDigi*>(digis->At(iDigi));
            if (!digi) {
                LOG(FATAL) << "[Recalibration] Recalibration is not available for branch " << branchName << FairLogger::endl;
            }
            const auto channel = GetChannelNumber(digi->Channel(), recalibrationTask.fSim2RawChannelMapping);
            if (prevTimeCalibration && timeCalibration) {
                if (channel >= prevTimeCalibration->GetNrows() || channel >= timeCalibration->GetNrows()) {
                    LOG(FATAL) << "[Recalibration] Channel " << channel << " not found time in calibration tables of station " 
                        << recalibrationTask.fStationName << " of detector " 
                        << recalibrationTask.fDetectorName <<  FairLogger::endl;
                }
                const auto rawTime = (digi->Time() - (*prevTimeCalibration)[channel][0]) 
                    / (*prevTimeCalibration)[channel][1];
                const auto newTime = (*timeCalibration)[channel][0] + (*timeCalibration)[channel][1] * rawTime;
                LOG(DEBUG) << "[Recalibration] Time: channel = " << channel <<", previous a = " << (*prevTimeCalibration)[channel][0]
                           << ", previous b = " << (*prevTimeCalibration)[channel][1] << " new a = " <<  (*timeCalibration)[channel][0]
                           << ", new b = " << (*timeCalibration)[channel][1] << ", previous = " << digi->Time() 
                           << ", raw = " << rawTime << ", new time =  " << newTime << FairLogger::endl;
                digi->SetTime(newTime);
            }
            if (prevAmpCalibration && ampCalibration) {
                if (channel >= prevAmpCalibration->GetNrows() || channel >= ampCalibration->GetNrows()) {
                    LOG(FATAL) << "[Recalibration] Channel " << channel << " not found in amp calibration tables of station " 
                        << recalibrationTask.fStationName << " of detector " 
                        << recalibrationTask.fDetectorName <<  FairLogger::endl;
                }
                const auto rawEdep = (digi->Edep() - (*prevAmpCalibration)[channel][0]) 
                    / (*prevAmpCalibration)[channel][1];
                const auto newEdep = (*ampCalibration)[channel][0] + (*ampCalibration)[channel][1] * rawEdep;
                LOG(DEBUG) << "[Recalibration] Edep: channel = " << channel << ", previous a = " << (*prevAmpCalibration)[channel][0]
                           << ", previous b = " << (*prevAmpCalibration)[channel][1] << " new a = " <<  (*ampCalibration)[channel][0]
                           << ", new b = " << (*ampCalibration)[channel][1] <<", previous = " << digi->Edep() 
                           << ", raw = " << rawEdep << ", new =  " << newEdep << FairLogger::endl;
                digi->SetEdep(newEdep);
            }
            if (prevTACCalibration && TACCalibration) {
                if (channel >= prevTACCalibration->GetNrows() || channel >= TACCalibration->GetNrows()) {
                    LOG(FATAL) << "[Recalibration] Channel " << channel << " not found in TAC calibration tables of station " 
                        << recalibrationTask.fStationName << " of detector " 
                        << recalibrationTask.fDetectorName <<  FairLogger::endl;
                }
                auto* NDDigi = dynamic_cast<ERNDDigi*>(digi);
                if (!NDDigi)
                    LOG(FATAL) << "[Recalibration] You are trying to recalibrate TAC, but digi is not ERNDDigi" << FairLogger::endl;
                const auto rawTAC = (NDDigi->TAC() - (*prevTACCalibration)[channel][0]) 
                    / (*prevTACCalibration)[channel][1];
                const auto newTAC = (*TACCalibration)[channel][0] + (*TACCalibration)[channel][1] * rawTAC;
                LOG(DEBUG) << "[Recalibration] TAC: channel = " << channel << ", previous a = " << (*prevTACCalibration)[channel][0]
                           << ", previous b = " << (*prevTACCalibration)[channel][1] << " new a = " <<  (*TACCalibration)[channel][0]
                           << ", new b = " << (*TACCalibration)[channel][1] <<", previous = " << NDDigi->TAC() 
                           << ", raw = " << rawTAC << ", new =  " << newTAC << FairLogger::endl;
                NDDigi->SetTAC(newTAC);
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
    const auto tofTime = static_cast<ERDigi*>(tofBranchAndDigi.second->At(0))->Time();
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
            const auto channel = GetChannelNumber(digi->Channel(), stationCuts.fSim2RawChannelMapping);
            const auto time = digi->Time() - tofTime;
            const auto edep = digi->Edep();
            const auto& channelsGCuts = stationCuts.fChannelGCuts;
            if (channelsGCuts.find(channel) != channelsGCuts.end()) {
                if (!channelsGCuts.at(channel)->IsInside(time, edep)) {
                    digisToRemove.push_back(digi);
                    continue;
                }
            }
            const auto& channelsMinAmp = stationCuts.fChannelMinAmp;
            if (channelsMinAmp.find(channel) != channelsMinAmp.end()) {
                if (channelsMinAmp.at(channel) > edep) {
                    digisToRemove.push_back(digi);
                    continue;
                }
            }            
            const auto& channelsMaxAmp = stationCuts.fChannelMaxAmp;
            if (channelsMaxAmp.find(channel) != channelsMaxAmp.end()) {
                if (channelsMaxAmp.at(channel) < edep) {
                    digisToRemove.push_back(digi);
                    continue;
                }
            }
            const auto& channelsMinTime = stationCuts.fChannelMinTime;
            if (channelsMinTime.find(channel) != channelsMinTime.end()) {
                if (channelsMinTime.at(channel) > time) {
                    digisToRemove.push_back(digi);
                    continue;
                }
            }
            const auto& channelsMaxTime = stationCuts.fChannelMaxTime;
            if (channelsMaxTime.find(channel) != channelsMaxTime.end()) {
                if (channelsMaxTime.at(channel) < time) {
                    digisToRemove.push_back(digi);
                    continue;
                }
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
        const auto multiplicityRange = detStationMultiplicity.second;
        auto branchNameAndDigis = GetBranchNameAndDigis(detectorName, stationName);
        auto* digi = branchNameAndDigis.second;
        if (!digi) {
            LOG(FATAL) << "Digi branch for station " << detectorName 
                       << " of detector " << stationName << " not found." << FairLogger::endl;
        }
        if (digi->GetEntriesFast() < multiplicityRange.first || digi->GetEntriesFast() > multiplicityRange.second) {
            digi->Clear();
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
                                    TMatrixD* previousTACCalibration /*= nullptr*/, TMatrixD* TACCalibration/*= nullptr*/,
                                    ChannelMapping* raw2SimChannelsMapping/* = nullptr*/)
: fDetectorName(detectorName), fStationName(stationName),
fPreviousAmpCalibration(previousAmpCalibration),
fAmpCalibration(ampCalibration),
fPreviousTimeCalibration(previousTimeCalibration),
fTimeCalibration(timeCalibration),
fPreviousTACCalibration(previousTACCalibration),
fTACCalibration(TACCalibration) {
    if (raw2SimChannelsMapping) {
        fSim2RawChannelMapping = new ChannelMapping();
        for (const auto raw2sim : *raw2SimChannelsMapping) {
            (*fSim2RawChannelMapping)[raw2sim.second] = raw2sim.first;
        }
    }
}
//--------------------------------------------------------------------------------------------------
ERDigiCleaner::StationCuts::StationCuts(const TString& detectorName, const TString& stationName,
                                        const std::map<ERChannel, TCutG*>& channelGCuts,
                                        const std::map<ERChannel, Double_t>& channelMinAmp,
                                        const std::map<ERChannel, Double_t>& channelMaxAmp,
                                        const std::map<ERChannel, Double_t>& channelMinTime,
                                        const std::map<ERChannel, Double_t>& channelMaxTime,
                                        const ChannelMapping* raw2SimChannelsMapping/* = nullptr*/)
    : fDetectorName(detectorName), fStationName(stationName),
    fChannelGCuts(channelGCuts), fChannelMinAmp(channelMinAmp),
    fChannelMaxAmp(channelMaxAmp), fChannelMinTime(channelMinTime),
    fChannelMaxTime(channelMaxTime) 
{
    if (raw2SimChannelsMapping) {
        fSim2RawChannelMapping = new ChannelMapping();
        for (const auto raw2sim : *raw2SimChannelsMapping) {
            (*fSim2RawChannelMapping)[raw2sim.second] = raw2sim.first;
        }
    }
}
//--------------------------------------------------------------------------------------------------