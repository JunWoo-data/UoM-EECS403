// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

using namespace std;

//Programmer: Youngjun Woo
//Date: January 23, 2022
//Purpose: A collection of constants used by a treasure hunt simulation.

// RowColumnClass related values
const uint32_t DEFAULT_ROW_COLUMN_VALUE = 0;

// TerrainClass related values
const char LAND_TERRAIN = 'o';
const char WATER_TERRAIN = '.';
const char IMPASSABLE_TERRAIN = '#';
const char START_TERRAIN = '@';
const char TREASURE_TERRAIN = 'X'; 
const char VERTICAL_LINE = '|';
const char HORIZON_LINE = '-';
const char CROSS_LINE = '+';
const char DEFAULT_INVESTIGATED_DIRECTION = ' ';

// TreasureMap related values
const uint32_t DEFAULT_MAP_SIZE = 2;

// HuntSimulationClass related values
const string DEFAULT_HUNT_ORDER = "NESW";
const char DEFAULT_SAIL_CONTAINER_STRUCTURE = 'S';
const char DEFAULT_SEARCH_CONTAINER_STRUCTURE = 'Q';
const char NORTH_DIRECTION = 'N';
const char EAST_DIRECTION = 'E';
const char WEST_DIRECTION = 'W';
const char SOUTH_DIRECTION = 'S';
const char SHOW_PATH_MAP_MODE = 'M';
const char SHOW_PATH_LIST_MODE = 'L';
const char SHOW_PATH_NONE_MODE = 'N';

#endif
