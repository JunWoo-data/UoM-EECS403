// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#include <iostream>
#include "constants.h"
using namespace std;

//Programmer: Youngjun Woo
//Date: January 23, 2022
//Purpose: This class describe the attribute of each pixel of the map.
//         Character shows that wheter this terrain is 'land' or 'water' or 
//         'impassable'. Also, if the terrain has discovered before, then 
//         isDiscovered will be true.

class TerrainClass
{
  private:
    char character;
    char investigatedDirection;
    bool isDiscovered;
    
  public:
    TerrainClass()
    {
      character = WATER_TERRAIN;
      isDiscovered = false;       
    }

    void setTerrainTo(char inChar, bool inIsDiscovered, char inDirection)
    {
      character = inChar;
      isDiscovered = inIsDiscovered;
      investigatedDirection = inDirection;  
    }

    void setTerrainCharacter(char inChar)
    {
      character = inChar;   
    }

    char getCharacter() const
    {
      return character;  
    }

    bool getIsDiscovered() const
    {
      return isDiscovered;  
    }

    char getInvestigatedDirection() const
    {
      return investigatedDirection;   
    }

    void printTerrain() const;
};

#endif
