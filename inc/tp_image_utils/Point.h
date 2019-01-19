#ifndef tp_image_utils_Point_h
#define tp_image_utils_Point_h

#include "tp_image_utils/ColorMap.h"

#include "tp_utils/TPPixel.h"

#include "json.hpp"

namespace tp_image_utils
{

//##################################################################################################
enum PointType
{
  PointTypeGeneral,
  PointTypeRectCorner,
  PointTypeRectSide
};

//##################################################################################################
std::string pointTypeToString(PointType pointType);

//##################################################################################################
PointType pointTypeFromString(const std::string& data);

//##################################################################################################
struct PointStyle
{
  bool drawPoint;
  bool drawLine;
  bool closeLine;

  TPPixel lineColor;

  PointStyle():
    drawPoint(true),
    drawLine(true),
    closeLine(false),
    lineColor(255, 0, 0)
  {

  }
};

//##################################################################################################
struct Point
{
  PointType type;
  float x;
  float y;

  //################################################################################################
  Point();

  //################################################################################################
  Point(float x_, float y_);

  //################################################################################################
  Point(PointType type_, float x_, float y_);

  //################################################################################################
  //! Find the length of this vector
  float length()const;

  //################################################################################################
  //! Turns this point into a unit vector
  bool normalize();

  //################################################################################################
  //! Rotates the point around (0,0)
  void rotate(float radians);

  //################################################################################################
  //! Translate the point
  void translate(const Point& vector);

  //################################################################################################
  static float dot(const Point& p1, const Point& p2);

  //################################################################################################
  bool positionEquals(const Point& other)const;
};

//##################################################################################################
//! A line or polyline
typedef std::vector<Point> Line;

//##################################################################################################
//! A list of lines or polylines
typedef std::vector<Line> LineCollection;

//##################################################################################################
Point deserializePoint(const std::string& data);

//##################################################################################################
std::string serializePoint(const Point& point);

//##################################################################################################
Line deserializeLine(const std::string& data);

//##################################################################################################
std::string serializeLine(const Line& line);

//##################################################################################################
LineCollection deserializeLineCollection(const nlohmann::json& j);

//##################################################################################################
nlohmann::json serializeLineCollection(const LineCollection& lineCollection);

//##################################################################################################
std::vector<LineCollection> deserializeLineCollections(const nlohmann::json& j);

//##################################################################################################
nlohmann::json serializeLineCollections(const std::vector<LineCollection>& lineCollections);

//##################################################################################################
//! Rotates a line around (0,0)
void rotate(Line& line, float radians);

//##################################################################################################
//! Rotates a colleaction of lines around (0,0)
void rotate(LineCollection& lines, float radians);

//##################################################################################################
//! Translate a line
void translate(Line& line, const Point& vector);

//##################################################################################################
//! Translate a colleaction of lines
void translate(LineCollection& lines, const Point& vector);

//##################################################################################################
Point meanPoint(const LineCollection& lines);

}

#endif
