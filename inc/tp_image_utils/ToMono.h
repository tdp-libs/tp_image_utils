#ifndef tp_image_utils_ToMono_h
#define tp_image_utils_ToMono_h

#include "tp_image_utils/Globals.h"
#include "tp_image_utils/ByteMap.h"

namespace tp_image_utils
{
//##################################################################################################
ByteMap toMono(const ByteMap& src, uint8_t threshold=127);

//##################################################################################################
ByteMap toMono(const ColorMap& src, int threshold=384);

}

#endif
