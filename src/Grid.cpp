#include "tp_image_utils/Grid.h"

#include "json.hpp"

namespace tp_image_utils
{

//##################################################################################################
std::string gridTypeToString(GridType gridType)
{
  switch(gridType)
  {
  case GridTypeFinite:   return "GridTypeFinite";
  case GridTypeInfinite: return "GridTypeInfinite";
  }

  return "GridTypeInfinite";
}

//##################################################################################################
GridType gridTypeFromString(const std::string& gridType)
{
  if(gridType == "GridTypeFinite")
    return GridTypeFinite;

  return GridTypeInfinite;
}

//##################################################################################################
Grid::Grid():
  type(GridTypeInfinite),
  origin(0.0f, 0.0f),
  xAxis(1.0f, 0.0f),
  yAxis(0.0f, 1.0f),
  xCells(0),
  yCells(0)
{

}

//##################################################################################################
Grid deserializeGrid(const std::string& data)
{
  Grid result;

  if(!data.empty())
  {
    nlohmann::json j = nlohmann::json::parse(data, nullptr, false);

    result.type = gridTypeFromString(j["type"]);

    result.origin = deserializePoint(j["origin"]);

    result.xAxis = deserializePoint(j["xAxis"]);
    result.yAxis = deserializePoint(j["yAxis"]);

    result.xCells = j["xCells"];
    result.yCells = j["yCells"];
  }

  return result;
}

//##################################################################################################
std::string serializeGrid(const Grid& grid)
{
  nlohmann::json j;

  j["type"] = gridTypeToString(grid.type);

  j["origin"] = serializePoint(grid.origin);

  j["xAxis"] = serializePoint(grid.xAxis);
  j["yAxis"] = serializePoint(grid.yAxis);

  j["xCells"] = grid.xCells;
  j["yCells"] = grid.yCells;

  return j;
}

//##################################################################################################
std::vector<Grid> deserializeGrids(const std::vector<std::string>& data)
{
  std::vector<Grid> result;
  for(const std::string& grid : data)
    result.push_back(deserializeGrid(grid));
  return result;
}

//##################################################################################################
std::vector<std::string> serializeGrids(const std::vector<Grid>& grids)
{
  std::vector<std::string> result;
  for(const Grid& grid : grids)
    result.push_back(serializeGrid(grid));
  return result;
}

}
