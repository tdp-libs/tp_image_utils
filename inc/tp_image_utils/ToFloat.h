#ifndef tp_image_utils_ToFloat_h
#define tp_image_utils_ToFloat_h

#include "tp_image_utils/ColorMap.h"
#include "tp_image_utils/ColorMapF.h"

namespace tp_image_utils
{

//##################################################################################################
ColorMapF toFloat(const ColorMap& src);

//##################################################################################################
ColorMap fromFloat(const ColorMapF& src);

}

#endif
