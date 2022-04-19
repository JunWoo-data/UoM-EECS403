// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef _TREASUREMAP_H_
#define _TREASUREMAP_H_

#include <iostream>
#include "RowColumnClass.h"
#include "TerrainClass.h"
#include <vector>
using namespace std;

//Programmer: Youngjun Woo
//Date: January 23, 2022
//Purpose: This class describe the attribute of the treasure map such as 
//         map size, start location, treasure location... The mapVector is 
//         consist of 2D vector.

class TreasureMap
{
  private:
    vector<vector<TerrainClass>> mapVector;
    RowColumnClass startLocation;
    RowColumnClass treasureLocation;
    uint32_t mapSize;
    
  public:
    TreasureMap()
    {
      RowColumnClass defaultRowCol;

      setSizeTo(DEFAULT_MAP_SIZE);  
      setStartLocation(defaultRowCol);
      setTreasureLocation(defaultRowCol);  
    }

    void setSizeTo(uint32_t inSize)
    {
      mapSize = inSize;   
    }

    uint32_t getSize()
    {
      return mapSize;      
    }

    void setStartLocation(const RowColumnClass &inLocation)
    {
      startLocation = inLocation;   
    }

    RowColumnClass getStartLocation() const
    {
      return startLocation;   
    }

    void setTreasureLocation(const RowColumnClass &inLocation)
    {
      treasureLocation = inLocation;  
    }

    RowColumnClass getTreasureLocation() const
    {
      return treasureLocation;  
    }

    void setTerrainAtLocation(const RowColumnClass &inLocation, 
                              const TerrainClass &inTerrain)
    {
      mapVector[inLocation.getRow()][inLocation.getCol()] = inTerrain; 
    } 

    void getTerrainAtLocation(const RowColumnClass &inLocation, 
                              TerrainClass &outTerrain)
    {
      outTerrain = mapVector[inLocation.getRow()][inLocation.getCol()];  
    }                          
                            
    void printTreasureMap() const;
    void resizeMapVector(uint32_t inSize);
};

#endif