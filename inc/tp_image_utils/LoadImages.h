#ifndef tp_image_utils_LoadImages_h
#define tp_image_utils_LoadImages_h

#include "tp_image_utils/Globals.h" // IWYU pragma: keep
#include "tp_image_utils/ColorMap.h"
#include "tp_image_utils/ColorMapF.h"
#include "tp_image_utils/ByteMap.h"

#include "json.hpp"

#include <vector>
#include <unordered_set>

namespace tp_image_utils
{

//##################################################################################################
ColorMap loadImage(const std::string& path);

//##################################################################################################
ColorMap loadImage(const std::string& path, std::vector<std::string>& errors);

//##################################################################################################
ColorMap loadImageFromData(const std::string& data);

//##################################################################################################
ColorMap loadImageFromData(const std::string& data, std::vector<std::string>& errors);

//##################################################################################################
ColorMap loadImageFromResource(const std::string& path);

//##################################################################################################
ColorMap loadImageFromResource(const std::string& path, std::vector<std::string>& errors);

//##################################################################################################
//! List all image file in a directory.
/*!
This returns paths to all image files in a directory that the loadImage function can probably load.

\param directory to search for images in.
\return A list of absolute paths to image files contained in the provided directory.
*/
std::vector<std::string> imagePaths(const std::string& directory);

//##################################################################################################
std::vector<ColorMap> loadImages(const std::string& path, int64_t maxBytes=1073741824);

//##################################################################################################
std::vector<ColorMap> loadImages(const std::string& path, std::vector<std::string>& names, int64_t maxBytes=1073741824);

//##################################################################################################
ColorMap loadImageFromJson(const nlohmann::json& j);

//##################################################################################################
ColorMap loadImageFromJson(const nlohmann::json& j, std::vector<std::string>& errors);

//##################################################################################################
ByteMap loadByteMapFromJson(const nlohmann::json& j);

//##################################################################################################
ByteMap loadByteMapFromJson(const nlohmann::json& j, std::vector<std::string>& errors);

//##################################################################################################
ColorMapF loadColorMapFFromData(const std::string& data);

//##################################################################################################
ColorMapF loadColorMapFFromData(const std::string& data, std::vector<std::string>& errors);

//##################################################################################################
std::vector<std::string> imageTypes();

//##################################################################################################
std::unordered_set<std::string> imageTypesSet();

//##################################################################################################
std::string imageTypesFilter();

//##################################################################################################
std::string imageAndVideoTypesFilter();

//##################################################################################################
std::string videoTypesFilter();

//##################################################################################################
std::vector<float> imageToFloatRGB(const ColorMap& image);

//##################################################################################################
std::vector<std::vector<float>> imagesToFloatRGB(const std::vector<ColorMap>& images, size_t width, size_t height);

//##################################################################################################
std::vector<std::vector<uint8_t>> imagesToCharRGB(const std::vector<ColorMap>& images, size_t width, size_t height);

extern ColorMap (*loadImage_)(const std::string& path, std::vector<std::string>& errors);
extern ColorMap (*loadImageFromData_)(const std::string& data, std::vector<std::string>& errors);
extern std::vector<std::string> (*imagePaths_)(const std::string& path);
extern std::vector<ColorMap> (*loadImages_)(const std::string& path, std::vector<std::string>& names, int64_t maxBytes);

}

#endif
