#ifndef tp_image_utils_AbstractImageSequence_h
#define tp_image_utils_AbstractImageSequence_h

#include "tp_image_utils/Globals.h"

namespace tp_image_utils
{
class ColorMap;

//##################################################################################################
class TP_IMAGE_UTILS_EXPORT AbstractImageSequence
{
public:
  //################################################################################################
  virtual ~AbstractImageSequence();

  //################################################################################################
  virtual tp_image_utils::ColorMap loadImage(size_t i)=0;

  //################################################################################################
  static AbstractImageSequence* loadSequence(const std::string& data);

  //################################################################################################
  static void addFactory(const std::function<AbstractImageSequence*(const std::string&)>& factory);

private:
  static std::vector<std::function<AbstractImageSequence*(const std::string&)>> sm_factories;
};

}

#endif
