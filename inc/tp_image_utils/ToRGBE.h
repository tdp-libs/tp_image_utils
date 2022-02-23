#ifndef tp_image_utils_ToRGBE_h
#define tp_image_utils_ToRGBE_h

#include "tp_image_utils/Globals.h"
#include "tp_image_utils/ColorMap.h"
#include "tp_image_utils/ColorMapF.h"

namespace tp_image_utils
{

//##################################################################################################
void rgbeToRGBA(const ColorMap& rgbe, ColorMapF& rgba);

//##################################################################################################
void rgbaToRGBE(const ColorMapF& rgba, ColorMap& rgbe);

}

#endif
