#include "tp_image_utils/IndexMap.h"

#include <cstring>

namespace tp_image_utils
{

//##################################################################################################
IndexMap::IndexMap(size_t w, size_t h):
  m_width(w),
  m_height(h)
{
  m_data.resize(m_width*m_height);
}

//##################################################################################################
void IndexMap::fill(uint32_t value)
{
  if(!m_data.empty())
    memset(&m_data[0], value, m_data.size());
}

//##################################################################################################
const uint32_t* IndexMap::constData() const
{
  return m_data.data();
}

//##################################################################################################
uint32_t* IndexMap::data()
{
  return m_data.data();
}

//##################################################################################################
size_t IndexMap::width() const
{
  return m_width;
}

//##################################################################################################
size_t IndexMap::height() const
{
  return m_height;
}

//##################################################################################################
size_t IndexMap::size() const
{
  return m_data.size();
}

//##################################################################################################
void IndexMap::setPixel(size_t x, size_t y, uint32_t value)
{
  if(x<m_width && y<m_height)
    m_data[(y*m_width)+x]=value;
}

//##################################################################################################
uint32_t IndexMap::pixel(size_t x, size_t y, uint32_t defaultValue) const
{
  return (x<m_width && y<m_height)?(*(m_data.data()+((y*m_width)+x))):defaultValue;
}

//##################################################################################################
IndexMap IndexMap::subImage(size_t left, size_t top, size_t right, size_t bottom) const
{
  left   = tpBound(size_t(0), left,   m_width -1);
  top    = tpBound(size_t(0), top,    m_height-1);
  right  = tpBound(size_t(0), right,  m_width );
  bottom = tpBound(size_t(0), bottom, m_height);

  size_t width  = tpMax(size_t(1), right - left);
  size_t height = tpMax(size_t(1), bottom - top);

  IndexMap dst(width, height);

  for(size_t y=0; y<height; y++)
    for(size_t x=0; x<width; x++)
      dst.setPixel(x, y, pixel(x+left, y+top));

  return dst;
}

//##################################################################################################
IndexMap IndexMap::rotate90CW() const
{
  IndexMap dst(m_height, m_width);

  size_t sh = m_height-1;

  for(size_t y=0; y<m_height; y++)
    for(size_t x=0; x<m_width; x++)
      dst.setPixel(sh-y, x, pixel(x, y));

  return dst;
}

//##################################################################################################
IndexMap IndexMap::rotate90CCW() const
{
  IndexMap dst(m_height, m_width);

  size_t sw = m_width-1;

  for(size_t y=0; y<m_height; y++)
    for(size_t x=0; x<m_width; x++)
      dst.setPixel(y, sw-x, pixel(x, y));

  return dst;
}

//##################################################################################################
std::vector<uint32_t> IndexMap::extractRow(size_t y) const
{
  std::vector<uint32_t> result;
  if(m_width>0 && m_height>0 && y<m_height)
  {
    result.resize(m_width);
    const auto* s = m_data.data()+(y*m_width);
    uint32_t* d = result.data();
    memcpy(d, s, m_width * sizeof(uint32_t));
  }
  return result;
}

//##################################################################################################
std::vector<uint32_t> IndexMap::extractColumn(size_t x) const
{
  std::vector<uint32_t> result;
  if(m_width>0 && m_height>0 && x<m_width)
  {
    result.resize(m_height);
    const auto* s = m_data.data()+x;
    uint32_t* d = result.data();
    uint32_t* dMax = d + m_height;

    for(; d<dMax; d++, s+=m_width)
      *d = *s;
  }
  return result;
}

//##################################################################################################
void IndexMap::setRow(size_t y, const std::vector<uint32_t>& values)
{
  if(m_width>0 && m_height>0 && y<m_height && values.size() == m_width)
  {
    const uint32_t* s = values.data();
    auto* d = m_data.data()+(y*m_width);
    memcpy(d, s, m_width * sizeof(uint32_t));
  }
}

//##################################################################################################
void IndexMap::setColumn(size_t x, const std::vector<uint32_t>& values)
{
  if(m_width>0 && m_height>0 && x<m_width && values.size() == m_height)
  {
    auto* d = m_data.data()+x;
    const uint32_t* s = values.data();
    const uint32_t* sMax = s + m_height;

    for(; s<sMax; s++, d+=m_width)
      *d = *s;
  }
}

//##################################################################################################
void IndexMap::setSize(size_t width, size_t height)
{
  m_width  = width;
  m_height = height;
  m_data.resize(width*height);
}

}
