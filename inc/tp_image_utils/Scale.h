#ifndef tp_image_utils_Scale_h
#define tp_image_utils_Scale_h

#include "tp_image_utils/Globals.h"
#include "tp_image_utils/ByteMap.h"
#include "tp_image_utils/ColorMap.h"
#include "tp_image_utils/ColorMapF.h"

#include "tp_utils/TimeUtils.h"
#include "tp_utils/DebugUtils.h"

#include <functional>
#include <cmath>


#if defined(TP_LINUX) || defined(TP_OSX) || defined(TP_WIN32)
#define TP_SCALE_IN_THREAD
#include <thread>
#endif


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
struct ByteMapDefault
{
  template<typename GetPixel>
  uint8_t operator()(GetPixel getPixel,
                     float x1,
                     float y1,
                     float x2,
                     float y2) const
  {
    int px1 = int(std::floor(x1));
    int py1 = int(std::floor(y1));
    int px2 = int(std::ceil(x2));
    int py2 = int(std::ceil(y2));

    float a=0.0;

    for(int y=py1; y<py2; y++)
    {
      float oy = overlap(y1, y2, float(y), float(y)+1.0f);
      for(int x=px1; x<px2; x++)
      {
        float ox = overlap(x1, x2, float(x), float(x)+1.0f);
        a += (ox*oy) * float(getPixel(x, y));
      }
    }

    return uint8_t(a / ((x2-x1)*(y2-y1)));
  }
};

//##################################################################################################
struct ColorMapDefault
{
  template<typename GetPixel>
  TPPixel operator()(GetPixel getPixel,
                     float x1,
                     float y1,
                     float x2,
                     float y2) const
  {
    int px1 = int(std::floor(x1));
    int py1 = int(std::floor(y1));
    int px2 = int(std::ceil(x2));
    int py2 = int(std::ceil(y2));

    float r=0.0f;
    float g=0.0f;
    float b=0.0f;
    float a=0.0f;

    float ta = 0.0f;
    for(int y=py1; y<py2; y++)
    {
      float oy = overlap(y1, y2, float(y), float(y+1));
      for(int x=px1; x<px2; x++)
      {
        float ox = overlap(x1, x2, float(x), float(x+1));
        TPPixel p = getPixel(x, y);
        float area = ox*oy;
        ta+=area;
        r +=  area * float(p.r);
        g +=  area * float(p.g);
        b +=  area * float(p.b);
        a +=  area * float(p.a);
      }
    }

    //float ta = (x2-x1)*(y2-y1);

    return TPPixel(uint8_t(r/ta), uint8_t(g/ta), uint8_t(b/ta), uint8_t(a/ta));
  }
};

//##################################################################################################
struct ColorMapFDefault
{
  template<typename GetPixel>
  glm::vec4 operator()(GetPixel getPixel,
                       float x1,
                       float y1,
                       float x2,
                       float y2) const
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
      double oy = double(overlap(y1, y2, float(y), float(y)+1.0f));
      for(int x=px1; x<px2; x++)
      {
        double ox = double(overlap(x1, x2, float(x), float(x)+1.0f));
        glm::vec4 p = getPixel(x, y);
        double area = ox*oy;
        r +=  area * double(p.x);
        g +=  area * double(p.y);
        b +=  area * double(p.z);
        a +=  area * double(p.w);
      }
    }

    double ta = (double(x2)-double(x1))*(double(y2)-double(y1));

    return glm::vec4(float(r/ta), float(g/ta), float(b/ta), float(a/ta));
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
  TPPixel   fillPixel{0,0,0,0};
  glm::vec4 fillColorF{0.0f,0.0f,0.0f,0.0f};

  void fill(uint8_t& v) const{v = fillValue;}
  void fill(TPPixel& v) const{v = fillPixel;}
  void fill(glm::vec4& v) const{v = fillColorF;}
};

//##################################################################################################
template<typename Container, typename Value, typename CalculatePixel>
Container scale(const Container& src,
                size_t width,
                size_t height,
                CalculatePixel calculatePixel,
                const ScaleDetails& scaleDetails)
{
  TP_FUNCTION_TIME("tp_image_utils::scale");

  if(src.width()<1 || src.height()<1 || width<1 || height<1)
    return Container();

  float fx = float(src.width ()) / float(width );
  float fy = float(src.height()) / float(height);

  auto _getPixel = [&src, &scaleDetails](size_t x, size_t y) -> Value
  {
    Value v;
    scaleDetails.fill(v);
    if(x<src.width() && y<src.height())
      return src.pixel(x, y, v);
    return v;
  };

  Container result(width, height);

  float ox = 0.0;
  float oy = 0.0;
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

    ox = ((float(width)  * fx) - float(src.width ())) / 2.0f;
    oy = ((float(height) * fy) - float(src.height())) / 2.0f;

    break;
  }
  }

  auto execRow = [calculatePixel, width, fx, ox, &_getPixel](auto dst, float py, float sy)
  {
    float px=0.0;
    for(size_t x=0; x<width; x++, dst++)
    {
      float sx = (float(x+1) * fx)-ox;
      (*dst) = calculatePixel(_getPixel, px, py, sx, sy);
      px=sx;
    }
  };

  auto dst = result.data();

#ifdef TP_SCALE_IN_THREAD
  using ArrayType = decltype (dst);
  struct RowDetails_lt
  {
    ArrayType dst;
    float py;
    float sy;
  };
  std::vector<RowDetails_lt> rows;
  rows.resize(height);

  auto addRow = [&rows](auto dst, size_t y, float py, float sy)
  {
    auto& r = rows.at(y);
    r.dst = dst;
    r.py = py;
    r.sy = sy;
  };

#else

  auto addRow = [calculatePixel, width, height, fx, ox, &_getPixel, &execRow](auto dst, size_t y, float py, float sy)
  {
    TP_UNUSED(y);
    execRow(dst, py, sy);
  };

#endif

  float py=0.0;
  for(size_t y=0; y<height; y++)
  {
    float sy = (float(y+1) * fy)-oy;

    addRow(dst, y, py, sy);
    dst+=width;
    py=sy;
  }

#ifdef TP_SCALE_IN_THREAD
  {
    std::atomic<size_t> nextRow{0};
    auto rowsPtr = rows.data();
    size_t nThreads = std::min(size_t(std::thread::hardware_concurrency()), rows.size());
    std::vector<std::thread*> threads;
    threads.reserve(nThreads);
    for(size_t n=0; n<nThreads; n++)
    {
      threads.push_back(new std::thread([&]
      {
        for(;;)
        {
          size_t next = nextRow++;
          if(next >= height)
            return;

          const auto& r = rowsPtr[next];
          {
            execRow(r.dst, r.py, r.sy);
          }
        }
      }));
    }

    for(auto t : threads)
    {
      t->join();
      delete t;
    }
  }
#endif

  return result;
}

//##################################################################################################
ByteMap scale(const ByteMap& src, size_t width, size_t height);

//##################################################################################################
ColorMap scale(const ColorMap& src, size_t width, size_t height, ScaleMode scaleMode=ScaleMode::Stretch);

//##################################################################################################
ColorMapF scale(const ColorMapF& src, size_t width, size_t height);

//##################################################################################################
void halfScaleInPlace(ColorMap& img);

}

#endif
