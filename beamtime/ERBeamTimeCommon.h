#ifndef ERBeamTimeCommon_H
#define ERBeamTimeCommon_H

#include <map>
#include <vector>

#include "ERSupport.h"

using Signal = short;
using AmplitudeAndTime = std::pair<Signal, Signal>;
using SignalsAndChannelCount = std::pair<Signal*, ERChannel>;
using Channels = std::vector<ERChannel>;
using ChannelToSignal = std::map<ERChannel, Signal>;
using ChannelToAmplitudeAndTimeSignals = std::map<ERChannel, AmplitudeAndTime>;
using ChannelMapping = std::map<ERChannel, ERChannel>;
const Signal no_signal = 0;

//--------------------------------------------------------------------------------------------------
inline ERChannel GetChannelNumber(const ERChannel raw_channel, const ChannelMapping* channelsMapping) {
  if (!channelsMapping)
    return raw_channel;
  const auto it = channelsMapping->find(raw_channel);
  if (it == channelsMapping->end())
    return raw_channel;
  return it->second;
}
//--------------------------------------------------------------------------------------------------

#endif //ERBeamTimeCommon_H