#include "tp_image_utils/ColorMap.h"

#include <vector>
#include <sstream>
#include <cstring>
#include <stdint.h>

namespace tp_image_utils
{

//##################################################################################################
ColorMap::ColorMap(size_t w, size_t h, const TPPixel* data):
  m_width(w),
  m_height(h)
{
  m_data.resize(w*h, TPPixel());

  if(data)
    memcpy(m_data.data(), data, m_data.size()*sizeof(TPPixel));
}

//##################################################################################################
void ColorMap::fill(TPPixel value)
{
  std::fill(m_data.begin(), m_data.end(), value);
}

//##################################################################################################
const TPPixel* ColorMap::constData() const
{
  return m_data.data();
}

//##################################################################################################
TPPixel* ColorMap::data()
{
  return m_data.data();
}

//##################################################################################################
const std::vector<TPPixel>& ColorMap::constDataVector() const
{
  return m_data;
}

//##################################################################################################
size_t ColorMap::width()const
{
  return m_width;
}

//##################################################################################################
size_t ColorMap::height()const
{
  return m_height;
}

//##################################################################################################
size_t ColorMap::size()const
{
  return m_data.size();
}

//##################################################################################################
void ColorMap::setPixel(size_t x, size_t y, TPPixel value)
{
  if(x<m_width && y<m_height)
    m_data[(y*m_width) + x] = value;
}

//##################################################################################################
TPPixel ColorMap::pixel(size_t x, size_t y)const
{
  return (x<m_width && y<m_height)?m_data.at((y*m_width) + x):TPPixel();
}

//##################################################################################################
TPPixel& ColorMap::pixelRef(size_t x, size_t y)
{
  return m_data.at((y*m_width) + x);
}

//##################################################################################################
ColorMap ColorMap::subImage(size_t left, size_t top, size_t right, size_t bottom)const
{
  left   = tpBound(size_t(0), left,   m_width -1);
  top    = tpBound(size_t(0), top,    m_height-1);
  right  = tpBound(size_t(0), right,  m_width );
  bottom = tpBound(size_t(0), bottom, m_height);

  size_t width  = tpMax(size_t(1), right - left);
  size_t height = tpMax(size_t(1), bottom - top);

  ColorMap dst(width, height);

  for(size_t y=0; y<height; y++)
    for(size_t x=0; x<width; x++)
      dst.setPixel(x, y, pixel(x+left, y+top));

  return dst;
}

//##################################################################################################
ColorMap ColorMap::rotate90CW()const
{
  ColorMap dst(m_height, m_width);

  size_t sh = m_height-1;

  for(size_t y=0; y<m_height; y++)
    for(size_t x=0; x<m_width; x++)
      dst.setPixel(sh-y, x, pixel(x, y));

  return dst;
}

//##################################################################################################
ColorMap ColorMap::rotate90CCW()const
{
  ColorMap dst(m_height, m_width);

  size_t sw = m_width-1;

  for(size_t y=0; y<m_height; y++)
    for(size_t x=0; x<m_width; x++)
      dst.setPixel(y, sw-x, pixel(x, y));

  return dst;
}

//##################################################################################################
ColorMap ColorMap::flipped()const
{
  ColorMap dst(m_width, m_height);

  size_t sh = m_height-1;

  for(size_t y=0; y<m_height; y++)
    for(size_t x=0; x<m_width; x++)
      dst.setPixel(x, sh-y, pixel(x, y));

  return dst;
}

//##################################################################################################
std::vector<TPPixel> ColorMap::extractRow(size_t y)const
{
  std::vector<TPPixel> result;
  if(m_width>0 && m_height>0 && y<m_height)
  {
    result.resize(m_width);
    const TPPixel* s = m_data.data()+(y*m_width);
    TPPixel* d = result.data();
    std::memcpy(d, s, m_width);
  }
  return result;
}

//##################################################################################################
std::vector<TPPixel> ColorMap::extractColumn(size_t x)const
{
  std::vector<TPPixel> result;
  if(m_width>0 && m_height>0 && x<m_width)
  {
    result.resize(m_height);
    const TPPixel* s = m_data.data()+x;
    TPPixel* d = result.data();
    TPPixel* dMax = d + m_height;

    for(; d<dMax; d++, s+=m_width)
      *d = *s;
  }
  return result;
}

//##################################################################################################
void ColorMap::setRow(size_t y, const std::vector<TPPixel>& values)
{
  if(m_width>0 && m_height>0 && y<m_height && values.size() == m_width)
  {
    const TPPixel* s = values.data();
    TPPixel* d = m_data.data()+(y*m_width);
    std::memcpy(d, s, m_width);
  }
}

//##################################################################################################
void ColorMap::setColumn(size_t x, const std::vector<TPPixel>& values)
{
  if(m_width>0 && m_height>0 && x<m_width && values.size() == m_height)
  {
    TPPixel* d = m_data.data()+x;
    const TPPixel* s = values.data();
    const TPPixel* sMax = s + m_height;

    for(; s<sMax; s++, d+=m_width)
      *d = *s;
  }
}

//##################################################################################################
void ColorMap::setSize(size_t width, size_t height)
{
  m_width  = width;
  m_height = height;
  m_data.resize(width*height);
}

}
