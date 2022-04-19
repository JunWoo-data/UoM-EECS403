// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include "HuntSimulationClass.h"
#include <iostream>
#include <string>
#include <getopt.h>
#include "TreasureMap.h"
#include "constants.h"
using namespace std;

//Programmer: Youngjun Woo
//Date: January 23, 2022
//Purpose: This class will act as the basis for the treasure hunt simulation
//         that investigates water and island to find treasure under different 
//         options determined by command line arguments.

/*

    This program can be run with seven different command line options:
        
        [--help | -h]
            Print a useful help message and exit the program.
        
        [--captain | -c] <"QUEUE"|"STACK">
            The route-finding container used while sailing in the water.
            (if unspecified, container default is stack)
        
        [--first-mate | -f] <"QUEUE"|"STACK">
            The route-finding container used while searching on land.
            (if unspecified, container default is queue)

        [--hunt-order | -o] <ORDER>
            The order of discovery of adjacent tiles around the current 
            location, a four character string using exactly one of each of the
            four characters 'N', 'E', 'S', and 'W'.
            (if unspecified, the default order is: N -> E -> S -> W)

        [--verbose | -v]
            Print verbose output while searching

        [--stats | -s]
            Display statistics after the search is complete

        [--show-path | -p] <M|L>
            Display a treasure map or the list of locations that describe the 
            path. 

*/

// Read and process command line options.
void HuntSimulationClass::get_options(int argc, char** argv)
{
  int option_index = 0, option = 0;
  opterr = false;

  size_t len;
  string stringArg;
  int countN = 0;
  int countE = 0;
  int countW = 0;
  int countS = 0;

  struct option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                              { "captain", required_argument, nullptr, 'c' },
                              { "first-mate", required_argument, nullptr, 'f' },
                              { "hunt-order", required_argument, nullptr, 'o' },
                              { "verbose", no_argument, nullptr, 'v' },
                              { "stats", no_argument, nullptr, 's' },
                              { "show-path", required_argument, nullptr, 'p' },
                              { nullptr, 0, nullptr, '\0' }};
  
  while ((option = getopt_long(argc, argv, "hc:f:o:vsp:", 
          longOpts, &option_index)) != -1)
  {
    switch (option)
    {
      case 'h':
      {
        cout << "This program reads a txt file that contains information\n"
             << "about the chain of islands that we want to investigate.\n"
             << "   - .: water\n"
             << "   - o: land\n"
             << "   - #: impassable\n"
             << "   - @: starting location\n"
             << "   - $: treasure location\n"
             << "It then simulate a treasure hunt based on the setting you\n"
             << "select by the command line arguments, then output result of\n" 
             << "the hunt. More detailed output can be displayed by verbose,\n"
             << "stats, or show-path options.\n"
             << "Usage: \'./haunt\n\t[--help | -h]\n"
             <<                   "\t[--captain | -c] <'QUEUE'|'STACK'>\n"
             <<                   "\t[--first-mate | -f] <'QUEUE'|'STACK'>\n"
             <<                   "\t[--hunt-order | -o] <ORDER>\n"
             <<                   "\t[--verbose | -v]\n"
             <<                   "\t[--stats | -s]\n"
             <<                   "\t[--show-path | -p] <M|L>\n"
             <<                   "\t< <TXT Map File>\'" << '\n';
        exit(0);
      } // case 'h'

      case 'c':  
      {
        stringArg = optarg;

        if ((stringArg != "STACK") && (stringArg != "QUEUE"))
        {
          cerr << "ERROR: The [-c | --captain] must be followed by 'STACK' or "
               << "'QUEUE'" << '\n';    
          exit(1);
        } // if

        if (stringArg == "STACK")
        {
          sailContainerStructure = DEFAULT_SAIL_CONTAINER_STRUCTURE;  
        }

        else
        {
          sailContainerStructure = DEFAULT_SEARCH_CONTAINER_STRUCTURE;  
        }
        
        break;
      } // case 'c'

      case 'f':
      {
        stringArg = optarg;

        if ((stringArg != "STACK") && (stringArg != "QUEUE"))
        {
          cerr << "ERROR: The [-f | --first-mate] must be followed by 'STACK' "
               << "or 'QUEUE'" << '\n';    
          exit(1);
        } // if

        if (stringArg == "STACK")
        {
          searchContainerStructure = DEFAULT_SAIL_CONTAINER_STRUCTURE;  
        }

        else
        {
          sailContainerStructure = DEFAULT_SEARCH_CONTAINER_STRUCTURE;  
        }
        
        break;
      } // case 'f'

      case 'o':
      {
        huntOrder = optarg;    

        len = huntOrder.length();
        if (len != 4)
        {
          cerr << "ERROR: Length of hunt order must be 4." << '\n';  
          exit(1);
        } // if

        else 
        {
          for (size_t i = 0; i < len; i++)
          {
            stringArg = huntOrder[i];

            if (stringArg.compare("N") != 0 && stringArg.compare("E") != 0
                && stringArg.compare("W") != 0 && stringArg.compare("S") != 0)
            {
              cerr << "ERROR: Hunt order argument must contain one " 
                   << "and only one of each of 'NESW' (in any array)." << '\n';
              exit(1);
            } // if char is in N, E, W, S

            if (stringArg.compare("N") == 0)
            {
              countN += 1;  
            } // if char == N

            else if (stringArg.compare("E") == 0)    
            {
              countE += 1;  
            } // if char == E

            else if (stringArg.compare("W") == 0)
            {
              countW += 1;  
            } // if char == W

            else
            {
              countS += 1;  
            } // if char == S
          } // for   

          if ((countN != 1) | (countE != 1) | (countS != 1) | (countW != 1))
          {
            cerr << "ERROR: Hunt order argument must contain one " 
                   << "and only one of each of 'NESW' (in any array)." << '\n';
            exit(1);  
          }
        } // else
        
        break;
      } // case 'o'

      case 'v':
      {
        isVerbose = true;
        break;  
      } // case 'v'

      case 's':
      {
        isStats = true;
        break;
      } // case 's'

      case 'p':
      {
        //Check if -p option is specified before.
        if ((showPathMethod == SHOW_PATH_LIST_MODE) | 
            (showPathMethod == SHOW_PATH_MAP_MODE))
        {
          cerr << "ERROR: [-p | --show-path] can only be specified once "
               << '\n'; 
          exit(1); 
        }
        
        stringArg = optarg;
        
        if (stringArg == "M")
        {
          showPathMethod = SHOW_PATH_MAP_MODE;    
        }

        else if (stringArg == "L")
        {
          showPathMethod = SHOW_PATH_LIST_MODE;  
        }

        else
        {
          cerr << "ERROR: The [-p | --show-path] must be followed by 'M' "
               << "or 'L'" << '\n';    
          exit(1);    
        }
    
        // showPathMethod = *optarg;
        
        // // Check if -p option is followed by M or L.
        // if ((showPathMethod != SHOW_PATH_MAP_MODE) && 
        //     (showPathMethod != SHOW_PATH_LIST_MODE))
        // {
        //   cerr << "ERROR: The [-p | --show-path] must be followed by 'M' "
        //        << "or 'L'" << '\n';    
        //   exit(1);
        // }
        
        break;
      } // case 'p'  
      
      default:
      {
        cerr << "ERROR: Missing argument or invalid options." << '\n';
        exit(1);  
      } // case default
    } // switch    
  } // while 
} //get_options

void HuntSimulationClass::readMapFromFile()
{
  string inLine;
  string inMode = "#";
  uint32_t inSize;
  RowColumnClass currentRowCol;
  TerrainClass currentTerrain;
  string terrain; 
 
  // Skip header lines
  while (inMode.compare("#") == 0)
  {
    getline(cin, inLine);  
    inMode = inLine[0];
  }

  // Read mode and map size
  cin >> inSize >> ws;

  // Resize the mapVector of map and fill with water terrain because when we 
  // read list mode, default terrain will be a water terrain.
  map.setSizeTo(inSize);
  map.resizeMapVector(map.getSize());
  
  if (inMode == "L")
  {
    uint32_t row, col;

    while (cin >> row >> col >> terrain)
    {
      currentRowCol.setRowCol(row, col);
      
      if (terrain == "o")
      {
        currentTerrain.setTerrainTo(LAND_TERRAIN, false, 
                                    DEFAULT_INVESTIGATED_DIRECTION);
                                   
      } // terrain == "o"
      else if (terrain == ".")
      {
        currentTerrain.setTerrainTo(WATER_TERRAIN, false, 
                                    DEFAULT_INVESTIGATED_DIRECTION); 
                                    
      } // terrain == "."
      else if (terrain == "#")
      {
        currentTerrain.setTerrainTo(IMPASSABLE_TERRAIN, false, 
                                    DEFAULT_INVESTIGATED_DIRECTION);
                                    
      } // terrain == "#"
      else if (terrain == "@")
      {
        currentTerrain.setTerrainTo(WATER_TERRAIN, false, 
                                    DEFAULT_INVESTIGATED_DIRECTION);                      
        map.setStartLocation(currentRowCol);  
      } // terrain == "@"
      else
      {
        currentTerrain.setTerrainTo(LAND_TERRAIN, false, 
                                    DEFAULT_INVESTIGATED_DIRECTION);
        map.setTreasureLocation(currentRowCol);
      } // terrain == "$"
    
      map.setTerrainAtLocation(currentRowCol, currentTerrain);
    } // cin
  } // mode = "L"

  else
  {
    uint32_t rInd = 0;

    while(getline(cin, inLine))
    {
      for (uint32_t cInd = 0; cInd < map.getSize(); cInd++)
      {
        currentRowCol.setRowCol(rInd, cInd);
        terrain = inLine[cInd];

        if (terrain.compare("o") == 0)
        { 
          currentTerrain.setTerrainTo(LAND_TERRAIN, false, 
                                      DEFAULT_INVESTIGATED_DIRECTION);
        } // if o

        else if (terrain.compare("$") == 0)
        {
          currentTerrain.setTerrainTo(LAND_TERRAIN, false, 
                                      DEFAULT_INVESTIGATED_DIRECTION);
          map.setTreasureLocation(currentRowCol);
        } // if $

        else if (terrain.compare(".") == 0)
        {
          currentTerrain.setTerrainTo(WATER_TERRAIN, false, 
                                      DEFAULT_INVESTIGATED_DIRECTION);
        } // if .

        else if (terrain.compare("@") == 0)
        {
          currentTerrain.setTerrainTo(WATER_TERRAIN, false, 
                                      DEFAULT_INVESTIGATED_DIRECTION);
          map.setStartLocation(currentRowCol);
        } // if @

        else
        {
          currentTerrain.setTerrainTo(IMPASSABLE_TERRAIN, false, 
                                      DEFAULT_INVESTIGATED_DIRECTION);  
        } // if #
        
        map.setTerrainAtLocation(currentRowCol, currentTerrain);
      } // for cInd    

      rInd += 1; 
    } // while getline  
  } // mode = 'M'
}

void HuntSimulationClass::printSimulationStats(bool isFound) const
{
  cout << "--- STATS ---" << '\n';
  cout << "Starting location: " << map.getStartLocation().getRow() << "," 
       << map.getStartLocation().getCol() << '\n';
  cout << "Water locations investigated: " << numOfWaterInvestigated << '\n';
  cout << "Land locations investigated: " << numOfLandInvestigated << '\n';  
  cout << "Went ashore: " << numOfWentAshore << '\n';

  if (isFound)
  {
    cout << "Path length: " << pathLength << '\n';
    cout << "Treasure location: " << map.getTreasureLocation().getRow() << ","
         << map.getTreasureLocation().getCol() << '\n';  
  }  
  cout << "--- STATS ---" << '\n';
}

// Set simulation by setting start location as sail location
void HuntSimulationClass::setSimulation()
{
  RowColumnClass rowCol;
  TerrainClass terrain;

  // set start location as discovered 
  rowCol = map.getStartLocation();
  map.getTerrainAtLocation(rowCol, terrain);
  terrain.setTerrainTo(WATER_TERRAIN, true, DEFAULT_INVESTIGATED_DIRECTION);
  map.setTerrainAtLocation(rowCol, terrain);
  
  // add start location to sail container
  sailContainer.push_back(rowCol);

  // if verbose option, print out additional information
  if (isVerbose)
  {
    cout << "Treasure hunt started at: " << rowCol.getRow() << ","
         << rowCol.getCol() << '\n';  
  } 
}

// Set sail location before start to investigate adjacent terrains.
// Return true if there is a coordinate in the sail container to be set as 
// sail location, and return false otherwise.
bool HuntSimulationClass::setSailLocation()
{
  if (sailContainer.empty())
  {
    return false;  
  }

  else
  {
    if (sailContainerStructure == DEFAULT_SAIL_CONTAINER_STRUCTURE)
    {
      sailLocation = sailContainer.back();
      sailContainer.pop_back();
    } // container == "STACK"  

    else
    {
      sailLocation = sailContainer.front();
      sailContainer.pop_front();  
    } // container == "QUEUE"
    
    numOfWaterInvestigated += 1;
    return true;
  }
}

RowColumnClass HuntSimulationClass::investigateAdjacent(char direction, 
                                                 const RowColumnClass &atRowCol)
{
  RowColumnClass adjacentRowCol;

  if (direction == NORTH_DIRECTION)
  {
    adjacentRowCol.setRowCol(atRowCol.getRow() - 1, atRowCol.getCol());  
  } // if direction == "N"

  else if (direction == EAST_DIRECTION)
  {
    adjacentRowCol.setRowCol(atRowCol.getRow(), atRowCol.getCol() + 1);  
  } // if direction == "E"

  else if (direction == WEST_DIRECTION)
  {
    adjacentRowCol.setRowCol(atRowCol.getRow(), atRowCol.getCol() - 1);  
  } // if direction == "W"

  else
  {
    adjacentRowCol.setRowCol(atRowCol.getRow() + 1, atRowCol.getCol());    
  } // if direction == "S"

  return adjacentRowCol;
}

bool HuntSimulationClass::checkIsValidLocation(const RowColumnClass &inLocation)
{  
  TerrainClass outTerrain;

  // Check if inLocation is out of bound of the map
  if ((inLocation.getRow() >= map.getSize()) | 
      (inLocation.getCol() >= map.getSize())) 
  {
    return false;  
  } 
  
  // Check if terrain at inLocation is discovered 
  map.getTerrainAtLocation(inLocation, outTerrain); 
  
  if (outTerrain.getIsDiscovered() == true)
  {
    return false;
  }

  else
  {
    return true;  
  }
}

bool HuntSimulationClass::runCaptainHunting()
{
  // set sail location from sail container
  bool isSuccess;
    
  isSuccess = setSailLocation();    
  
  if (isSuccess == false)
  {
    return false;  
  }
  
  // investigate adjacent and check adjacent location is valid to put sail or 
  // search container
  char direction;
  RowColumnClass adjacentLocation;
  bool isValid;
  TerrainClass outTerrain;

  for (uint32_t i = 0; i < huntOrder.length(); i++)
  {
    direction = huntOrder[i];
    adjacentLocation = investigateAdjacent(direction, sailLocation);
    isValid = checkIsValidLocation(adjacentLocation);

    if (isValid == true)
    {
      map.getTerrainAtLocation(adjacentLocation, outTerrain);
      
      if (outTerrain.getCharacter() == LAND_TERRAIN)
      {
        searchContainer.push_back(adjacentLocation);
        outTerrain.setTerrainTo(LAND_TERRAIN, true, direction);
        map.setTerrainAtLocation(adjacentLocation, outTerrain); 
        
        if (sailContainerStructure == DEFAULT_SAIL_CONTAINER_STRUCTURE)
        {
          sailContainer.push_back(sailLocation); 
        } // if sail container == "STACK"

        else
        {
          sailContainer.push_front(sailLocation);
        } // if sail container == "QUEUE"  
        
        numOfWaterInvestigated -= 1;

        return true;
      } // if adjacent terrain is land

      else if (outTerrain.getCharacter() == WATER_TERRAIN)
      {
        sailContainer.push_back(adjacentLocation);
        outTerrain.setTerrainTo(WATER_TERRAIN, true, direction);
        map.setTerrainAtLocation(adjacentLocation, outTerrain);    
      } // if adjacent terrain is water
    } // isValid == true
  }

  return true;
}

bool HuntSimulationClass::setSearchLocation()
{
  if (searchContainer.empty())
  {
    return false;  
  }

  else
  {
    if (searchContainerStructure == DEFAULT_SEARCH_CONTAINER_STRUCTURE)
    {
      searchLocation = searchContainer.front();
      searchContainer.pop_front();
    } // container == "QUEUE"  

    else
    {
      searchLocation = searchContainer.back();
      searchContainer.pop_back();  
    } // container == "STACK"
    
    numOfLandInvestigated += 1;

    return true;
  }  
}

// do Firtst Mate's hunting and return true if mate find the treasure and return 
// false if not.
bool HuntSimulationClass::runMateHunting()
{
  // set search location from sail container
  bool isSuccess;
    
  isSuccess = setSearchLocation();    
  
  if (isSuccess == false)
  {
    return false;  
  }
  
  // check if current search location is treasure land
  if (isTreasureLand(searchLocation))  
  {
    // cout << "Treasure found at " << searchLocation.getRow() << "," 
    //      << searchLocation.getCol() << " with path length " << pathLength 
    //      << "." << '\n'; 

    numOfLandInvestigated -= 1;
    return true;
  } // if current search location is treasure land

  // investigate adjacent and check adjacent location is valid to put search
  // container or adjacent location is treasure land
  char direction;
  RowColumnClass adjacentLocation;
  bool isValid;
  TerrainClass outTerrain;

  for (uint32_t i = 0; i < huntOrder.length(); i++)
  {
    direction = huntOrder[i];
    adjacentLocation = investigateAdjacent(direction, searchLocation);
    isValid = checkIsValidLocation(adjacentLocation);

    if (isValid == true)
    {
      map.getTerrainAtLocation(adjacentLocation, outTerrain);
      
      if (outTerrain.getCharacter() == LAND_TERRAIN)
      {
        searchContainer.push_back(adjacentLocation);
        outTerrain.setTerrainTo(LAND_TERRAIN, true, direction);
        map.setTerrainAtLocation(adjacentLocation, outTerrain);  

        if (isTreasureLand(adjacentLocation))
        {
          return true;
        } // if adjacent terrain is treasure land
      } // if adjacent terrain is land
    } // isValid == true
  }

  return false;
}

bool HuntSimulationClass::runHuntProcess()
{
  bool isFound = false;
  bool isSuccess;

  while (!isFound)
  {
    isSuccess = runCaptainHunting(); 

    if (isSuccess == false)
    {
      if (isVerbose)
      {
        cout << "Treasure hunt failed" << '\n';  
      }
      
      if (isStats)
      {
        printSimulationStats(isFound);
      }

      cout << "No treasure found after investigating " 
           << numOfWaterInvestigated + numOfLandInvestigated << " locations." 
           << '\n';  

      return false;
    }

    if (!searchContainer.empty())
    {
      numOfWentAshore += 1;

      if (isVerbose)
      {
        if (searchContainerStructure == DEFAULT_SEARCH_CONTAINER_STRUCTURE)
        {
          cout << "Went ashore at: " << searchContainer.front().getRow() << ","
               << searchContainer.front().getCol() << '\n';
        } // if search container structure == "QUEUE"

        else
        {
          cout << "Went ashore at: " << searchContainer.back().getRow() << ","
               << searchContainer.back().getCol() << '\n';
        } // if search container structure == "STACK"
      } // if verbose mode

      while (!((isFound == true) | (searchContainer.empty())))
      { 
        isFound = runMateHunting();    
      } // while find treasure of search container is empty
      
      if (isVerbose)
      {
        if (isFound == true)
        {
          cout << "Searching island... party found treasure at "
               << map.getTreasureLocation().getRow() << ","
               << map.getTreasureLocation().getCol() << "." << '\n'; 
        } // is mate found treasure

        else
        {
          cout << "Searching island... party returned with no treasure." 
               << '\n';  
        } // if mate failed to find treasure
      } // if verbose mode
    } // if search container is not empty
  }
  
  numOfWaterInvestigated += 1;
  numOfLandInvestigated += 1;
  
  doBackTrace();
  pathLength = pathInfo.size(); 
   
  if (isStats)
  {
    printSimulationStats(isFound);
  }

  if ((showPathMethod == SHOW_PATH_LIST_MODE) | 
      (showPathMethod == SHOW_PATH_MAP_MODE))
  {
    showPath();  
  }

  cout << "Treasure found at " << map.getTreasureLocation().getRow() << ","
       << map.getTreasureLocation().getCol() << " with path length "
       << pathLength << "." << '\n';

  return true;
}


bool HuntSimulationClass::isTreasureLand(const RowColumnClass &inLand)
{
  if ((inLand.getRow() == map.getTreasureLocation().getRow()) &
      (inLand.getCol() == map.getTreasureLocation().getCol()))
  {
    return true;  
  }  

  else
  {
    return false;  
  }
}

RowColumnClass HuntSimulationClass::getBeforeRowCol(const RowColumnClass &inRowCol, 
                                                    char inDirection)                                                     
{
  RowColumnClass outRowCol;

  if (inDirection == WEST_DIRECTION)
  {
    outRowCol.setRowCol(inRowCol.getRow(), inRowCol.getCol() + 1);    
  } // if direction == 'W'

  else if (inDirection == EAST_DIRECTION)
  {
    outRowCol.setRowCol(inRowCol.getRow(), inRowCol.getCol() - 1);    
  } // if direction == 'E'

  else if (inDirection == NORTH_DIRECTION)
  {
    outRowCol.setRowCol(inRowCol.getRow() + 1, inRowCol.getCol());    
  } // if direction == 'N'

  else
  {
    outRowCol.setRowCol(inRowCol.getRow() - 1, inRowCol.getCol());      
  } // if direction == 'S'

  return outRowCol;
}

RowColumnClass HuntSimulationClass::getAfterRowCol(const RowColumnClass &inRowCol, 
                                                   char inDirection)                                                     
{
  RowColumnClass outRowCol;

  if (inDirection == WEST_DIRECTION)
  {
    outRowCol.setRowCol(inRowCol.getRow(), inRowCol.getCol() - 1);    
  } // if direction == 'W'

  else if (inDirection == EAST_DIRECTION)
  {
    outRowCol.setRowCol(inRowCol.getRow(), inRowCol.getCol() + 1);    
  } // if direction == 'E'

  else if (inDirection == NORTH_DIRECTION)
  {
    outRowCol.setRowCol(inRowCol.getRow() - 1, inRowCol.getCol());    
  } // if direction == 'N'

  else
  {
    outRowCol.setRowCol(inRowCol.getRow() + 1, inRowCol.getCol());      
  } // if direction == 'S'

  return outRowCol;
}

void HuntSimulationClass::doBackTrace()
{
  RowColumnClass currentRowCol;
  char direction;
  TerrainClass outTerrain;

  currentRowCol = map.getTreasureLocation();

  while (!((currentRowCol.getRow() == map.getStartLocation().getRow()) &
           (currentRowCol.getCol() == map.getStartLocation().getCol())))
  {
    map.getTerrainAtLocation(currentRowCol, outTerrain);
    direction = outTerrain.getInvestigatedDirection();

    pathInfo.push_front(direction);

    currentRowCol = getBeforeRowCol(currentRowCol, direction);    
  }  
}

void HuntSimulationClass::drawLineOnTerrain(const RowColumnClass &currentRowCol)
{
  TerrainClass currentTerrain;

  map.getTerrainAtLocation(currentRowCol, currentTerrain);
  
  if (abs(int(getAfterRowCol(currentRowCol, pathInfo[1]).getRow()) - 
          int(getBeforeRowCol(currentRowCol, pathInfo.front()).getRow())) == 2)
  {
    currentTerrain.setTerrainCharacter(VERTICAL_LINE);   
    map.setTerrainAtLocation(currentRowCol, currentTerrain);  
  }

  else if (abs(int(getAfterRowCol(currentRowCol, pathInfo[1]).getCol()) - 
               int(getBeforeRowCol(currentRowCol, pathInfo.front()).getCol())) == 2)      
  {
    currentTerrain.setTerrainCharacter(HORIZON_LINE);
    map.setTerrainAtLocation(currentRowCol, currentTerrain);          
  }

  else
  {
    currentTerrain.setTerrainCharacter(CROSS_LINE);
    map.setTerrainAtLocation(currentRowCol, currentTerrain);            
  }
}  

void HuntSimulationClass::showPath()
{
  char direction;
  RowColumnClass currentRowCol;
  TerrainClass currentTerrain;
  
  currentRowCol = map.getStartLocation();
  map.getTerrainAtLocation(currentRowCol, currentTerrain);

  if (showPathMethod == SHOW_PATH_LIST_MODE)
  {
    cout << "Sail:" << '\n';
    cout << currentRowCol.getRow() << "," << currentRowCol.getCol() << '\n';
  } // if list mode  
  
  direction = pathInfo.front();
  currentRowCol = getAfterRowCol(currentRowCol, direction);  
  map.getTerrainAtLocation(currentRowCol, currentTerrain);
  
  drawLineOnTerrain(currentRowCol); // for map mode  
  
  while (currentTerrain.getCharacter() == WATER_TERRAIN)
  {
    if (showPathMethod == SHOW_PATH_LIST_MODE)
    {
      cout << currentRowCol.getRow() << "," << currentRowCol.getCol() << '\n'; 
    } // if list mode  
 
    pathInfo.pop_front();
    direction = pathInfo.front();
    currentRowCol = getAfterRowCol(currentRowCol, direction);  
    map.getTerrainAtLocation(currentRowCol, currentTerrain);

    drawLineOnTerrain(currentRowCol); // for map mode 
  } // while watter terrain

  if (showPathMethod == SHOW_PATH_LIST_MODE) 
  {
    cout << "Search:" << '\n';
  } // for list mode

  while (!((currentRowCol.getRow() == map.getTreasureLocation().getRow()) &
           (currentRowCol.getCol() == map.getTreasureLocation().getCol())))
  {
    if (showPathMethod == SHOW_PATH_LIST_MODE) 
    {
      cout << currentRowCol.getRow() << "," << currentRowCol.getCol() << '\n';
    } // for list mode

    pathInfo.pop_front();
    direction = pathInfo.front();
    currentRowCol = getAfterRowCol(currentRowCol, direction);  
    map.getTerrainAtLocation(currentRowCol, currentTerrain);

    if (pathInfo.size() != 1)
    {
      drawLineOnTerrain(currentRowCol); 
    } // for map mode   
  }

  if (showPathMethod == SHOW_PATH_LIST_MODE) 
  {
    cout << currentRowCol.getRow() << "," << currentRowCol.getCol() << '\n';
  } // for list mode

  if (showPathMethod == SHOW_PATH_MAP_MODE)
  {
    currentRowCol = map.getStartLocation();
    map.getTerrainAtLocation(currentRowCol, currentTerrain);
    currentTerrain.setTerrainCharacter(START_TERRAIN);
    map.setTerrainAtLocation(currentRowCol, currentTerrain);  
    
    currentRowCol = map.getTreasureLocation();
    map.getTerrainAtLocation(currentRowCol, currentTerrain);
    currentTerrain.setTerrainCharacter(TREASURE_TERRAIN);
    map.setTerrainAtLocation(currentRowCol, currentTerrain);  
    
    for (uint32_t rInd = 0; rInd < map.getSize(); rInd++)
    {
      for (uint32_t cInd = 0; cInd < map.getSize(); cInd++)
      {
        currentRowCol.setRowCol(rInd, cInd);
        map.getTerrainAtLocation(currentRowCol, currentTerrain);

        cout << currentTerrain.getCharacter();    
      } // for cInd

      cout << '\n';
    } // for rInd 
  }
} 

// TODO: delete if these are not needed anymore
// RowColumnClass HuntSimulationClass::getSailLocation() const
// {
//   return sailLocation;  
// }

// RowColumnClass HuntSimulationClass::getSearchLocation() const
// {
//   return searchLocation;  
// }

// TreasureMap HuntSimulationClass::getMap() const
// {
//   return map;  
// }

// deque<RowColumnClass> HuntSimulationClass::getSearchContainer() const
// {
//   return searchContainer;  
// }

// void HuntSimulationClass::printPathInfo() const
// {
//   cout << "front -- ";
//   for (uint32_t i = 0; i < pathInfo.size(); i++)
//   {
//     cout << pathInfo[i] << " -- ";
//   }    
//   cout << "back" << '\n';
// }

// char HuntSimulationClass::getShowPathOption() const
// {
//   return showPathMethod;  
// }

// // Print option arguments read by command line
// void HuntSimulationClass::printOptionArgument() const
// {
//   cout << "===== Simulation control parameters =====" << '\n';
//   cout << "  Is showing verbose: " << isVerbose << '\n';
//   cout << "  Is showing stats: " << isStats << '\n';
//   cout << "  Sail container structure: " << sailContainerStructure << '\n';
//   cout << "  Search container structure: " << searchContainerStructure << '\n';
//   cout << "  Hunt order: " << huntOrder << '\n';
//   cout << "  Show path option: " << showPathMethod << '\n';
//   cout << '\n';
// }

// void HuntSimulationClass::printSimulationExecutionAttributes() const
// {
//   cout << "===== Simulation execution attributes =====" << '\n';
//   cout << "  sailLocation: ";
//   sailLocation.printRowCol();
//   cout << '\n';
//   cout << '\n';

//   cout << "  sailContainer: " << '\n';
//   if (sailContainer.size() != 0)
//   {
//     printSailContainer();
//   }
//   cout << '\n';

//   cout << "  Water locations investigated: " << numOfWaterInvestigated << '\n';
//   cout << '\n';

//   cout << "  searchLocation: ";
//   searchLocation.printRowCol();
//   cout << '\n';
//   cout << '\n';
  
//   cout << "  searchContainer: " << '\n';
//   if (searchContainer.size() != 0)
//   {
//     printSearchContainer();
//   }
//   cout << '\n';

//   cout << "  Land locations investigated: " << numOfLandInvestigated << '\n';
//   cout << '\n';
// }

// void HuntSimulationClass::printSailContainer() const
// {
//   size_t size;

//   size = sailContainer.size();
  
//   cout << "front -- ";
//   for (size_t i = 0; i < size; i++)
//   {
//     sailContainer[i].printRowCol();
//     cout << " -- ";    
//   }
//   cout << "back" << '\n';
// }

// void HuntSimulationClass::printSearchContainer() const
// {
//   size_t size;

//   size = searchContainer.size();
  
//   cout << "front -- ";
//   for (size_t i = 0; i < size; i++)
//   {
//     searchContainer[i].printRowCol();
//     cout << " -- ";    
//   }
//   cout << "back" << '\n';  
// }