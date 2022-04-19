// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <iostream>
#include "TreasureMap.h"
#include "RowColumnClass.h"
#include "TerrainClass.h"
#include "constants.h"
#include <string>
using namespace std;

//Programmer: Youngjun Woo
//Date: January 23, 2022
//Purpose: This class describe the attribute of the treasure map such as 
//         map size, start location, treasure location... Map is consist of 
//         2d vector.

void TreasureMap::printTreasureMap() const
{
  cout << "=== Print treasure map ===" << '\n';
  cout << "- Map size: " << mapSize << '\n';
  cout << "- Start location: " ;
  startLocation.printRowCol();
  cout << '\n';
  cout << "- Treasure location: " ;
  treasureLocation.printRowCol();
  cout << '\n';
  cout << "- Map vector: " << '\n';

  for (uint32_t rInd = 0; rInd < mapSize; rInd++)
  {
    for (uint32_t cInd = 0; cInd < mapSize; cInd++)
    {
      mapVector[rInd][cInd].printTerrain();
    } // col   
    
    cout << '\n';  
  } // row
  cout << '\n';  
}

void TreasureMap::resizeMapVector(uint32_t inSize)
{
  mapVector.resize(inSize, vector<TerrainClass>(inSize));
}