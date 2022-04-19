// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef _HUNTSIMULATIONCLASS_H_
#define _HUNTSIMULATIONCLASS_H_

#include "TreasureMap.h"
#include <deque>
#include "RowColumnClass.h"
#include "constants.h"
using namespace std;

//Programmer: Youngjun Woo
//Date: January 23, 2022
//Purpose: This class will act as the basis for the treasure hunt simulation
//         that investigates water and island to find treasure under different 
//         options determined by command line arguments.

/*  [variables]
    
    -- Treasure hunt simulation control parameters 
    bool isVerbose;
    bool isStats;
    char showPathMethod;
    string huntOrder;
    char sailContainerStructure;
    char searchContainerStructure;

    -- Simulation execution attributes
    TreasureMap map;
    deque<RowColumnClass> sailContainer;
    deque<RowColumnClass> searchContainer;
    RowColumnClass sailLocation;
    RowColumnClass searchLocation;
    deque<char> pathInfo;

    -- Statistics-Related attributes
    int numOfWaterInvestigated;
    int numOfLandInvestigated;
    int numOfWentAshore;
    size_t pathLength;

*/


class HuntSimulationClass
{
  private:
    TreasureMap map;
    deque<RowColumnClass> sailContainer;
    deque<RowColumnClass> searchContainer;
    deque<char> pathInfo;
    RowColumnClass sailLocation;
    RowColumnClass searchLocation;
    string huntOrder;
    int numOfWaterInvestigated;
    int numOfLandInvestigated;
    int numOfWentAshore;
    size_t pathLength; // TODO: delete
    char showPathMethod;
    char sailContainerStructure;
    char searchContainerStructure;
    bool isVerbose;
    bool isStats;
    
  public:
    HuntSimulationClass()
    {
      isVerbose = false;
      isStats = false;
      huntOrder = DEFAULT_HUNT_ORDER;
      showPathMethod = SHOW_PATH_NONE_MODE;
      sailContainerStructure = DEFAULT_SAIL_CONTAINER_STRUCTURE;
      searchContainerStructure = DEFAULT_SEARCH_CONTAINER_STRUCTURE;  
      numOfWaterInvestigated = 0;
      numOfLandInvestigated = 0;
      pathLength = 0;
      numOfWentAshore = 0;
    }

    void get_options(int argc, char** argv);
    void readMapFromFile();

    void printSimulationStats(bool isFound) const; 
    
    void setSimulation();
    bool setSailLocation();
    RowColumnClass investigateAdjacent(char direction, 
                                       const RowColumnClass &atRowCol);
    bool checkIsValidLocation(const RowColumnClass &inLocation);
    bool runCaptainHunting();
    
    bool setSearchLocation();
    bool runMateHunting();
    
    bool runHuntProcess();
    
    bool isTreasureLand(const RowColumnClass &inLand);
    RowColumnClass getBeforeRowCol(const RowColumnClass &inRowCol, 
                                   char inDirection);
    RowColumnClass getAfterRowCol(const RowColumnClass &inRowCol, 
                                  char inDirection); 
    void doBackTrace();
    void drawLineOnTerrain(const RowColumnClass &currentRowCol);
    void showPath();
    
    //void printOptionArgument() const; // TODO: delete
    // void printSimulationExecutionAttributes() const; // TODO: delete
    // void printSailContainer() const; // TODO: delete
    // void printSearchContainer() const; // TODO: deletev

    //void printPathInfo() const; // TODO: delete
    //RowColumnClass getSailLocation() const; // TODO: delete
    //RowColumnClass getSearchLocation() const; // TODO: deletev
    //deque<RowColumnClass> getSearchContainer() const; // TODO: delete
    //TreasureMap getMap() const; // TODO: delete
    //char getShowPathOption() const; // TODO: delete
};

#endif