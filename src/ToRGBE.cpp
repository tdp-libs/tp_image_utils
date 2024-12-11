#include "tp_image_utils/ToRGBE.h"

#include "tp_utils/Parallel.h"

#include <cstdlib>
#include <atomic>

namespace tp_image_utils
{

//##################################################################################################
void rgbeToRGBA(const ColorMap& rgbe, ColorMapF& rgba)
{
  size_t w = rgbe.width();
  size_t h = rgbe.height();

  rgba.setSize(w, h);

  glm::vec4* rgbaData = rgba.data();

  std::atomic<size_t> c{0};
  tp_utils::parallel([&](auto /*locker*/)
  {
    for(;;)
    {
      size_t const y=c++;

      if(y>=h)
        return;

      const TPPixel* i=rgbe.constData() + (y*w);
      const TPPixel* iMax=i+w;

      glm::vec4* o=rgbaData + (y*w);
      for(; i<iMax; i++, o++)
      {
        float d = std::pow(2.0f, float(int(i->a) - 128));
        o->x = (float(i->r) /  255.0f) * d; // Some implementations use 256 here but 255 makes more sense to me.
        o->y = (float(i->g) /  255.0f) * d;
        o->z = (float(i->b) /  255.0f) * d;
        o->w = 1.0f;
      }
    }
  });
}

//##################################################################################################
void rgbaToRGBE(const ColorMapF& rgba, ColorMap& rgbe)
{
  size_t w = rgba.width();
  size_t h = rgba.height();

  rgbe.setSize(w, h);

  TPPixel* rgbeData = rgbe.data();

  std::atomic<size_t> c{0};
  tp_utils::parallel([&](auto /*locker*/)
  {
    for(;;)
    {
      size_t const y=c++;

      if(y>=h)
        return;

      const glm::vec4* i=rgba.constData() + (y*w);
      const glm::vec4* iMax=i+w;

      TPPixel* o=rgbeData + (y*w);
      for(; i<iMax; i++, o++)
      {
        if(float v = std::max(i->x, std::max(i->y, i->z)); v < 1e-32f)
        {
          o->r = 0;
          o->g = 0;
          o->b = 0;
          o->a = 0;
        }
        else
        {
          int e;
          v = static_cast<float>(std::frexp(v,&e) * 256.0f/v);
          o->r = uint8_t(std::clamp(i->x * v, 0.0f, 255.0f));
          o->g = uint8_t(std::clamp(i->y * v, 0.0f, 255.0f));
          o->b = uint8_t(std::clamp(i->z * v, 0.0f, 255.0f));
          o->a = uint8_t(e + 128);
        }
      }
    }
  });
}

}
