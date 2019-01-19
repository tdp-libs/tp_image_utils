#ifndef tp_image_utils_Scale_h
#define tp_image_utils_Scale_h

#include "tp_image_utils/Globals.h"
#include "tp_image_utils/ByteMap.h"
#include "tp_image_utils/ColorMap.h"

#include <functional>
#include <cmath>

namespace tp_image_utils
{

namespace scale_func
{

//##################################################################################################
template<typename T>
T overlap(T min1, T max1, T min2, T max2)
{
  return tpMax(T(), tpMin(max1, max2) - tpMax(min1, min2));
}

//##################################################################################################
struct ByteMapBase
{
  virtual ~ByteMapBase();

  virtual std::string name()=0;
  virtual uint8_t operator()(std::function<uint8_t(int x, int y)> getPixel,
                             double x1,
                             double y1,
                             double x2,
                             double y2) = 0;
};

//##################################################################################################
struct ColorMapBase
{
  virtual ~ColorMapBase();

  virtual std::string name()=0;
  virtual TPPixel operator()(std::function<TPPixel(int x, int y)> getPixel,
                           double x1,
                           double y1,
                           double x2,
                           double y2) = 0;
};

//##################################################################################################
struct ByteMapDefault : public ByteMapBase
{
  std::string name() override;

  uint8_t operator()(std::function<uint8_t(int x, int y)> getPixel,
                     double x1,
                     double y1,
                     double x2,
                     double y2) override
  {
    int px1 = int(std::floor(x1));
    int py1 = int(std::floor(y1));
    int px2 = int(std::ceil(x2));
    int py2 = int(std::ceil(y2));

    double a=0.0;

    for(int y=py1; y<py2; y++)
    {
      double oy = overlap(y1, y2, double(y), double(y)+1.0);
      for(int x=px1; x<px2; x++)
      {
        double ox = overlap(x1, x2, double(x), double(x)+1.0);
        a += (ox*oy) * double(getPixel(x, y));
      }
    }

    return uint8_t(a / ((x2-x1)*(y2-y1)));
  }
};

//##################################################################################################
struct ColorMapDefault : public ColorMapBase
{
  std::string name() override;

  TPPixel operator()(std::function<TPPixel(int x, int y)> getPixel,
                   double x1,
                   double y1,
                   double x2,
                   double y2)override
  {
    int px1 = int(std::floor(x1));
    int py1 = int(std::floor(y1));
    int px2 = int(std::ceil(x2));
    int py2 = int(std::ceil(y2));

    double r=0.0;
    double g=0.0;
    double b=0.0;
    double a=0.0;

    for(int y=py1; y<py2; y++)
    {
      double oy = overlap(y1, y2, double(y), double(y)+1.0);
      for(int x=px1; x<px2; x++)
      {
        double ox = overlap(x1, x2, double(x), double(x)+1.0);
        TPPixel p = getPixel(x, y);
        double area = ox*oy;
        r +=  area * double(p.r);
        g +=  area * double(p.g);
        b +=  area * double(p.b);
        a +=  area * double(p.a);
      }
    }

    double ta = (x2-x1)*(y2-y1);

    return TPPixel(uint8_t(r/ta), uint8_t(g/ta), uint8_t(b/ta), uint8_t(a/ta));
  }
};

}

//##################################################################################################
enum class ScaleMode
{
  Stretch,  //! Stretch the image to fit the new dimensions.
  Crop,     //! Scale the image to fit on one dimension and crop the src on the other.
  Pad,      //! Scale the image to fit on one dimesion and pad the other dimension with filler.
  PadCenter //! Same as Pad but centered.
};

//##################################################################################################
struct ScaleDetails
{
  ScaleMode mode{ScaleMode::Stretch};

  uint8_t fillValue{0};
  TPPixel   fillPixel;

  void fill(uint8_t& v)const{v = fillValue;}
  void fill(TPPixel& v)const{v = fillPixel;}
};

//##################################################################################################
template<typename Container, typename Value>
Container scale(const Container& src,
                size_t width,
                size_t height,
                std::function<Value(std::function<Value(size_t x, size_t y)> getPixel,
                                    double x1,
                                    double y1,
                                    double x2,
                                    double y2)> calculatePixel,
                const ScaleDetails& scaleDetails)
{
  if(src.width()<1 || src.height()<1 || width<1 || height<1)
    return Container();

  double fx = double(src.width ()) / double(width );
  double fy = double(src.height()) / double(height);

  auto _getPixel = [&src, &scaleDetails](size_t x, size_t y) -> Value
  {
    if(x<src.width() && y<src.height())
      return src.pixel(x, y);
    Value v;
    scaleDetails.fill(v);
    return v;
  };

  Container result(width, height);

  double ox = 0.0;
  double oy = 0.0;
  switch(scaleDetails.mode)
  {
  case ScaleMode::Stretch: //-----------------------------------------------------------------------
  {
    break;
  }

  case ScaleMode::Crop: //--------------------------------------------------------------------------
  {
    if(fx>fy)
      fx=fy;
    else
      fy=fx;
    break;
  }

  case ScaleMode::Pad: //---------------------------------------------------------------------------
  {
    if(fx>fy)
      fy=fx;
    else
      fx=fy;
    break;
  }

  case ScaleMode::PadCenter: //---------------------------------------------------------------------
  {
    if(fx>fy)
      fy=fx;
    else
      fx=fy;

    ox = ((double(width)  * fx) - double(src.width ())) / 2.0;
    oy = ((double(height) * fy) - double(src.height())) / 2.0;

    break;
  }
  }

  double py=0.0;
  for(size_t y=0; y<height; y++)
  {
    double sy = (double(y) * fy)-oy;

    double px=0.0;
    for(size_t x=0; x<width; x++)
    {
      double sx = (double(x) * fx)-ox;
      result.setPixel(x, y, calculatePixel(_getPixel, px, py, sx, sy));
      px=sx;
    }
    py=sy;
  }

  return result;
}

//##################################################################################################
inline ByteMap scale(const ByteMap& src, size_t width, size_t height)
{
  return scale<ByteMap, uint8_t>(src, width, height, scale_func::ByteMapDefault(), ScaleDetails());
}

//##################################################################################################
inline ColorMap scale(const ColorMap& src, size_t width, size_t height)
{
  return scale<ColorMap, TPPixel>(src, width, height, scale_func::ColorMapDefault(), ScaleDetails());
}

//##################################################################################################
void halfScaleInPlace(ColorMap& img);

}

#endif
