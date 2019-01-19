#include "tp_image_utils/ToMono.h"
#include "tp_image_utils/ColorMap.h"

namespace tp_image_utils
{

//##################################################################################################
ByteMap toMono(const ByteMap& src, uint8_t threshold)
{
  ByteMap dst(src.width(), src.height());

  uint8_t* d = dst.data();
  const uint8_t* s = src.constData();
  const uint8_t* sMax = s + src.size();

  while(s<sMax)
  {
    *d = ((*s)>threshold)?255:0;
    d++;
    s++;
  }

  return dst;
}

//##################################################################################################
ByteMap toMono(const ColorMap& src, int threshold)
{  
  ByteMap dst(src.width(), src.height());
  uint8_t* d = dst.data();

  const TPPixel* s = src.constData();
  const TPPixel* sMax = s + src.size();

  while(s<sMax)
  {
    *d = (int(int(s->r) + int(s->g) + int(s->b))>threshold)?255:0;
    s++;
    d++;
  }

  return dst;
}

}
