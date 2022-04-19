// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef _WARFARESIMULATION_H_
#define _WARFARESIMULATION_H_

#include <iostream>
#include <sstream>
#include "PlanetClass.h"
#include "GeneralDeploymentClass.h"
#include "GeneralClass.h"
using namespace std;

class WarefareSimulation 
{
  private:
    // Simulation related variables
    vector<PlanetClass> planetList;
    vector<GeneralClass> generalList;
    GeneralDeploymentClass currentDeploy;
    stringstream ss;
    uint32_t numOfGenerals;
    uint32_t numOfPlanets;
    uint32_t randomSeed;
    uint32_t numOfDeployments;
    uint32_t arrivalRate;
    uint32_t currentPlanet;
    int currentTimestamp;
    int numOfBattles;
    char inputMode;

    // Option related variables
    bool isVerbose;
    bool isMedian;
    bool isWatcher;
    bool isGeneralEval;

  public:
    WarefareSimulation()
    {
      isVerbose = false;
      isMedian = false;
      isWatcher = false;
      isGeneralEval = false;  
      randomSeed = 0;
      numOfDeployments = 0;
      arrivalRate = 0;
      currentTimestamp = 0;
      numOfBattles = 0;
    }
    void get_options(int argc, char** argv);
    void printOptions() const
    {
      cout << "-- Command line option -- " << '\n';
      cout << "Is verbose mode: " << isVerbose << '\n';
      cout << "Is median mode: " << isMedian << '\n';
      cout << "Is general eval mode:" << isGeneralEval << '\n';
      cout << "Is watcher mode: " << isWatcher << '\n';    
      cout << '\n';   
    }       
    void readSimulationInput();
    void printSimulationInput() const
    {
      cout << "-- Simulation input variables -- " << '\n';   
      cout << "Input mode: " << inputMode << '\n';  
      cout << "Num of generals: " << numOfGenerals << '\n';
      cout << "Num of planets: " << numOfPlanets << '\n';
      cout << "Random seed: " << randomSeed << '\n';
      cout << "Num of deployments: " << numOfDeployments << '\n';
      cout << "Arrival rates: " << arrivalRate << '\n';
      cout << "Number of total battles: " << numOfBattles << '\n'; 
      cout << "Current timestamp: " << currentTimestamp << '\n';
      cout << '\n';  
    }

    vector<PlanetClass> getPlanetList() const
    {
      return planetList;  
    }
    
    bool checkInputError(uint32_t inGeneral, uint32_t inPlanet, int inForce, 
                         int inTroops);
    void matchBattalion(string currentSide, uint32_t currentGeneral);
    void printVerbose(GeneralDeploymentClass currentSith, 
                      GeneralDeploymentClass currentJedi) const;
    void checkBestAttack(GeneralDeploymentClass currentDeploy, 
                         string currentSide);
    void checkBestAmbush(GeneralDeploymentClass currentDeploy, 
                         string currentSide);                     
    void runSimulation();
    void printRandomTestfile() const
    {
      cout << ss.str();
    }
};

#endif