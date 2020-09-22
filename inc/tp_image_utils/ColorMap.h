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
  ColorMap(const ColorMap& other);

  //################################################################################################
  ColorMap(size_t w=0, size_t h=0, const TPPixel* data=nullptr, TPPixel fill=TPPixel());

  //################################################################################################
  ~ColorMap();

  //################################################################################################
  ColorMap& operator=(const ColorMap& other);

  //################################################################################################
  void fill(TPPixel value);

  //################################################################################################
  const TPPixel* constData() const;

  //################################################################################################
  TPPixel* data();

  //################################################################################################
  const std::vector<TPPixel>& constDataVector() const;

  //################################################################################################
  size_t width() const;

  //################################################################################################
  size_t height() const;

  //################################################################################################
  //! Returns the size as a count of pixels
  size_t size() const;

  //################################################################################################
  void setPixel(size_t x, size_t y, TPPixel value);

  //################################################################################################
  TPPixel pixel(size_t x, size_t y) const;

  //################################################################################################
  TPPixel& pixelRef(size_t x, size_t y);

  //################################################################################################
  ColorMap subImage(size_t left, size_t top, size_t right, size_t bottom) const;

  //################################################################################################
  //! Rotate the image 90 degrees clockwise
  ColorMap rotate90CW() const;

  //################################################################################################
  //! Rotate the image 90 degrees counter clockwise
  ColorMap rotate90CCW() const;

  //################################################################################################
  ColorMap flipped() const;

  //################################################################################################
  std::vector<TPPixel> extractRow(size_t y) const;

  //################################################################################################
  std::vector<TPPixel> extractColumn(size_t x) const;

  //################################################################################################
  void setRow(size_t y, const std::vector<TPPixel>& values);

  //################################################################################################
  void setColumn(size_t x, const std::vector<TPPixel>& values);

  //################################################################################################
  void setRow(size_t y, TPPixel value);

  //################################################################################################
  void setColumn(size_t x, TPPixel value);

  //################################################################################################
  //! Simply sets the sise of the image, does NOT scale the contents
  void setSize(size_t width, size_t height);

  //################################################################################################
  //! Clone the texture and pad to a power of 2
  ColorMap clone2() const;

  //################################################################################################
  //! Clone the texture and pad to a power of 2 into an existing texture
  void clone2IntoOther(ColorMap& clone) const;

  //################################################################################################
  /*!
  Used for textures that have been padded to make them a power of 2.
  These will be a value between 0.5f and 1.0f.
  */
  float fw() const;

  //################################################################################################
  /*!
  Used for textures that have been padded to make them a power of 2.
  These will be a value between 0.5f and 1.0f.
  */
  float fh() const;

private:
  struct Private;
  friend struct Private;
  Private* sd;
};
}

#endif
