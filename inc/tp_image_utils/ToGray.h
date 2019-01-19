#ifndef tp_image_utils_ToGray_h
#define tp_image_utils_ToGray_h

#include "tp_image_utils/Globals.h"
#include "tp_image_utils/ByteMap.h"

namespace tp_image_utils
{
//##################################################################################################
//! Convert a color image to a grayscale image
/*!
Converts a color image to a grayscale image bycalculating the mean of red, greenm and blue.

\param src 0 The color image in RGBA format.
\return The grayscale image.
*/
ByteMap toGray(const ColorMap& src);

}

#endif
