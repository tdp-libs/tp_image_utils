#ifndef tp_image_utils_ColorMap_h
#define tp_image_utils_ColorMap_h

#include "tp_image_utils/Globals.h"

#include "tp_utils/TPPixel.h"
#include "tp_utils/RefCount.h"

#include <vector>
#include <stdint.h>

namespace tp_image_utils
{

//##################################################################################################
class ColorMap
{
  TP_REF_COUNT_OBJECTS("ColorMap");
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
  ColorMap& operator=(ColorMap&& other);

  //################################################################################################
  void fill(TPPixel value);

  //################################################################################################
  [[nodiscard]] const TPPixel* constData() const;

  //################################################################################################
  [[nodiscard]] TPPixel* data();

  //################################################################################################
  [[nodiscard]] const std::vector<TPPixel>& constDataVector() const;

  //################################################################################################
  [[nodiscard]] size_t width() const;

  //################################################################################################
  [[nodiscard]] size_t height() const;

  //################################################################################################
  //! Returns the size as a count of pixels
  [[nodiscard]] size_t size() const;

  //################################################################################################
  //! Returns the size of the image in bytes
  [[nodiscard]] size_t sizeInBytes() const;

  //################################################################################################
  void setPixel(size_t x, size_t y, TPPixel value);

  //################################################################################################
  [[nodiscard]] TPPixel pixel(size_t x, size_t y, TPPixel defaultValue=TPPixel()) const;

  //################################################################################################
  [[nodiscard]] TPPixel& pixelRef(size_t x, size_t y);

  //################################################################################################
  [[nodiscard]] ColorMap subImage(size_t left, size_t top, size_t right, size_t bottom) const;

  //################################################################################################
  //! Rotate the image 90 degrees clockwise
  [[nodiscard]] ColorMap rotate90CW() const;

  //################################################################################################
  //! Rotate the image 90 degrees counter clockwise
  [[nodiscard]] ColorMap rotate90CCW() const;

  //################################################################################################
  [[nodiscard]] ColorMap flipped() const;

  //################################################################################################
  [[nodiscard]] std::vector<TPPixel> extractRow(size_t y) const;

  //################################################################################################
  [[nodiscard]] std::vector<TPPixel> extractColumn(size_t x) const;

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
  [[nodiscard]] ColorMap clone2() const;

  //################################################################################################
  //! Clone the texture and pad to a power of 2 into an existing texture
  void clone2IntoOther(ColorMap& clone) const;

  //################################################################################################
  void setFractionalSize(float fw, float fh);

  //################################################################################################
  /*!
  Used for textures that have been padded to make them a power of 2.
  These will be a value between 0.5f and 1.0f.
  */
  [[nodiscard]] float fw() const;

  //################################################################################################
  /*!
  Used for textures that have been padded to make them a power of 2.
  These will be a value between 0.5f and 1.0f.
  */
  [[nodiscard]] float fh() const;

  //################################################################################################
  bool sameObject(const ColorMap& other) const;

private:
  struct Private;
  friend struct Private;
  Private* sd;
};
}

#endif
