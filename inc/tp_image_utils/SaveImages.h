#ifndef tp_image_utils_SaveImages_h
#define tp_image_utils_SaveImages_h

#include "tp_image_utils/Globals.h"
#include "tp_image_utils/ColorMap.h"
#include "tp_image_utils/ColorMapF.h"
#include "tp_image_utils/ByteMap.h"

#include "json.hpp"

#include <vector>

namespace tp_image_utils
{

//##################################################################################################
void saveImage(const std::string& path, const ColorMap& image);

//##################################################################################################
std::string saveImageToData(const ColorMap& image);

//##################################################################################################
nlohmann::json saveImageToJson(const ColorMap& image);

//##################################################################################################
nlohmann::json saveByteMapToJson(const ByteMap& image);

//##################################################################################################
std::string saveColorMapFToData(const ColorMapF& image);

extern void (*saveImage_)(const std::string& path, const ColorMap& image);
extern std::string (*saveImageToData_)(const ColorMap& image);
}

#endif
