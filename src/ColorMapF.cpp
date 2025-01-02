#include "tp_image_utils/ColorMapF.h"

#include "tp_math_utils/Globals.h"

#include <vector>
#include <sstream>
#include <cstring>
#include <atomic>
#include <memory>

namespace tp_image_utils
{


//##################################################################################################
struct ColorMapF::SD
{
  std::unique_ptr<glm::vec4[]> data;
  size_t width{0};
  size_t height{0};

  //Used for textures that have been padded to make them a power of 2.
  //These will be a value between 0.5f and 1.0f.
  float fw{1.0f};
  float fh{1.0f};

  std::atomic_int refCount{1};

  //################################################################################################
  void detach(ColorMapF* q, bool nocopy = false)
  {
    if(refCount==1)
      return;

    auto newSD = new SD();

    if(!nocopy)
    {
      newSD->data.reset(new glm::vec4[width*height]);
      memcpy(newSD->data.get(), data.get(),width*height*sizeof(glm::vec4));

      newSD->width = width;
      newSD->height = height;
      newSD->fw = fw;
      newSD->fh = fh;
    }

    if(refCount.fetch_sub(1)==1)
      delete this;

    q->sd = newSD;
  }
};

//##################################################################################################
ColorMapF::ColorMapF(const ColorMapF& other):
  sd(other.sd)
{
  sd->refCount++;
}

//##################################################################################################
ColorMapF::ColorMapF(size_t w, size_t h, const glm::vec4* data, const glm::vec4& fill):
  sd(new SD())
{  
  sd->width = w;
  sd->height = h;
  sd->data.reset(new glm::vec4[w*h]);
  if(data){
    memcpy(sd->data.get(), data, w*h*sizeof(glm::vec4));
  }
  else{
    std::fill(sd->data.get(), sd->data.get() + w*h, fill);
  }
}

//##################################################################################################
ColorMapF::~ColorMapF()
{
  if(sd->refCount.fetch_sub(1)==1)
    delete sd;
}

//##################################################################################################
ColorMapF& ColorMapF::operator=(const ColorMapF& other)
{
  if(sd == other.sd)
    return *this;

  if(sd->refCount.fetch_sub(1)==1)
    delete sd;

  sd = other.sd;
  sd->refCount++;

  return *this;
}

//################################################################################################
ColorMapF& ColorMapF::operator=(ColorMapF&& other)
{
  if(sd == other.sd)
    return *this;

  std::swap(sd, other.sd);

  return *this;
}

//##################################################################################################
void ColorMapF::fill(const glm::vec4& value)
{
  sd->detach(this);
  std::fill(sd->data.get(), sd->data.get() + size(), value);
}

//##################################################################################################
const glm::vec4* ColorMapF::constData() const
{
  return sd->data.get();
}

//##################################################################################################
glm::vec4* ColorMapF::data()
{
  sd->detach(this);
  return sd->data.get();
}

//##################################################################################################
const glm::vec4 * ColorMapF::constDataVector() const
{
  return sd->data.get();
}

//##################################################################################################
size_t ColorMapF::width() const
{
  return sd->width;
}

//##################################################################################################
size_t ColorMapF::height() const
{
  return sd->height;
}

//##################################################################################################
size_t ColorMapF::size() const
{  
  return sd->width*sd->height;
}

//##################################################################################################
void ColorMapF::setPixel(size_t x, size_t y, const glm::vec4& value)
{
  sd->detach(this);
  if(x<sd->width && y<sd->height)
    sd->data[(y*sd->width) + x] = value;
}

//##################################################################################################
const glm::vec4& ColorMapF::pixel(size_t x, size_t y, const glm::vec4& defaultValue) const
{
  return (x<sd->width && y<sd->height)?sd->data[(y*sd->width) + x]:defaultValue;
}

//##################################################################################################
glm::vec4& ColorMapF::pixelRef(size_t x, size_t y)
{
  sd->detach(this);
  return sd->data[(y*sd->width) + x];
}

//##################################################################################################
ColorMapF ColorMapF::subImage(size_t left, size_t top, size_t right, size_t bottom) const
{
  left   = tpBound(size_t(0), left,   sd->width -1);
  top    = tpBound(size_t(0), top,    sd->height-1);
  right  = tpBound(size_t(0), right,  sd->width );
  bottom = tpBound(size_t(0), bottom, sd->height);

  size_t width  = tpMax(size_t(1), right - left);
  size_t height = tpMax(size_t(1), bottom - top);

  ColorMapF dst(width, height);

  for(size_t y=0; y<height; y++)
    for(size_t x=0; x<width; x++)
      dst.setPixel(x, y, pixel(x+left, y+top));

  return dst;
}

//##################################################################################################
ColorMapF ColorMapF::rotate90CW() const
{
  ColorMapF dst(sd->height, sd->width);

  size_t sh = sd->height-1;

  for(size_t y=0; y<sd->height; y++)
    for(size_t x=0; x<sd->width; x++)
      dst.setPixel(sh-y, x, pixel(x, y));

  return dst;
}

//##################################################################################################
ColorMapF ColorMapF::rotate90CCW() const
{
  ColorMapF dst(sd->height, sd->width);

  size_t sw = sd->width-1;

  for(size_t y=0; y<sd->height; y++)
    for(size_t x=0; x<sd->width; x++)
      dst.setPixel(y, sw-x, pixel(x, y));

  return dst;
}

//##################################################################################################
ColorMapF ColorMapF::flipped() const
{
  ColorMapF dst(sd->width, sd->height);

  size_t sh = sd->height-1;

  for(size_t y=0; y<sd->height; y++)
    for(size_t x=0; x<sd->width; x++)
      dst.setPixel(x, sh-y, pixel(x, y));

  return dst;
}

//##################################################################################################
std::vector<glm::vec4> ColorMapF::extractRow(size_t y) const
{
  std::vector<glm::vec4> result;
  if(sd->width>0 && sd->height>0 && y<sd->height)
  {
    result.resize(sd->width);
    const glm::vec4* s = sd->data.get() + y*sd->width;
    glm::vec4* d = result.data();
    std::memcpy(d, s, sd->width*sizeof(glm::vec4));
  }
  return result;
}

//##################################################################################################
std::vector<glm::vec4> ColorMapF::extractColumn(size_t x) const
{
  std::vector<glm::vec4> result;
  if(sd->width>0 && sd->height>0 && x<sd->width)
  {
    result.resize(sd->height);
    const glm::vec4* s = sd->data.get()+x;
    glm::vec4* d = result.data();
    glm::vec4* dMax = d + sd->height;

    for(; d<dMax; d++, s+=sd->width)
      *d = *s;
  }
  return result;
}

//##################################################################################################
void ColorMapF::setRow(size_t y, const std::vector<glm::vec4>& values)
{
  sd->detach(this);
  if(sd->width>0 && sd->height>0 && y<sd->height && values.size() == sd->width)
  {
    const glm::vec4* s = values.data();
    glm::vec4* d = sd->data.get()+(y*sd->width);
    std::memcpy(d, s, sd->width*sizeof(glm::vec4));
  }
}

//##################################################################################################
void ColorMapF::setColumn(size_t x, const std::vector<glm::vec4>& values)
{
  sd->detach(this);
  if(sd->width>0 && sd->height>0 && x<sd->width && values.size() == sd->height)
  {
    glm::vec4* d = sd->data.get()+x;
    const glm::vec4* s = values.data();
    const glm::vec4* sMax = s + sd->height;

    for(; s<sMax; s++, d+=sd->width)
      *d = *s;
  }
}

//##################################################################################################
void ColorMapF::setRow(size_t y, const glm::vec4 &value)
{
  sd->detach(this);
  if(sd->width>0 && sd->height>0 && y<sd->height)
  {
    glm::vec4* d = sd->data.get()+(y*sd->width);
    glm::vec4* dMax = d+sd->width;
    for(; d<dMax; d++)
      (*d) = value;
  }
}

//##################################################################################################
void ColorMapF::setColumn(size_t x, const glm::vec4 &value)
{
  sd->detach(this);
  if(sd->width>0 && sd->height>0 && x<sd->width)
  {
    glm::vec4* d = sd->data.get()+x;
    glm::vec4* dMax = sd->data.get() + size();

    for(; d<dMax; d+=sd->width)
      (*d) = value;
  }
}

//##################################################################################################
void ColorMapF::setSize(size_t width, size_t height)
{
  sd->detach(this, true);
  sd->width = width;
  sd->height = height;
  sd->data.reset(new glm::vec4[size()]);
}

//##################################################################################################
ColorMapF ColorMapF::clone2() const
{
  ColorMapF clone;
  clone2IntoOther(clone);
  return clone;
}

//##################################################################################################
void ColorMapF::clone2IntoOther(ColorMapF& clone) const
{
  if(sd->width<1||sd->height<1)
  {
    clone = ColorMapF();
    return;
  }

  size_t newSize = tp_math_utils::powerOf2(tpMax(sd->width, sd->height));

  if(sd->width == newSize && sd->height == newSize)
  {
    clone = *this;
    return;
  }

  clone = ColorMapF(newSize, newSize);

  clone.sd->fw = (float(sd->width )*sd->fw) / float(clone.sd->width );
  clone.sd->fh = (float(sd->height)*sd->fh) / float(clone.sd->height);

  size_t srcW = sd->width*sizeof(glm::vec4);
  for(size_t y=0; y<sd->height; y++)
  {
    glm::vec4* dst = clone.sd->data.get()+(y*clone.sd->width);
    memcpy(dst, sd->data.get()+(y*sd->width), srcW);

    {
      glm::vec4* d=dst+sd->width;
      glm::vec4  p = *(d-1);
      glm::vec4* dMax=dst+clone.sd->width;
      for(; d<dMax; d++)
        (*d) = p;
    }
  }

  {
    size_t dstW = clone.sd->width*sizeof(glm::vec4);
    const void* src = clone.sd->data.get()+(clone.sd->width*(sd->height-1));
    for(size_t y=sd->height; y<clone.sd->height; y++)
      memcpy(clone.sd->data.get()+(clone.sd->width*y), src, dstW);
  }
}

//##################################################################################################
void ColorMapF::setFractionalSize(float fw, float fh)
{
  sd->fw = fw;
  sd->fh = fh;
}

//##################################################################################################
float ColorMapF::fw() const
{
  return sd->fw;
}

//##################################################################################################
float ColorMapF::fh() const
{
  return sd->fh;
}


}
