#include "tp_image_utils/ToFloat.h"

namespace tp_image_utils
{

//##################################################################################################
ColorMapF toFloat(const ColorMap& src)
{
  ColorMapF dst(src.width(), src.height());

  const TPPixel* i=src.constData();
  glm::vec4* o=dst.data();
  glm::vec4* oMax=o+dst.size();
  for(; o<oMax; i++, o++)
  {
    o->x = float(i->r) / 255.0f;
    o->y = float(i->g) / 255.0f;
    o->z = float(i->b) / 255.0f;
    o->w = float(i->a) / 255.0f;
  }

  return dst;
}

//##################################################################################################
ColorMap fromFloat(const ColorMapF& src)
{
  ColorMap dst(src.width(), src.height());

  const glm::vec4* i=src.constData();
  TPPixel* o=dst.data();
  TPPixel* oMax=o+dst.size();
  for(; o<oMax; i++, o++)
  {
    o->r = uint8_t(std::clamp(i->x * 255.0f, 0.0f, 255.0f)+0.5f);
    o->g = uint8_t(std::clamp(i->y * 255.0f, 0.0f, 255.0f)+0.5f);
    o->b = uint8_t(std::clamp(i->z * 255.0f, 0.0f, 255.0f)+0.5f);
    o->a = uint8_t(std::clamp(i->w * 255.0f, 0.0f, 255.0f)+0.5f);
  }

  return dst;
}

}
