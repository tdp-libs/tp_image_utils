#include "tp_image_utils/SaveImages.h"
#include "tp_image_utils/ColorMap.h"

#include "base64.h"

namespace tp_image_utils
{
void (*saveImage_)(const std::string& path, const ColorMap& image) = nullptr;
std::string (*saveImageToData_)(const ColorMap& image) = nullptr;

//##################################################################################################
void saveImage(const std::string& path, const ColorMap& image)
{
  if(saveImage_)
    saveImage_(path, image);
}

//##################################################################################################
std::string saveImageToData(const ColorMap& image)
{
  return (saveImageToData_)?saveImageToData_(image):std::string();
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

}
