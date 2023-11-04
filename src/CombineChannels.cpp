#include "tp_image_utils/CombineChannels.h"
#include "tp_image_utils/ColorMap.h"

namespace tp_image_utils
{

//##################################################################################################
ColorMap TP_IMAGE_UTILS_EXPORT combineChannels(const ColorMap* rImage,
                                               const ColorMap* gImage,
                                               const ColorMap* bImage,
                                               const ColorMap* aImage,
                                               size_t rIndex,
                                               size_t gIndex,
                                               size_t bIndex,
                                               size_t aIndex,
                                               TPPixel defaultColor)
{
  ColorMap rgbaImage;

  size_t w=1;
  size_t h=1;

  auto getMaxSize = [&](const ColorMap* image)
  {
    if(!image)
      return;

    w = tpMax(w, image->width()); h = tpMax(h, image->height());
  };

  getMaxSize(rImage);
  getMaxSize(gImage);
  getMaxSize(bImage);
  getMaxSize(aImage);

  rgbaImage.setSize(w, h);
  TPPixel* p = rgbaImage.data();
  TPPixel* pMax = p + rgbaImage.size();

  if(rImage && gImage && bImage && aImage &&
     w==rImage->width() && h==rImage->height() &&
     w==gImage->width() && h==gImage->height() &&
     w==bImage->width() && h==bImage->height() &&
     w==aImage->width() && h==aImage->height())
  {
    const TPPixel* r = rImage->constData();
    const TPPixel* g = gImage->constData();
    const TPPixel* b = bImage->constData();
    const TPPixel* a = aImage->constData();

    for(; p<pMax; p++, r++, g++, b++, a++)
    {
      p->r = r->v[rIndex];
      p->g = g->v[gIndex];
      p->b = b->v[bIndex];
      p->a = a->v[aIndex];
    }
  }
  else if(rImage && gImage && bImage && !aImage &&
          w==rImage->width() && h==rImage->height() &&
          w==gImage->width() && h==gImage->height() &&
          w==bImage->width() && h==bImage->height())
  {
    const TPPixel* r = rImage->constData();
    const TPPixel* g = gImage->constData();
    const TPPixel* b = bImage->constData();

    auto a = defaultColor.a;

    for(; p<pMax; p++, r++, g++, b++)
    {
      p->r = r->v[rIndex];
      p->g = g->v[gIndex];
      p->b = b->v[bIndex];
      p->a = a;
    }
  }
  else
  {
    auto c = defaultColor;
    TPPixel rDefault{c.r,c.r,c.r,c.r};
    TPPixel gDefault{c.g,c.g,c.g,c.g};
    TPPixel bDefault{c.b,c.b,c.b,c.b};
    TPPixel aDefault{c.a,c.a,c.a,c.a};

    for(size_t y=0; y<h; y++)
    {
      for(size_t x=0; x<w; x++, p++)
      {
        auto getChannel = [&](const tp_image_utils::ColorMap* image, size_t index, TPPixel defaultValue)
        {
          if(image)
            return image->pixel(x, y, defaultValue).v[index];
          return defaultValue.v[index];
        };

        p->r = getChannel(rImage, rIndex, rDefault);
        p->g = getChannel(gImage, gIndex, gDefault);
        p->b = getChannel(bImage, bIndex, bDefault);
        p->a = getChannel(aImage, aIndex, aDefault);
      }
    }
  }

  return rgbaImage;
}

}

