#include "tp_image_utils/ColorMap.h"

#include "tp_math_utils/Globals.h"

#include <vector>
#include <sstream>
#include <cstring>
#include <atomic>
#include <memory>

namespace tp_image_utils
{

//##################################################################################################
struct ColorMap::Private
{
  std::unique_ptr<TPPixel[]> data;
  size_t width{0};
  size_t height{0};

  //Used for textures that have been padded to make them a power of 2.
  //These will be a value between 0.5f and 1.0f.
  float fw{1.0f};
  float fh{1.0f};

  std::atomic_int refCount{1};

  //################################################################################################
  void detach(ColorMap* q, bool nocopy = false)
  {
    if(refCount==1)
      return;

    auto newSD = new Private();
    if(!nocopy)
    {
      newSD->data.reset(new TPPixel[width*height]);
      memcpy(newSD->data.get(), data.get(), width*height*sizeof(TPPixel));

      newSD->width = width;
      newSD->height = height;
      newSD->fw = fw;
      newSD->fh = fh;
    }

    if(refCount.fetch_sub(1)==1)
      delete this;

    q->sd = newSD;
  }

  void detach(ColorMap* q, size_t newW, size_t newH)
  {
    if(refCount==1)
      return;

    auto newSD = new Private();
    newSD->data.reset(new TPPixel[newW*newH]);

    newSD->width = newW;
    newSD->height = newH;
    newSD->fw = fw;
    newSD->fh = fh;

    if(refCount.fetch_sub(1)==1)
      delete this;

    q->sd = newSD;
  }

};

//##################################################################################################
ColorMap::ColorMap(const ColorMap& other):
  sd(other.sd)
{
  sd->refCount++;
}

//##################################################################################################
ColorMap::ColorMap(size_t w, size_t h, const TPPixel* data, const TPPixel &fill):
  sd(new Private())
{  
  sd->width = w;
  sd->height = h;
  sd->data.reset( new TPPixel[w*h]);

  if(data)
    memcpy(sd->data.get(), data, w*h*sizeof(TPPixel));
  else
    std::fill(sd->data.get(),sd->data.get() + w*h, fill);
}

//##################################################################################################
ColorMap::~ColorMap()
{
  if(sd->refCount.fetch_sub(1)==1)
    delete sd;
}

//##################################################################################################
ColorMap& ColorMap::operator=(const ColorMap& other)
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
ColorMap& ColorMap::operator=(ColorMap&& other)
{
  if(sd == other.sd)
    return *this;

  std::swap(sd, other.sd);

  return *this;
}

//##################################################################################################
void ColorMap::fill(TPPixel value)
{
  sd->detach(this);
  std::fill(sd->data.get(), sd->data.get() + size(), value);
}

//##################################################################################################
const TPPixel * ColorMap::constData() const
{
  return sd->data.get();
}

//##################################################################################################
TPPixel* ColorMap::data()
{
  sd->detach(this);
  return sd->data.get();
}

//##################################################################################################
size_t ColorMap::width() const
{
  return sd->width;
}

//##################################################################################################
size_t ColorMap::height() const
{
  return sd->height;
}

//##################################################################################################
size_t ColorMap::size() const
{
  return sd->width*sd->height;
}

//##################################################################################################
size_t ColorMap::sizeInBytes() const
{
  return size() * sizeof(TPPixel);
}

//##################################################################################################
void ColorMap::setPixel(size_t x, size_t y, const TPPixel& value)
{
  sd->detach(this);
  if(x<sd->width && y<sd->height)
    sd->data[(y*sd->width) + x] = value;
}

//##################################################################################################
const TPPixel& ColorMap::pixel(size_t x, size_t y, const TPPixel& defaultValue) const
{
  return (x<sd->width && y<sd->height)?sd->data[(y*sd->width) + x]:defaultValue;
}

//##################################################################################################
TPPixel& ColorMap::pixelRef(size_t x, size_t y)
{
  sd->detach(this);
  return sd->data[(y*sd->width) + x];
}

//##################################################################################################
ColorMap ColorMap::subImage(size_t left, size_t top, size_t right, size_t bottom) const
{
  left   = tpBound(size_t(0), left,   sd->width -1);
  top    = tpBound(size_t(0), top,    sd->height-1);
  right  = tpBound(size_t(0), right,  sd->width );
  bottom = tpBound(size_t(0), bottom, sd->height);

  size_t width  = tpMax(size_t(1), right - left);
  size_t height = tpMax(size_t(1), bottom - top);

  ColorMap dst(width, height);

  for(size_t y=0; y<height; y++)
    for(size_t x=0; x<width; x++)
      dst.setPixel(x, y, pixel(x+left, y+top));

  return dst;
}

//##################################################################################################
ColorMap ColorMap::rotate90CW() const
{
  ColorMap dst(sd->height, sd->width);

  size_t sh = sd->height-1;

  for(size_t y=0; y<sd->height; y++)
    for(size_t x=0; x<sd->width; x++)
      dst.setPixel(sh-y, x, pixel(x, y));

  return dst;
}

//##################################################################################################
ColorMap ColorMap::rotate90CCW() const
{
  ColorMap dst(sd->height, sd->width);

  size_t sw = sd->width-1;

  for(size_t y=0; y<sd->height; y++)
    for(size_t x=0; x<sd->width; x++)
      dst.setPixel(y, sw-x, pixel(x, y));

  return dst;
}

//##################################################################################################
ColorMap ColorMap::flipped() const
{
  ColorMap dst(sd->width, sd->height);

  size_t sh = sd->height-1;

  for(size_t y=0; y<sd->height; y++)
    for(size_t x=0; x<sd->width; x++)
      dst.setPixel(x, sh-y, pixel(x, y));

  return dst;
}

//##################################################################################################
std::vector<TPPixel> ColorMap::extractRow(size_t y) const
{
  std::vector<TPPixel> result;
  if(sd->width>0 && sd->height>0 && y<sd->height)
  {
    result.resize(sd->width);
    const TPPixel* s = sd->data.get()+(y*sd->width);
    TPPixel* d = result.data();
    std::memcpy(d, s, sd->width*sizeof(TPPixel));
  }
  return result;
}

//##################################################################################################
std::vector<TPPixel> ColorMap::extractColumn(size_t x) const
{
  std::vector<TPPixel> result;
  if(sd->width>0 && sd->height>0 && x<sd->width)
  {
    result.resize(sd->height);
    const TPPixel* s = sd->data.get()+x;
    TPPixel* d = result.data();
    TPPixel* dMax = d + sd->height;

    for(; d<dMax; d++, s+=sd->width)
      *d = *s;
  }
  return result;
}

//##################################################################################################
void ColorMap::setRow(size_t y, const std::vector<TPPixel>& values)
{
  sd->detach(this);
  if(sd->width>0 && sd->height>0 && y<sd->height && values.size() == sd->width)
  {
    const TPPixel* s = values.data();
    TPPixel* d = sd->data.get()+(y*sd->width);
    std::memcpy(d, s, sd->width*sizeof(TPPixel));
  }
}

//##################################################################################################
void ColorMap::setColumn(size_t x, const std::vector<TPPixel>& values)
{
  sd->detach(this);
  if(sd->width>0 && sd->height>0 && x<sd->width && values.size() == sd->height)
  {
    TPPixel* d = sd->data.get()+x;
    const TPPixel* s = values.data();
    const TPPixel* sMax = s + sd->height;

    for(; s<sMax; s++, d+=sd->width)
      *d = *s;
  }
}

//##################################################################################################
void ColorMap::setRow(size_t y, const TPPixel &value)
{
  sd->detach(this);
  if(sd->width>0 && sd->height>0 && y<sd->height)
  {
    TPPixel* d = sd->data.get()+(y*sd->width);
    TPPixel* dMax = d+sd->width;
    for(; d<dMax; d++)
      (*d) = value;
  }
}

//##################################################################################################
void ColorMap::setColumn(size_t x, const TPPixel& value)
{
  sd->detach(this);
  if(sd->width>0 && sd->height>0 && x<sd->width)
  {
    TPPixel* d = sd->data.get()+x;
    TPPixel* dMax = sd->data.get()+size();

    for(; d<dMax; d+=sd->width)
      (*d) = value;
  }
}

//##################################################################################################
void ColorMap::setSize(size_t width, size_t height)
{
  sd->detach(this, true);
  sd->width = width;
  sd->height = height;
  sd->data.reset(new TPPixel[size()]);
}

//##################################################################################################
ColorMap ColorMap::clone2() const
{
  ColorMap clone;
  clone2IntoOther(clone);
  return clone;
}

//##################################################################################################
void ColorMap::clone2IntoOther(ColorMap& clone) const
{
  if(sd->width<1||sd->height<1)
  {
    clone = ColorMap();
    return;
  }

  size_t newSize = tp_math_utils::powerOf2(tpMax(sd->width, sd->height));

  if(sd->width == newSize && sd->height == newSize)
  {
    clone = *this;
    return;
  }

  clone = ColorMap(newSize, newSize);

  clone.sd->fw = (float(sd->width )*sd->fw) / float(clone.sd->width );
  clone.sd->fh = (float(sd->height)*sd->fh) / float(clone.sd->height);

  size_t srcW = sd->width*sizeof(TPPixel);
  for(size_t y=0; y<sd->height; y++)
  {
    TPPixel* dst = clone.sd->data.get()+(y*clone.sd->width);
    memcpy(dst, sd->data.get()+(y*sd->width), srcW);

    {
      TPPixel* d=dst+sd->width;
      TPPixel  p = *(d-1);
      TPPixel* dMax=dst+clone.sd->width;
      for(; d<dMax; d++)
        (*d) = p;
    }
  }

  {
    size_t dstW = clone.sd->width*sizeof(TPPixel);
    const void* src = clone.sd->data.get()+(clone.sd->width*(sd->height-1));
    for(size_t y=sd->height; y<clone.sd->height; y++)
      memcpy(clone.sd->data.get()+(clone.sd->width*y), src, dstW);
  }
}

//##################################################################################################
void ColorMap::setFractionalSize(float fw, float fh)
{
  sd->fw = fw;
  sd->fh = fh;
}

//##################################################################################################
float ColorMap::fw() const
{
  return sd->fw;
}

//##################################################################################################
float ColorMap::fh() const
{
  return sd->fh;
}

//##################################################################################################
bool ColorMap::sameObject(const ColorMap& other) const
{
  return sd == other.sd;
}


}
