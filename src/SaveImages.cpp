#include "tp_image_utils/SaveImages.h"
#include "tp_image_utils/ColorMap.h"

#include "base64.h"

#include <cstring>

namespace tp_image_utils
{
bool (*saveImage_)(const std::string& path, const ColorMap& image) = nullptr;
std::string (*saveImageToData_)(const ColorMap& image) = nullptr;
std::string (*saveJPEGToData_)(const tp_image_utils::ColorMap& image, int quality) = nullptr;

//##################################################################################################
bool saveImage(const std::string& path, const ColorMap& image)
{
  if(saveImage_)
    return saveImage_(path, image);

  return false;
}

//##################################################################################################
std::string saveImageToData(const ColorMap& image)
{
  return (saveImageToData_)?saveImageToData_(image):std::string();
}

//##################################################################################################
std::string saveJPEGToData(const ColorMap& image, int quality)
{
  return (saveJPEGToData_)?saveJPEGToData_(image, quality):std::string();
}

//##################################################################################################
nlohmann::json saveImageToJson(const ColorMap& image)
{
  nlohmann::json j;

  size_t w = image.width();
  size_t h = image.height();

  j["w"] = w;
  j["h"] = h;

  if(w<1 || h<1)
    return j;

  j["data"] = base64_encode(reinterpret_cast<const unsigned char*>(image.constData()), image.size()*4);

  return j;
}

//##################################################################################################
nlohmann::json saveByteMapToJson(const ByteMap& image)
{
  nlohmann::json j;

  size_t w = image.width();
  size_t h = image.height();

  j["w"] = w;
  j["h"] = h;

  if(w<1 || h<1)
    return j;

  j["data"] = base64_encode(image.constData(), image.size());

  return j;
}

//##################################################################################################
std::string saveColorMapFToData(const ColorMapF& image)
{
  size_t headerSize = sizeof(uint32_t)*2;               // Width & Height
  size_t   dataSize = image.size() * 4 * sizeof(float); // Data
  size_t totalSize  = headerSize + dataSize;

  std::string data;
  data.resize(totalSize);

  char* dst = data.data();

  uint32_t size[2]{uint32_t(image.width()), uint32_t(image.height())};
  std::memcpy(dst, size, headerSize);
  dst+=headerSize;

  std::memcpy(dst, image.constData(), dataSize);

  return data;
}

}
