#include "tp_image_utils/Globals.h"

namespace tp_image_utils
{

//##################################################################################################
std::string fileTypeToString(FileType fileType)
{
  switch(fileType)
  {
    case FileType::Unknown     : return "Unknown";

    case FileType::ImagesStart : return "Unknown";
    case FileType::jpg         : return "jpg";
    case FileType::png         : return "png";
    case FileType::gif         : return "gif";
    case FileType::tiff        : return "tiff";
    case FileType::bmp         : return "bmp";
    case FileType::webp        : return "webp";
    case FileType::ico         : return "ico";
    case FileType::ImagesEnd   : return "Unknown";

    case FileType::VideosStart : return "Unknown";
    case FileType::mp4         : return "mp4";
    case FileType::VideosEnd   : return "Unknown";
  }
}

//##################################################################################################
FileType fileTypeFromString(const std::string& fileType)
{
  std::string lower = tpToLower(fileType);
  if(fileType == "jpg") return FileType::jpg;
  if(fileType == "png") return FileType::png;
  if(fileType == "gif") return FileType::gif;
  if(fileType == "tiff") return FileType::tiff;
  if(fileType == "bmp") return FileType::bmp;
  if(fileType == "webp") return FileType::webp;
  if(fileType == "ico") return FileType::ico;
  if(fileType == "mp4") return FileType::mp4;

  return FileType::Unknown;
}

//##################################################################################################
FileType guessImageFormat(const std::string& data, const std::string& name)
{
  using namespace std::string_literals;

  auto startsWith = [&](const std::string& pattern)
  {
    return data.rfind(pattern, 0) == 0;
  };

  auto compare = [&](const std::string& pattern, size_t pos)
  {
    if((pos + pattern.size()) >= data.size())
      return false;

    return data.compare(pos, pattern.size(), pattern) == 0;
  };

  if(startsWith("\xFF\xD8\xFF"s))
    return FileType::jpg;

  if(startsWith("\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"s))
    return FileType::png;

  if(startsWith("GIF87a"s) || startsWith("GIF89a"s))
    return FileType::gif;

  if(startsWith("\x49\x49\x2A\x00"s) || startsWith("\x4D\x4D\x00\x2A"s))
    return FileType::tiff;

  if(startsWith("BM"))
    return FileType::bmp;

  if(startsWith("RIFF") || startsWith("WEBP"))
    return FileType::webp;

  if(compare("ftypisom", 4))
    return FileType::mp4;

  if(startsWith(std::string("\x00\x00\x01\x00"s)) || startsWith("\x00\x00\x02\x00"s))
    return FileType::ico;

  std::vector<std::string> results;
  tpSplit(results, name, '.');
  return results.empty()?FileType::Unknown:fileTypeFromString(results.back());
}

//##################################################################################################
bool isImage(FileType fileType)
{
  return fileType>FileType::ImagesStart && fileType<FileType::ImagesEnd;
}

//##################################################################################################
bool isVideo(FileType fileType)
{
  return fileType>FileType::VideosStart && fileType<FileType::VideosEnd;
}

}
