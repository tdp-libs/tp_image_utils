#ifndef tp_image_utils_CombineChannels_h
#define tp_image_utils_CombineChannels_h

#include "tp_image_utils/Globals.h"

#include "tp_utils/TPPixel.h"

namespace tp_image_utils
{
class ColorMap;

//##################################################################################################
ColorMap TP_IMAGE_UTILS_EXPORT combineChannels(const ColorMap* rImage,
                                               const ColorMap* gImage,
                                               const ColorMap* bImage,
                                               const ColorMap* aImage,
                                               size_t rIndex,
                                               size_t gIndex,
                                               size_t bIndex,
                                               size_t aIndex,
                                               TPPixel defaultColor);

}

#endif
