#ifndef tp_image_utils_ColorMap_h
#define tp_image_utils_ColorMap_h

#include "tp_utils/TPPixel.h"

#include <vector>
#include <stdint.h>

namespace tp_image_utils
{

//##################################################################################################
class ColorMap
{
public:
  //################################################################################################
  ColorMap(size_t w=0, size_t h=0, const TPPixel* data=nullptr);

  //################################################################################################
  void fill(TPPixel value);

  //################################################################################################
  const TPPixel* constData() const;

  //################################################################################################
  TPPixel* data();

  //################################################################################################
  size_t width()const;

  //################################################################################################
  size_t height()const;

  //################################################################################################
  //! Returns the size as a count of pixels
  size_t size()const;

  //################################################################################################
  void setPixel(size_t x, size_t y, TPPixel value);

  //################################################################################################
  TPPixel pixel(size_t x, size_t y)const;

  //################################################################################################
  ColorMap subImage(size_t left, size_t top, size_t right, size_t bottom)const;

  //################################################################################################
  //! Rotate the image 90 degrees clockwise
  ColorMap rotate90CW()const;

  //################################################################################################
  //! Rotate the image 90 degrees counter clockwise
  ColorMap rotate90CCW()const;

  //################################################################################################
  std::vector<TPPixel> extractRow(size_t y)const;

  //################################################################################################
  std::vector<TPPixel> extractColumn(size_t x)const;

  //################################################################################################
  void setRow(size_t y, const std::vector<TPPixel>& values);

  //################################################################################################
  void setColumn(size_t x, const std::vector<TPPixel>& values);

  //################################################################################################
  //! Simply sets the sise of the image, does NOT scale the contents
  void setSize(size_t width, size_t height);

private:
  std::vector<TPPixel> m_data;
  size_t m_width;
  size_t m_height;
};
}

#endif
