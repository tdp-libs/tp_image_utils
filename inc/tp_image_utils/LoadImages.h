#ifndef tp_image_utils_LoadImages_h
#define tp_image_utils_LoadImages_h

#include "tp_image_utils/Globals.h"
#include "tp_image_utils/ColorMap.h"
#include "tp_image_utils/ByteMap.h"

#include "json.hpp"

#include <vector>
#include <unordered_set>

namespace tp_image_utils
{

//##################################################################################################
ColorMap loadImage(const std::string& path);

//##################################################################################################
ColorMap loadImageFromData(const std::string& data);

//##################################################################################################
ColorMap loadImageFromResource(const std::string& path);

//##################################################################################################
std::vector<std::string> imagePaths(const std::string& path);

//##################################################################################################
std::vector<ColorMap> loadImages(const std::string& path, int64_t maxBytes=1073741824);

//##################################################################################################
std::vector<ColorMap> loadImages(const std::string& path, std::vector<std::string>& names, int64_t maxBytes=1073741824);

//##################################################################################################
ColorMap loadImageFromJson(const nlohmann::json& j);

//##################################################################################################
ByteMap loadByteMapFromJson(const nlohmann::json& j);

//##################################################################################################
std::vector<std::string> imageTypes();

//##################################################################################################
std::unordered_set<std::string> imageTypesSet();

//##################################################################################################
std::vector<float> imageToFloatRGB(const ColorMap& image);

//##################################################################################################
std::vector<std::vector<float>> imagesToFloatRGB(const std::vector<ColorMap>& images, size_t width, size_t height);

//##################################################################################################
std::vector<std::vector<uint8_t>> imagesToCharRGB(const std::vector<ColorMap>& images, size_t width, size_t height);

extern ColorMap (*loadImage_)(const std::string& path);
extern ColorMap (*loadImageFromData_)(const std::string& data);
extern std::vector<std::string> (*imagePaths_)(const std::string& path);
extern std::vector<ColorMap> (*loadImages_)(const std::string& path, std::vector<std::string>& names, int64_t maxBytes);

}

#endif
