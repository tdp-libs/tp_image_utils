#include "tp_image_utils/Globals.h"

namespace tp_image_utils
{

//##################################################################################################
std::string guessImageFormat(const std::string& data, const std::string& name)
{
  auto startsWith = [&](auto pattern)
  {
    return data.rfind(pattern, 0) == 0;
  };

  if(startsWith("\xFF\xD8\xFF"))
    return "jpg";

  if(startsWith("\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"))
    return "png";

  if(startsWith("GIF87a") || startsWith("GIF89a"))
    return "gif";

  if(startsWith("\x49\x49\x2A\x00") || startsWith("\x4D\x4D\x00\x2A"))
    return "tiff";

  if(startsWith("BM"))
    return "bmp";

  if(startsWith("RIFF") || startsWith("WEBP"))
    return "webp";

  if(startsWith("\x00\x00\x01\x00") || startsWith("\x00\x00\x02\x00"))
    return "ico";

  std::vector<std::string> results;
  tpSplit(results, name, '.');
  return results.empty()?std::string():results.back();
}

}
