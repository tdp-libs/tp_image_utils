#include "tp_image_utils/ByteMap.h"
#include "tp_image_utils/ColorMap.h"

#include <cstring>

namespace tp_image_utils
{

//##################################################################################################
ByteMap::ByteMap(size_t w, size_t h):
  m_width(w),
  m_height(h)
{
  m_data.resize(m_width*m_height);
}

//################################################################################################
ByteMap::ByteMap(const ColorMap& img):
  m_width(img.width()),
  m_height(img.height())
{
  m_data.resize(m_width*m_height);

  const TPPixel* s = img.constData();
  const TPPixel* sMax = s + img.size();
  auto d = m_data.data();
  while(s<sMax)
  {
    (*d) = s->r;
    d++;
    s++;
  }
}

//##################################################################################################
void ByteMap::fill(uint8_t value)
{
  if(!m_data.empty())
    memset(&m_data[0], value, m_data.size());
}

//##################################################################################################
const uint8_t* ByteMap::constData()const
{
  return m_data.data();
}

//##################################################################################################
uint8_t* ByteMap::data()
{
  return m_data.data();
}

//##################################################################################################
size_t ByteMap::width()const
{
  return m_width;
}

//##################################################################################################
size_t ByteMap::height()const
{
  return m_height;
}

//##################################################################################################
size_t ByteMap::size()const
{
  return m_data.size();
}

//##################################################################################################
void ByteMap::setPixel(size_t x, size_t y, uint8_t value)
{
  if(x<m_width && y<m_height)
    m_data[(y*m_width)+x]=value;
}

//##################################################################################################
uint8_t ByteMap::pixel(size_t x, size_t y)const
{
  return (x<m_width && y<m_height)?(*(m_data.data()+((y*m_width)+x))):0;
}

//##################################################################################################
ColorMap ByteMap::toImage()const
{
  ColorMap dst(m_width, m_height);

  const auto* s = m_data.data();
  TPPixel* d = dst.data();
  TPPixel* dMax = d + dst.size();

  while(d<dMax)
  {
    while(d<dMax)
    {
      d->r = *s;
      d->g = *s;
      d->b = *s;
      d->a = 255;
      d++;
      s++;
    }
  }

  return dst;
}

//##################################################################################################
ByteMap ByteMap::subImage(size_t left, size_t top, size_t right, size_t bottom)const
{
  left   = tpBound(size_t(0), left,   m_width -1);
  top    = tpBound(size_t(0), top,    m_height-1);
  right  = tpBound(size_t(0), right,  m_width );
  bottom = tpBound(size_t(0), bottom, m_height);

  size_t width  = tpMax(size_t(1), right - left);
  size_t height = tpMax(size_t(1), bottom - top);

  ByteMap dst(width, height);

  for(size_t y=0; y<height; y++)
    for(size_t x=0; x<width; x++)
      dst.setPixel(x, y, pixel(x+left, y+top));

  return dst;
}

//##################################################################################################
ByteMap ByteMap::rotate90CW()const
{
  ByteMap dst(m_height, m_width);

  size_t sh = m_height-1;

  for(size_t y=0; y<m_height; y++)
    for(size_t x=0; x<m_width; x++)
      dst.setPixel(sh-y, x, pixel(x, y));

  return dst;
}

//##################################################################################################
ByteMap ByteMap::rotate90CCW()const
{
  ByteMap dst(m_height, m_width);

  size_t sw = m_width-1;

  for(size_t y=0; y<m_height; y++)
    for(size_t x=0; x<m_width; x++)
      dst.setPixel(y, sw-x, pixel(x, y));

  return dst;
}

//##################################################################################################
std::vector<uint8_t> ByteMap::extractRow(size_t y)const
{
  std::vector<uint8_t> result;
  if(m_width>0 && m_height>0 && y<m_height)
  {
    result.resize(m_width);
    const auto* s = m_data.data()+(y*m_width);
    uint8_t* d = result.data();
    memcpy(d, s, m_width);
  }
  return result;
}

//##################################################################################################
std::vector<uint8_t> ByteMap::extractColumn(size_t x)const
{
  std::vector<uint8_t> result;
  if(m_width>0 && m_height>0 && x<m_width)
  {
    result.resize(m_height);
    const auto* s = m_data.data()+x;
    uint8_t* d = result.data();
    uint8_t* dMax = d + m_height;

    for(; d<dMax; d++, s+=m_width)
      *d = *s;
  }
  return result;
}

//##################################################################################################
void ByteMap::setRow(size_t y, const std::vector<uint8_t>& values)
{
  if(m_width>0 && m_height>0 && y<m_height && values.size() == m_width)
  {
    const uint8_t* s = values.data();
    auto* d = m_data.data()+(y*m_width);
    memcpy(d, s, m_width);
  }
}

//##################################################################################################
void ByteMap::setColumn(size_t x, const std::vector<uint8_t>& values)
{
  if(m_width>0 && m_height>0 && x<m_width && values.size() == m_height)
  {
    auto* d = m_data.data()+x;
    const uint8_t* s = values.data();
    const uint8_t* sMax = s + m_height;

    for(; s<sMax; s++, d+=m_width)
      *d = *s;
  }
}

}
