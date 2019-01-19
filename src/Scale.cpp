#include "tp_image_utils/Scale.h"

namespace tp_image_utils
{

namespace scale_func
{
ByteMapBase::~ByteMapBase()=default;
ColorMapBase::~ColorMapBase()=default;

//##################################################################################################
std::string ByteMapDefault::name()
{
  return "Default";
}

//##################################################################################################
std::string ColorMapDefault::name()
{
  return "Default";
}
}

//##################################################################################################
void halfScaleInPlace(ColorMap& img)
{
  size_t width  = tpMax(size_t(1), img.width()/2);
  size_t height = tpMax(size_t(1), img.height()/2);

  if(img.width()<2 || img.height()<2)
  {
    img = scale(img, width, height);
    return;
  }

  size_t sWidth = img.width();
  const TPPixel* s = img.constData();
  TPPixel* d = img.data();

  for(size_t y=0; y<height; y++)
  {
    const TPPixel* s0 = s  + (sWidth*(y*2));
    const TPPixel* s1 = s0 + sWidth;

    for(size_t x=0; x<width; x++, d++, s0+=2, s1+=2)
    {
      int r = s0->r + s0[1].r + s1->r + s1[1].r;
      int g = s0->g + s0[1].g + s1->g + s1[1].g;
      int b = s0->b + s0[1].b + s1->b + s1[1].b;
      int a = s0->a + s0[1].a + s1->a + s1[1].a;
      d->r = uint8_t(r / 4);
      d->g = uint8_t(g / 4);
      d->b = uint8_t(b / 4);
      d->a = uint8_t(a / 4);
    }
  }

  img.setSize(width, height);
}

}
