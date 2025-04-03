/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERTelescopeParticle_H
#define ERTelescopeParticle_H

#include "TNamed.h"
#include "TLorentzVector.h"

#include "ERSupport.h"

/** @class ERTelescopeParticle
 ** @brief 
 ** @author V.Schetinin <schetinin@jinr.ru>
 ** @version 1.0
**/

class ERTelescopeParticle : public TNamed {
public:
  ERTelescopeParticle() = default;
  /** @brief Constructor 
   ** @param deadEloss - eloss in passive detector volumes;
   ** @param lvInteraction - Lorentz vector in interaction;
  **/
  ERTelescopeParticle(TLorentzVector lvinteraction, Double_t deadEloss,
                       Double_t kineticEnergy, Double_t edepInThickStation = -1.,
                       Double_t edepInThinStation = -1., Double_t correctedEdepInThickStation = -1.,
                       Double_t correctedEdepInThinStation = -1., 
                       ERChannel channelOfThinStation = consts::undefined_channel,
                       ERChannel channelOfThickStation = consts::undefined_channel,
                       std::map<TString, std::pair<Int_t, Double_t>> activeDeposites = std::map<TString, std::pair<Int_t, Double_t>>());
  Double_t GetDeadEloss() const {return fDeadEloss;}
  Double_t GetKineticEnergy() const {return fKineticEnergy;}
  TLorentzVector GetLVInteraction() const {return fLVInteraction;}
  Double_t GetEdepInThickStation() const {return fEdepInThickStation;}
  Double_t GetEdepInThinStation() const {return fEdepInThinStation;}
  Double_t GetCorrectedEdepInThickStation() const {return fCorrectedEdepInThickStation;}
  Double_t GetCorrectedEdepInThinStation() const {return fCorrectedEdepInThinStation;}
  ERChannel ChannelOfThinStation() const {return fChannelOfThinStation;}
  ERChannel ChannelOfThickStation() const {return fChannelOfThickStation;}
  const std::map<TString, std::pair<Int_t, Double_t>>& GetfEdeposits() const { return fEdeposits;}
  Double_t GetfEdeposit(const TString& key) const {
      auto it = fEdeposits.find(key);  
      if (it != fEdeposits.end()) {
          return it->second.second; 
      } else {
          LOG(WARNING) << "Key not found: " << key.Data() << FairLogger::endl;
          return -1.0f;  
      }
  }  
  Int_t GetfDepChannel(const TString& key) const {
        // Проверка, существует ли ключ в карте
        auto it = fEdeposits.find(key);
        if (it != fEdeposits.end()) {
            return it->second.first;  // Возвращаем первый элемент пары
        } else {
            LOG(WARNING) << "Key not found: " << key.Data() << FairLogger::endl;
            return -1.0f;
        }
  }  
  std::vector<TString> GetAllEdepositNames() const {
      std::vector<TString> keys;  // Контейнер для ключей
      // Перебор всех элементов карты и добавление ключей в вектор
      for (const auto& pair : fEdeposits) {
          keys.push_back(pair.first);
      }
      return keys;  // Возврат всех ключей
  }

private:

  TLorentzVector fLVInteraction;
  Double_t  fKineticEnergy = 0.;
  Double_t  fDeadEloss = 0.;
  Double_t  fEdepInThickStation = -1.;
  Double_t  fEdepInThinStation = -1.;
  ERChannel fChannelOfThinStation = consts::undefined_channel;
  ERChannel fChannelOfThickStation = consts::undefined_channel;
  Double_t  fCorrectedEdepInThickStation = -1.;
  Double_t  fCorrectedEdepInThinStation = -1.;
  std::map<TString, std::pair<Int_t, Double_t>> fEdeposits;

  ClassDef(ERTelescopeParticle, 1)
};

#endif
