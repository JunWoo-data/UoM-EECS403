// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <iostream>
#include "TerrainClass.h"
using namespace std;

//Programmer: Youngjun Woo
//Date: January 23, 2022
//Purpose: This class describe the attribute of each pixel of the map.
//         Character shows that wheter this terrain is 'land' or 'water' or 
//         'impassable'. Also, if the terrain has discovered before, then 
//         isDiscovered will be true.

void TerrainClass::printTerrain() const
{
  cout << "(" << getCharacter() << ", " << getIsDiscovered() << ", "
       << getInvestigatedDirection() << ")";
}