#include "tp_image_utils/LoadImages.h"
#include "tp_image_utils/Scale.h"

#include "tp_utils/DebugUtils.h"
#include "tp_utils/JSONUtils.h"
#include "tp_utils/Resources.h"

#include "base64.h"

namespace tp_image_utils
{

ColorMap (*loadImage_)(const std::string& path) = nullptr;
ColorMap (*loadImageFromData_)(const std::string& data) = nullptr;
std::vector<std::string> (*imagePaths_)(const std::string& path) = nullptr;
std::vector<ColorMap> (*loadImages_)(const std::string& path, std::vector<std::string>& names, int64_t maxBytes) = nullptr;
std::vector<std::string> (*getImagePaths_)(const std::string& directory) = nullptr;

//##################################################################################################
ColorMap loadImage(const std::string& path)
{
  return (loadImage_)?loadImage_(path):ColorMap();
}

//##################################################################################################
ColorMap loadImageFromData(const std::string& data)
{
  return (loadImageFromData_)?loadImageFromData_(data):ColorMap();
}

//##################################################################################################
ColorMap loadImageFromResource(const std::string& path)
{
  auto res = tp_utils::resource(path);
  return res.data?loadImageFromData(std::string(res.data, res.size)):ColorMap();
}

//##################################################################################################
std::vector<std::string> imagePaths(const std::string& path)
{
  return (imagePaths_)?imagePaths_(path):std::vector<std::string>();
}

//##################################################################################################
std::vector<ColorMap> loadImages(const std::string& path, int64_t maxBytes)
{
  std::vector<std::string> names;
  return loadImages(path, names, maxBytes);
}

//##################################################################################################
std::vector<ColorMap> loadImages(const std::string& path, std::vector<std::string>& names, int64_t maxBytes)
{
  return (loadImages_)?loadImages_(path, names, maxBytes):std::vector<ColorMap>();
}

//##################################################################################################
ColorMap loadImageFromJson(const nlohmann::json& j)
{
  auto w = TPJSONSizeT(j, "w");
  auto h = TPJSONSizeT(j, "h");
  auto data = TPJSONString(j, "data");

  if(w<1 || h<1)
    return ColorMap();

  std::string decoded = base64_decode(data);

  size_t bytes = w * h * 4;

  if(decoded.size() != bytes)
    return ColorMap();

  ColorMap image(w, h);
  memcpy(image.data(), decoded.data(), bytes);
  return image;
}

//##################################################################################################
ByteMap loadByteMapFromJson(const nlohmann::json& j)
{  
  auto w = TPJSONSizeT(j, "w");
  auto h = TPJSONSizeT(j, "h");
  auto data = TPJSONString(j, "data");

  if(w<1 || h<1)
    return ByteMap();

  std::string decoded = base64_decode(data);

  size_t bytes = w * h;

  if(decoded.size() != bytes)
    return ByteMap();

  ByteMap image(w, h);
  memcpy(image.data(), decoded.data(), bytes);
  return image;
}

//##################################################################################################
std::vector<std::string> imageTypes()
{
  return {"*.jpg","*.png","*.bmp","*.jpeg","*.tif","*.tiff","*.tga"};
}

//##################################################################################################
std::unordered_set<std::string> imageTypesSet()
{
  return {"*.jpg","*.png","*.bmp","*.jpeg","*.tif","*.tiff","*.tga"};
}

//##################################################################################################
std::string imageTypesFilter()
{
  return "*.jpg *.png *.bmp *.jpeg *.tif *.tiff *.tga";
}

//##################################################################################################
std::vector<float> imageToFloatRGB(const ColorMap& image)
{
  std::vector<float> dest;
  dest.resize(image.width()*image.height()*3);
  float* dd = dest.data();

  const TPPixel* b = image.constData();
  const TPPixel* bMax = b + image.size();

  while(b<bMax)
  {
    (*dd) = b->r; dd++;
    (*dd) = b->g; dd++;
    (*dd) = b->b; dd++;
    b++;
  }

  return dest;
}

//##################################################################################################
std::vector<std::vector<float>> imagesToFloatRGB(const std::vector<ColorMap>& images, size_t width, size_t height)
{
  size_t imageSize = width*height*3;

  std::vector<std::vector<float>> results;

  for(ColorMap image : images)
  {
    if(width<1 || height<1)
      imageSize = image.width() * image.height() * 3;
    else if(image.width()!=width || image.height()!=height)
      image = scale(image, width, height);

    results.emplace_back();
    std::vector<float>& dest = results[results.size()-1];
    dest.reserve(imageSize);

    size_t xMax = image.width();
    size_t yMax = image.height();
    for(size_t y=0; y<yMax; y++)
    {
      for(size_t x=0; x<xMax; x++)
      {
        TPPixel p = image.pixel(x, y);
        dest.push_back(p.r);
        dest.push_back(p.g);
        dest.push_back(p.b);
      }
    }
  }

  return results;
}

//################################################################################################
std::vector<std::vector<uint8_t>> imagesToCharRGB(const std::vector<ColorMap>& images, size_t width, size_t height)
{
  size_t imageSize = width*height*3;

  std::vector<std::vector<uint8_t>> results;

  for(ColorMap image : images)
  {
    if(image.width()!=width || image.height()!=height)
      image = scale(image, width, height);

    results.emplace_back();
    std::vector<uint8_t>& dest = results[results.size()-1];
    dest.reserve(imageSize);

    for(size_t y=0; y<height; y++)
    {
      for(size_t x=0; x<width; x++)
      {
        TPPixel p = image.pixel(x, y);
        dest.push_back(p.r);
        dest.push_back(p.g);
        dest.push_back(p.b);
      }
    }
  }

  return results;
}

}
