#ifndef tp_image_utils_Grid_h
#define tp_image_utils_Grid_h

#include "tp_image_utils/Globals.h"
#include "tp_image_utils/Point.h"

#include "tp_utils/TPPixel.h"

#include <string>
#include <vector>

namespace tp_image_utils
{

//##################################################################################################
enum GridType
{
  GridTypeFinite,
  GridTypeInfinite
};

//##################################################################################################
std::string gridTypeToString(GridType gridType);

//##################################################################################################
GridType gridTypeFromString(const std::string& gridType);

//##################################################################################################
struct GridStyle
{
  bool drawPoint; //!< Draw a point at the intersections
  bool drawLine;  //!< Draw grid lines

  TPPixel lineColor;

  GridStyle():
    drawPoint(true),
    drawLine(true)
  {

  }
};

//##################################################################################################
struct Grid
{
  GridType type;

  Point origin; //!< The origin used to align the grid with the lines.

  Point xAxis; //!< This should be a unit vector pointing down the x axis of the grid.
  Point yAxis; //!< This should be a unit vector pointing down the y axis of the grid.

  int xCells; //!< The number of cells along the x axis.
  int yCells; //!< The number of cells along the y axis.

  //################################################################################################
  Grid();
};

//##################################################################################################
Grid deserializeGrid(const std::string& data);

//##################################################################################################
std::string serializeGrid(const Grid& grid);

//##################################################################################################
std::vector<Grid> deserializeGrids(const std::vector<std::string>& data);

//##################################################################################################
std::vector<std::string> serializeGrids(const std::vector<Grid>& grids);
}

#endif
