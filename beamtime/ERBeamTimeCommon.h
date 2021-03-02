#ifndef ERBeamTimeCommon_H
#define ERBeamTimeCommon_H

#include <map>
#include <vector>

using Channel = ushort;
using Signal = short;
using AmplitudeAndTime = std::pair<Signal, Signal>;
using SignalsAndChannelCount = std::pair<Signal*, Channel>;
using Channels = std::vector<Channel>;
using ChannelToSignal = std::map<Channel, Signal>;
using ChannelToAmplitudeAndTimeSignals = std::map<Channel, AmplitudeAndTime>;
using ChannelMapping = std::map<Channel, Channel>;
const Signal no_signal = 0;

//--------------------------------------------------------------------------------------------------
inline Channel GetChannelNumber(const Channel raw_channel, const ChannelMapping* channelsMapping) {
  if (!channelsMapping)
    return raw_channel;
  const auto it = channelsMapping->find(raw_channel);
  if (it == channelsMapping->end())
    return raw_channel;
  return it->second;
}
//--------------------------------------------------------------------------------------------------

#endif //ERBeamTimeCommon_H