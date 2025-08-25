#include "tp_image_utils/LoadImages.h"
#include "tp_image_utils/Scale.h"

#include "tp_utils/JSONUtils.h"
#include "tp_utils/Resources.h"
#include "tp_utils/DebugUtils.h"

#include "base64.h"

namespace tp_image_utils
{

ColorMap (*loadImage_)(const std::string& path, std::vector<std::string>& errors) = nullptr;
ColorMap (*loadImageFromData_)(const std::string& data, std::vector<std::string>& errors) = nullptr;
std::vector<std::string> (*imagePaths_)(const std::string& path) = nullptr;
std::vector<ColorMap> (*loadImages_)(const std::string& path, std::vector<std::string>& names, int64_t maxBytes) = nullptr;
std::vector<std::string> (*getImagePaths_)(const std::string& directory) = nullptr;

namespace
{
//##################################################################################################
struct PrintErrors
{
  std::vector<std::string> errors;

  //################################################################################################
  ~PrintErrors()
  {
  for(const auto& error : errors)
    tpWarning() << error;
  }
};
}

//##################################################################################################
ColorMap loadImage(const std::string& path)
{
  PrintErrors e;
  return loadImage(path, e.errors);
}

//##################################################################################################
ColorMap loadImage(const std::string& path, std::vector<std::string>& errors)
{
  return (loadImage_)?loadImage_(path, errors):ColorMap();
}

//##################################################################################################
ColorMap loadImageFromData(const std::string& data)
{
  PrintErrors e;
  return loadImageFromData(data, e.errors);
}

//##################################################################################################
ColorMap loadImageFromData(const std::string& data, std::vector<std::string>& errors)
{
  return (loadImageFromData_)?loadImageFromData_(data, errors):ColorMap();
}

//##################################################################################################
ColorMap loadImageFromResource(const std::string& path)
{
  PrintErrors e;
  return loadImageFromResource(path, e.errors);
}

//##################################################################################################
ColorMap loadImageFromResource(const std::string& path, std::vector<std::string>& errors)
{
  auto res = tp_utils::resource(path);

  if(res.data)
    return loadImageFromData(std::string(res.data, res.size), errors);

  errors.push_back("Failed to find resource: " + path);
  return ColorMap();
}

//##################################################################################################
std::vector<std::string> imagePaths(const std::string& directory)
{
  return (imagePaths_)?imagePaths_(directory):std::vector<std::string>();
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
  PrintErrors e;
  return loadImageFromJson(j, e.errors);
}

//##################################################################################################
ColorMap loadImageFromJson(const nlohmann::json& j, std::vector<std::string>& errors)
{
  auto w = TPJSONSizeT(j, "w");
  auto h = TPJSONSizeT(j, "h");
  auto data = TPJSONString(j, "data");

  if(w<1 || h<1)
  {
    errors.push_back("Error with image size, w: " + std::to_string(w) + " h: " + std::to_string(h));
    return ColorMap();
  }

  std::string decoded = base64_decode(data);

  size_t bytes = w * h * 4;

  if(decoded.size() != bytes)
  {
    errors.push_back("Size mismatch, decoded: " + std::to_string(decoded.size()) + " expected: " + std::to_string(bytes));
    return ColorMap();
  }

  ColorMap image(w, h);
  memcpy(image.data(), decoded.data(), bytes);
  return image;
}

//##################################################################################################
ByteMap loadByteMapFromJson(const nlohmann::json& j)
{
  PrintErrors e;
  return loadByteMapFromJson(j, e.errors);
}

//##################################################################################################
ByteMap loadByteMapFromJson(const nlohmann::json& j, std::vector<std::string>& errors)
{
  auto w = TPJSONSizeT(j, "w");
  auto h = TPJSONSizeT(j, "h");
  auto data = TPJSONString(j, "data");

  if(w<1 || h<1)
  {
    errors.push_back("Error with image size, w: " + std::to_string(w) + " h: " + std::to_string(h));
    return ByteMap();
  }

  std::string decoded = base64_decode(data);

  size_t bytes = w * h;

  if(decoded.size() != bytes)
  {
    errors.push_back("Size mismatch, decoded: " + std::to_string(decoded.size()) + " expected: " + std::to_string(bytes));
    return ByteMap();
  }

  ByteMap image(w, h);
  memcpy(image.data(), decoded.data(), bytes);
  return image;
}

//##################################################################################################
ColorMapF loadColorMapFFromData(const std::string& data)
{
  PrintErrors e;
  return loadColorMapFFromData(data, e.errors);
}

//##################################################################################################
ColorMapF loadColorMapFFromData(const std::string& data, std::vector<std::string>& errors)
{
  ColorMapF result;

  size_t headerSize = sizeof(uint32_t)*2; // Width & Height
  if(data.size() < headerSize)
  {
    errors.push_back("Data smaller than expected header, data size: " + std::to_string(data.size()) + " expected: " + std::to_string(headerSize));
    return result;
  }

  const char* src = data.data();

  uint32_t size[2];
  std::memcpy(size, src, headerSize);
  src += headerSize;

  size_t   dataSize = (size[0]*size[1]) * 4 * sizeof(float); // Data
  size_t totalSize  = headerSize + dataSize;

  if(data.size() != totalSize)
  {
    errors.push_back("Size mismatch, data size: " + std::to_string(data.size()) + " expected: " + std::to_string(totalSize));
    return result;
  }

  result.setSize(size[0], size[1]);
  std::memcpy(result.data(), src, dataSize);

  return result;
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
std::string imageAndVideoTypesFilter()
{
  return "*.jpg *.png *.bmp *.jpeg *.tif *.tiff *.tga *.mp4";
}

//##################################################################################################
std::string videoTypesFilter()
{
  return "*.mp4";
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
