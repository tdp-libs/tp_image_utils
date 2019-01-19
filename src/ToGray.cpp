#include "tp_image_utils/ToGray.h"
#include "tp_image_utils/ColorMap.h"

namespace tp_image_utils
{

//##################################################################################################
ByteMap toGray(const ColorMap& src)
{
  ByteMap dst(src.width(), src.height());
  uint8_t* d = dst.data();

  const TPPixel* s = src.constData();
  const TPPixel* sMax = s + src.size();

  while(s<sMax)
  {
    *d = uint8_t((int(s->r) + int(s->g) + int(s->b))/3);
    s++;
    d++;
  }

  return dst;
}

}
