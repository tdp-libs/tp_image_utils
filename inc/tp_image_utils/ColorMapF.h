#ifndef tp_image_utils_ColorMapF_h
#define tp_image_utils_ColorMapF_h

#include "tp_image_utils/Globals.h"

#include "tp_utils/RefCount.h"

#include "glm/glm.hpp"

#include <vector>
#include <stdint.h>

namespace tp_image_utils
{

//##################################################################################################
class ColorMapF
{
  TP_REF_COUNT_OBJECTS("ColorMapF");
public:
  //################################################################################################
  ColorMapF(const ColorMapF& other);

  //################################################################################################
  ColorMapF(size_t w=0, size_t h=0, const glm::vec4* data=nullptr, const glm::vec4& fill=glm::vec4(0,0,0,1));

  //################################################################################################
  ~ColorMapF();

  //################################################################################################
  ColorMapF& operator=(const ColorMapF& other);

  //################################################################################################
  ColorMapF& operator=(ColorMapF&& other);

  //################################################################################################
  void fill(const glm::vec4& value);

  //################################################################################################
  const glm::vec4* constData() const;

  //################################################################################################
  [[nodiscard]]glm::vec4* data();

  //################################################################################################
  [[nodiscard]]const std::vector<glm::vec4>& constDataVector() const;

  //################################################################################################
  [[nodiscard]]size_t width() const;

  //################################################################################################
  [[nodiscard]]size_t height() const;

  //################################################################################################
  //! Returns the size as a count of pixels
  [[nodiscard]]size_t size() const;

  //################################################################################################
  void setPixel(size_t x, size_t y, const glm::vec4& value);

  //################################################################################################
  [[nodiscard]]glm::vec4 pixel(size_t x, size_t y, const glm::vec4& defaultValue=glm::vec4()) const;

  //################################################################################################
  [[nodiscard]]glm::vec4& pixelRef(size_t x, size_t y);

  //################################################################################################
  [[nodiscard]]ColorMapF subImage(size_t left, size_t top, size_t right, size_t bottom) const;

  //################################################################################################
  //! Rotate the image 90 degrees clockwise
  [[nodiscard]]ColorMapF rotate90CW() const;

  //################################################################################################
  //! Rotate the image 90 degrees counter clockwise
  [[nodiscard]]ColorMapF rotate90CCW() const;

  //################################################################################################
  [[nodiscard]]ColorMapF flipped() const;

  //################################################################################################
  [[nodiscard]]std::vector<glm::vec4> extractRow(size_t y) const;

  //################################################################################################
  [[nodiscard]]std::vector<glm::vec4> extractColumn(size_t x) const;

  //################################################################################################
  void setRow(size_t y, const std::vector<glm::vec4>& values);

  //################################################################################################
  void setColumn(size_t x, const std::vector<glm::vec4>& values);

  //################################################################################################
  void setRow(size_t y, glm::vec4 value);

  //################################################################################################
  void setColumn(size_t x, glm::vec4 value);

  //################################################################################################
  //! Simply sets the sise of the image, does NOT scale the contents
  void setSize(size_t width, size_t height);

  //################################################################################################
  //! Clone the texture and pad to a power of 2
  [[nodiscard]]ColorMapF clone2() const;

  //################################################################################################
  //! Clone the texture and pad to a power of 2 into an existing texture
  void clone2IntoOther(ColorMapF& clone) const;

  //################################################################################################
  void setFractionalSize(float fw, float fh);

  //################################################################################################
  /*!
  Used for textures that have been padded to make them a power of 2.
  These will be a value between 0.5f and 1.0f.
  */
  [[nodiscard]]float fw() const;

  //################################################################################################
  /*!
  Used for textures that have been padded to make them a power of 2.
  These will be a value between 0.5f and 1.0f.
  */
  [[nodiscard]]float fh() const;

private:
  struct Private;
  friend struct Private;
  Private* sd;
};
}

#endif
