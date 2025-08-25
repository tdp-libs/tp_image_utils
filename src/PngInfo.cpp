#include "tp_image_utils/PngInfo.h"
#include <cstdint>

namespace tp_image_utils
{

namespace
{
//##################################################################################################
constexpr uint32_t byteswap32(uint32_t value)
{
  return ((value & 0x000000FFU) << 24) |
         ((value & 0x0000FF00U) << 8)  |
         ((value & 0x00FF0000U) >> 8)  |
         ((value & 0xFF000000U) >> 24);
}
}

//##################################################################################################
bool pngSize(const std::string& data, size_t& width, size_t& height)
{
  // A PNG header is [0x89 P N G][0x0D 0x0A 0x1A 0x0A][xx xx xx xx][I H D R][ww ww ww ww][hh hh hh hh]
  // where [ww ww ww ww] and [hh hh hh hh] are 32-bit big-endian width and height

  if(data.size() < 24)
    return false;

  const uint32_t* data32 = reinterpret_cast<const uint32_t*>(data.data());
  static constexpr uint32_t bigEndianHeader = (0x89 << 24) | ('P' << 16) | ('N' << 8) | 'G';
  static constexpr uint32_t littleEndianHeader = ('G' << 24) | ('N' << 16) | ('P' << 8) | 0x89;

  if(data32[0] == littleEndianHeader)
  {
    width = static_cast<size_t>(byteswap32(data32[4]));
    height = static_cast<size_t>(byteswap32(data32[5]));
    return true;
  }
  else if(data32[0] == bigEndianHeader)
  {
    width = static_cast<size_t>(data32[4]);
    height = static_cast<size_t>(data32[5]);
    return true;
  }

  return false;
}
}
