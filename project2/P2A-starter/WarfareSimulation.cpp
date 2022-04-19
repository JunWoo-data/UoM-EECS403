// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include <iostream>
#include <getopt.h>
#include "WarfareSimulation.h"
#include "P2random.h"
#include "GeneralDeploymentClass.h"
#include "GeneralClass.h"
#include "constants.h"
using namespace std;

void WarefareSimulation::get_options(int argc, char** argv)
{
  int option_index = 0, option = 0;  
  opterr = false;

  struct option longOpts[] = {{ "verbose", no_argument, nullptr, 'v' },
                              { "median", no_argument, nullptr, 'm' },
                              { "general-eval", no_argument, nullptr, 'g' },
                              { "watcher", no_argument, nullptr, 'w' },
                              { nullptr, 0, nullptr, '\0' }};

  while ((option = getopt_long(argc, argv, "vmgw", 
          longOpts, &option_index)) != -1)
  {
    switch (option)
    {
      case 'v':
      {
        isVerbose = true;
        break;      
      } // case v

      case 'm':
      {
        isMedian = true;
        break;    
      } // case m

      case 'g':
      {
        isGeneralEval = true;
        break;
      } // case g

      case 'w':
      {
        isWatcher = true;    
        break;
      } // case w

      default:
      {
        cerr << "ERROR: Unknown command line option." << '\n'
             << "Usage: ./galaxy\n\t[--verbose | -v]\n"
             <<                  "\t[--median | -m]\n"
             <<                  "\t[--general-eval | -g]\n"
             <<                  "\t[--wathcer | -w]\n"
             <<                  "\t< <TXT input file>\'" << '\n';
        std::exit(1);      
      }
    }
  }  
}

void WarefareSimulation::readSimulationInput()
{
  string junk;
  string input;
  
  // read comment line
  getline(cin, junk);
  
  // read simulation related variables
  cin >> junk >> input;
  if (input == "DL")
  {
    inputMode = 'D';    
  }

  else
  {
    inputMode = 'P';    
  }

  cin >> junk >> numOfGenerals;
  if (isGeneralEval)
  {
    generalList.resize(numOfGenerals);
  }

  cin >> junk >> numOfPlanets;
  planetList.resize(numOfPlanets);
  
  if (inputMode == 'P')
  {
    cin >> junk >> randomSeed;
    cin >> junk >> numOfDeployments;
    cin >> junk >> arrivalRate;    
    P2random::PR_init(ss, randomSeed, numOfGenerals, numOfPlanets, 
                      numOfDeployments, arrivalRate);
  }
}

void WarefareSimulation::runSimulation()
{
  int inTimestampInt;
  string inSide;
  string inGeneral;
  uint32_t inGeneralUint;
  string inPlanet;
  uint32_t inPlanetUint;
  string inForceSensitive;
  int inForceSensitivityInt;
  string inNumOfTroops; 
  int inNumOfTroopsInt; 
  int inDeployOrderInt = 0;
  istream &inputStream = inputMode == 'P' ? ss : cin;
  bool isTimeChanged;

  std::cout << "Deploying troops..." << '\n';

  while(inputStream >> inTimestampInt >> inSide >> inGeneral >> inPlanet >> 
        inForceSensitive >>  inNumOfTroops)  
  { 
    // error checking for non decreasing timestamp 
    if (currentTimestamp > inTimestampInt)
    { 
      cerr << "ERROR: Invalid decreasing timestamp" << '\n';
      std::exit(1);
    }

    inGeneral = inGeneral.erase(0, 1);
    stringstream ss_general(inGeneral);
    ss_general >> inGeneralUint;

    inPlanet = inPlanet.erase(0, 1);
    stringstream ss_planet(inPlanet);
    ss_planet >> inPlanetUint;

    inForceSensitive = inForceSensitive.erase(0, 1);
    stringstream ss_force(inForceSensitive);
    ss_force >> inForceSensitivityInt;

    inNumOfTroops = inNumOfTroops.erase(0, 1);
    stringstream ss_troop(inNumOfTroops);
    ss_troop >> inNumOfTroopsInt;
    
    checkInputError(inGeneralUint, inPlanetUint, inForceSensitivityInt, 
                    inNumOfTroopsInt);
    
    if (currentTimestamp != inTimestampInt)
    {
      isTimeChanged = true;  
    }

    else
    {
      isTimeChanged = false;  
    }

    if (isTimeChanged & isMedian)
    {
      for (uint32_t i = 0; i < numOfPlanets; i++)
      {
        if ((planetList[i].upperLostTroops.size() != 0) ||  
            (planetList[i].lowerLostTroops.size() != 0))
        {
          std::cout << "Median troops lost on planet " << i << " at time "
                    << currentTimestamp << " is " << planetList[i].getMedian()
                    << "." << '\n';
        }    
      }      
    }

    currentTimestamp = inTimestampInt;
    currentPlanet = inPlanetUint;
    inDeployOrderInt += 1;
    currentDeploy.setDeployTo(inGeneralUint, inForceSensitivityInt, 
                              inNumOfTroopsInt, currentTimestamp, 
                              inDeployOrderInt);
    
    if ((inSide == "JEDI") & isGeneralEval)
    {
      generalList[inGeneralUint].addTotalJedi(inNumOfTroopsInt);  
    } // if side == jedi & general mode on

    else if ((inSide == "SITH") & isGeneralEval)
    {
      generalList[inGeneralUint].addTotalSith(inNumOfTroopsInt);  
    } // else if side == sith & general mode on
    
    if (isWatcher)
    {
      checkBestAmbush(currentDeploy, inSide);
      checkBestAttack(currentDeploy, inSide);
    }  

    // std::cout << "before match" << endl;
    // cout << "general number:" << inGeneralUint << endl;
    // generalList[inGeneralUint].printGeneral(); 
    matchBattalion(inSide, inGeneralUint);

    // std::cout << "after match" << endl;
    // generalList[inGeneralUint].printGeneral(); 
    
  } // while read each battle line 
  
  if (isMedian)
  {
    for (uint32_t i = 0; i < numOfPlanets; i++)
    {
      if ((planetList[i].upperLostTroops.size() != 0) |  
          (planetList[i].lowerLostTroops.size() != 0))
      {
        std::cout << "Median troops lost on planet " << i << " at time "
                  << currentTimestamp << " is " << planetList[i].getMedian()
                  << "." << '\n';
      } // if median value exist   
    } // for planet      
  } // if median mode

  std::cout << "---End of Day---" << '\n';
  std::cout << "Battles: " << numOfBattles << '\n';

  if (isGeneralEval)
  {
    std::cout << "---General Evaluation---" << '\n';
    for (uint32_t i = 0; i < numOfGenerals; i++)
    {
      std::cout << "General " << i << " deployed " 
                << generalList[i].getTotalJedi() << " Jedi troops and "  
                << generalList[i].getTotalSith() << " Sith troops, and "
                << generalList[i].getTotalJedi() + generalList[i].getTotalSith() 
                   - generalList[i].getTotalDeath() << "/"
                << generalList[i].getTotalJedi() + generalList[i].getTotalSith() 
                << " troops survived." << '\n';  
    } // for general
  } // if general eval mode

  if (isWatcher)
  {   
    std::cout << "---Movie Watcher---" << '\n';
    for (uint32_t i = 0; i < numOfPlanets; i++)
    {
      // print ambush mode
      if ((planetList[i].ambushModeState == State::Initial) || 
          (planetList[i].ambushModeState == State::SeenOne))
      {
        std::cout << "A movie watcher would enjoy an ambush on planet " << i 
                  << " with Sith at time -1 and Jedi at time -1 with a force "
                  << "difference of 0." << '\n';
      } // if ambush mode state == initial or seen one (no pairs)

      else
      {
        std::cout << "A movie watcher would enjoy an ambush on planet " << i 
                  << " with Sith at time " 
                  << planetList[i].ambushModeBest[BEST_SITH_INDEX].getTimestamp()
                  << " and Jedi at time "
                  << planetList[i].ambushModeBest[BEST_JEDI_INDEX].getTimestamp()
                  << " with a force difference of " 
                  << planetList[i].ambushModeBest[BEST_SITH_INDEX].getForceSensitivity() 
                     - planetList[i].ambushModeBest[BEST_JEDI_INDEX].getForceSensitivity()
                  << "." << '\n';  
      } // else if there are pairs     
      
      // print attack mode
      if ((planetList[i].attackModeState == State::Initial) || 
          (planetList[i].attackModeState == State::SeenOne))
      {
        std::cout << "A movie watcher would enjoy an attack on planet " << i 
                  << " with Jedi at time -1 and Sith at time -1 with a force "
                  << "difference of 0." << '\n';
      } // if attack mode state == initial or seen one (no pairs)

      else
      {
        std::cout << "A movie watcher would enjoy an attack on planet " << i 
                  << " with Jedi at time " 
                  << planetList[i].attackModeBest[BEST_JEDI_INDEX].getTimestamp()
                  << " and Sith at time "
                  << planetList[i].attackModeBest[BEST_SITH_INDEX].getTimestamp()
                  << " with a force difference of " 
                  << planetList[i].attackModeBest[BEST_SITH_INDEX].getForceSensitivity() 
                     - planetList[i].attackModeBest[BEST_JEDI_INDEX].getForceSensitivity()
                  << "." << '\n';  
      } // else if there are pairs      
    } // for all planet
  } // if movie watcher mode
} // runs simulation function

bool WarefareSimulation::checkInputError(uint32_t inGeneral, uint32_t inPlanet, 
                                         int inForce, int inTroops)
{
  if ((int(inGeneral) < 0) | (inGeneral >= numOfGenerals))
  {
    cerr << "ERROR: Invalid general ID" << '\n';
    std::exit(1);
  } // num of general check

  else if ((int(inPlanet) < 0) | (inPlanet >= numOfPlanets))
  {
    cerr << "ERROR: Invalid planet ID" << '\n';
    std::exit(1);
  } // num of planets check

  else if (inForce <= 0) 
  {
    cerr << "ERROR: Invalid force sensntivity level" << '\n';
    std::exit(1);
  } // force sensitivity check

  else if (inTroops <= 0)       
  {
    cerr << "ERROR: Invalid number of troops" << '\n';  
    std::exit(1);
  } // num of troops check

  else
  {
    return true;  
  } // else
}

void WarefareSimulation::matchBattalion(string currentSide, 
                                        uint32_t currentGeneral)                                       
{
  GeneralDeploymentClass currentOppositeDeploy;
  int smallerNumOfTroops;
                       
  if (currentSide == "SITH")
  {
    // std::cout << "-- before fight" << endl;
    // std::cout << "currentPlanet: " << currentPlanet << endl;
    // std::cout << "jedi size: " << planetList[currentPlanet].jediPQ.size() << endl;
    // std::cout << "sith size: " << planetList[currentPlanet].sithPQ.size() << endl;

    // std::cout << "current general" << endl;
    // currentDeploy.printDeploy();
    

    if (planetList[currentPlanet].jediPQ.size() != 0)
    {
      currentOppositeDeploy = planetList[currentPlanet].jediPQ.top();
      // std::cout << "current opposite general" << endl;
      // currentOppositeDeploy.printDeploy();

      if (currentOppositeDeploy.getForceSensitivity() 
          <= currentDeploy.getForceSensitivity())
      {
        numOfBattles += 1;

        if (isVerbose)
        {
          printVerbose(currentDeploy, currentOppositeDeploy);  
        }
        
        smallerNumOfTroops = min(currentDeploy.getNumOfTroops(), 
                                 currentOppositeDeploy.getNumOfTroops());

        currentDeploy.setNumOfTroops(
          currentDeploy.getNumOfTroops() - smallerNumOfTroops); 
          
        currentOppositeDeploy.setNumOfTroops(
          currentOppositeDeploy.getNumOfTroops() - smallerNumOfTroops);
        
        if (isGeneralEval)
        {
          generalList[currentDeploy.getGeneralID()].addTotalDeath(smallerNumOfTroops);
          generalList[currentOppositeDeploy.getGeneralID()].addTotalDeath(smallerNumOfTroops);  
        }

        if (isMedian)
        {
          planetList[currentPlanet].pushLostTroops(smallerNumOfTroops * 2);  
        }
        
        // std::cout << "-- after fight" << endl;     
        // std::cout << "current general" << endl;
        // currentDeploy.printDeploy(); 
        // std::cout << "current opposite general" << endl;
        // currentOppositeDeploy.printDeploy();

        if (currentDeploy.getNumOfTroops() > 0)
        {
          planetList[currentPlanet].jediPQ.pop();  
          matchBattalion(currentSide, currentGeneral);
        } // if current.troops > 0

        else if (currentOppositeDeploy.getNumOfTroops() > 0)
        {
          planetList[currentPlanet].jediPQ.pop();
          planetList[currentPlanet].jediPQ.push(currentOppositeDeploy);  
        } // else if opposite.troops > 0

        else
        {
          planetList[currentPlanet].jediPQ.pop();  
        } // else if current.troops == opposite.troops == 0
      } // if sith.force >= jedi.force (fight case)  

      else
      {
        if (currentDeploy.getNumOfTroops() > 0)
        {
          planetList[currentPlanet].sithPQ.push(currentDeploy);  
        } // if current.troops > 0
      } // if sith.force < jedi.force (not fight case)
    } // if opposite side PQ.size() != 0
    
    else
    { 
      planetList[currentPlanet].sithPQ.push(currentDeploy);  
    } // if opposite side PQ.size() == 0
  } // if current == sith side

  else
  {
    // std::cout << "-- before fight" << endl;
    // std::cout << "currentPlanet: " << currentPlanet << endl;
    // std::cout << "jedi size: " << planetList[currentPlanet].jediPQ.size() << endl;
    // std::cout << "sith size: " << planetList[currentPlanet].sithPQ.size() << endl;

    // std::cout << "current general" << endl;
    // currentDeploy.printDeploy();
    
    if (planetList[currentPlanet].sithPQ.size() != 0)
    {
      currentOppositeDeploy = planetList[currentPlanet].sithPQ.top();
      // std::cout << "current opposite general" << endl;
      // currentOppositeDeploy.printDeploy();

      if (currentDeploy.getForceSensitivity() 
          <= currentOppositeDeploy.getForceSensitivity())
      {
        numOfBattles += 1;

        if (isVerbose)
        {
          printVerbose(currentOppositeDeploy, currentDeploy);  
        }

        smallerNumOfTroops = min(currentDeploy.getNumOfTroops(), 
                                 currentOppositeDeploy.getNumOfTroops());

        currentDeploy.setNumOfTroops(
          currentDeploy.getNumOfTroops() - smallerNumOfTroops); 
          
        currentOppositeDeploy.setNumOfTroops(
          currentOppositeDeploy.getNumOfTroops() - smallerNumOfTroops);
        
        if (isGeneralEval)
        {
          generalList[currentDeploy.getGeneralID()].addTotalDeath(smallerNumOfTroops);
          generalList[currentOppositeDeploy.getGeneralID()].addTotalDeath(smallerNumOfTroops);   
        }

        if (isMedian)
        {
          planetList[currentPlanet].pushLostTroops(smallerNumOfTroops * 2);  
        }
         
        // std::cout << "-- after fight" << endl;     
        // std::cout << "current general" << endl;
        // currentDeploy.printDeploy(); 
        // std::cout << "current opposite general" << endl;
        // currentOppositeDeploy.printDeploy();   
        
        if (currentDeploy.getNumOfTroops() > 0)
        {
          planetList[currentPlanet].sithPQ.pop();  
          matchBattalion(currentSide, currentGeneral);
        } // if current.troops > 0

        else if (currentOppositeDeploy.getNumOfTroops() > 0)
        {
          planetList[currentPlanet].sithPQ.pop();
          planetList[currentPlanet].sithPQ.push(currentOppositeDeploy);  
        } // else if opposite.troops > 0

        else
        {
          planetList[currentPlanet].sithPQ.pop();  
        } // else if current.troops == opposite.troops == 0
      } // if sith.force >= jedi.force (fight case)  

      else
      {
        if (currentDeploy.getNumOfTroops() > 0)
        {
          planetList[currentPlanet].jediPQ.push(currentDeploy);  
        } // if current.troops > 0
      } // if sith.force < jedi.force (not fight case)
    } // if opposite side PQ.size() != 0
    
    else
    { 
      planetList[currentPlanet].jediPQ.push(currentDeploy);  
    } // if opposite side PQ.size() == 0
  } // if current == jeldi side
}

void WarefareSimulation::printVerbose(GeneralDeploymentClass currentSith, 
                                      GeneralDeploymentClass currentJedi) const
{
  
  std::cout << "General " << currentSith.getGeneralID() << "'s battalion "
            << "attacked General " << currentJedi.getGeneralID() << "'s "
            << "battalion on planet " << currentPlanet << ". "
            << min(currentSith.getNumOfTroops(), currentJedi.getNumOfTroops()) 
               * 2
            << " troops were lost." << '\n';
}

void WarefareSimulation::checkBestAttack(GeneralDeploymentClass currentDeploy, 
                                         string currentSide)
{
  if (planetList[currentPlanet].attackModeBest.size() == 0)
  {
    planetList[currentPlanet].attackModeBest.resize(3);
    
    if (currentSide == "JEDI")
    {
      planetList[currentPlanet].attackModeState = State::SeenOne;
      planetList[currentPlanet].attackModeBest[BEST_JEDI_INDEX] = currentDeploy;
    } // if current side == jedi

    else
    {
      planetList[currentPlanet].attackModeState = State::Initial;
    } // else current side == sith
  } // if attackModeBest size == 0

  else if ((planetList[currentPlanet].attackModeState == State::Initial) &&
           (currentSide == "JEDI"))
  {
    planetList[currentPlanet].attackModeState = State::SeenOne;
    planetList[currentPlanet].attackModeBest[BEST_JEDI_INDEX] = currentDeploy; 
  } // else if initial state && see jedi            

  else if (planetList[currentPlanet].attackModeState == State::SeenOne)
  {
    if (currentSide == "JEDI")
    {
      if (planetList[currentPlanet].attackModeBest[BEST_JEDI_INDEX].getForceSensitivity()
          > currentDeploy.getForceSensitivity())
      {
        planetList[currentPlanet].attackModeBest[BEST_JEDI_INDEX] = currentDeploy;    
      } // if see better jedi  
    } // if seen one state && see jedi

    else
    {
      planetList[currentPlanet].attackModeState = State::SeenBoth;
      planetList[currentPlanet].attackModeBest[BEST_SITH_INDEX] = currentDeploy;    
    } // if seen one state && see sith
  } // else if seen one state

  else if (planetList[currentPlanet].attackModeState == State::SeenBoth)
  {
    if (currentSide == "JEDI")
    {
      if (planetList[currentPlanet].attackModeBest[BEST_JEDI_INDEX].getForceSensitivity()
          > currentDeploy.getForceSensitivity())
      {
        planetList[currentPlanet].attackModeState = State::MaybeBetter;
        planetList[currentPlanet].attackModeBest[MAYBE_BEST_JEDI_INDEX] = currentDeploy;    
      } // if see better jedi    
    } // if seen both state && see jedi

    else
    {
      if (planetList[currentPlanet].attackModeBest[BEST_SITH_INDEX].getForceSensitivity()
          < currentDeploy.getForceSensitivity())
      {
        planetList[currentPlanet].attackModeBest[BEST_SITH_INDEX] = currentDeploy;    
      } // if see better sith    
    } // if seen both state && see sith  
  } // else if seen both state

  else if (planetList[currentPlanet].attackModeState == State::MaybeBetter)
  {
    if (currentSide == "JEDI")
    {
      if (planetList[currentPlanet].attackModeBest[MAYBE_BEST_JEDI_INDEX].getForceSensitivity()
          > currentDeploy.getForceSensitivity())
      {
        planetList[currentPlanet].attackModeBest[MAYBE_BEST_JEDI_INDEX] = currentDeploy;    
      } // if see better jedi      
    } // if maybe better state && see jedi

    else
    {
      if ((planetList[currentPlanet].attackModeBest[BEST_SITH_INDEX].getForceSensitivity()
           - planetList[currentPlanet].attackModeBest[BEST_JEDI_INDEX].getForceSensitivity())
           < 
          (currentDeploy.getForceSensitivity() 
           - planetList[currentPlanet].attackModeBest[MAYBE_BEST_JEDI_INDEX].getForceSensitivity()))
      {
        planetList[currentPlanet].attackModeState = State::SeenBoth;
        planetList[currentPlanet].attackModeBest[BEST_JEDI_INDEX]
        = planetList[currentPlanet].attackModeBest[MAYBE_BEST_JEDI_INDEX];
        planetList[currentPlanet].attackModeBest[BEST_SITH_INDEX] 
        = currentDeploy;   
      } // if see better pair    
    } // if maybe better state && see sith
  } // else if maybe better state
}

void WarefareSimulation::checkBestAmbush(GeneralDeploymentClass currentDeploy, 
                                         string currentSide)
{
  if (planetList[currentPlanet].ambushModeBest.size() == 0)
  {
    planetList[currentPlanet].ambushModeBest.resize(3);
    
    if (currentSide == "SITH")
    {
      planetList[currentPlanet].ambushModeState = State::SeenOne;
      planetList[currentPlanet].ambushModeBest[BEST_SITH_INDEX] = currentDeploy;
    } // if current side == sith

    else
    {
      planetList[currentPlanet].ambushModeState = State::Initial;
    } // else current side == jelda
  } // if attackModeBest size == 0

  else if ((planetList[currentPlanet].ambushModeState == State::Initial) &&
           (currentSide == "SITH"))
  {
    planetList[currentPlanet].ambushModeState = State::SeenOne;
    planetList[currentPlanet].ambushModeBest[BEST_SITH_INDEX] = currentDeploy; 
  } // else if initial state && see sith            

  else if (planetList[currentPlanet].ambushModeState == State::SeenOne)
  {
    if (currentSide == "SITH")
    {
      if (planetList[currentPlanet].ambushModeBest[BEST_SITH_INDEX].getForceSensitivity()
          < currentDeploy.getForceSensitivity())
      {
        planetList[currentPlanet].ambushModeBest[BEST_SITH_INDEX] = currentDeploy;    
      } // if see better sith  
    } // if seen one state && see sith

    else
    {
      planetList[currentPlanet].ambushModeState = State::SeenBoth;
      planetList[currentPlanet].ambushModeBest[BEST_JEDI_INDEX] = currentDeploy;    
    } // if seen one state && see jedi
  } // else if seen one state

  else if (planetList[currentPlanet].ambushModeState == State::SeenBoth)
  {
    if (currentSide == "SITH")
    {
      if (planetList[currentPlanet].ambushModeBest[BEST_SITH_INDEX].getForceSensitivity()
          < currentDeploy.getForceSensitivity())
      {
        planetList[currentPlanet].ambushModeState = State::MaybeBetter;
        planetList[currentPlanet].ambushModeBest[MAYBE_BEST_SITH_INDEX] = currentDeploy;    
      } // if see better sith    
    } // if seen both state && see sith

    else
    {
      if (planetList[currentPlanet].ambushModeBest[BEST_JEDI_INDEX].getForceSensitivity()
          > currentDeploy.getForceSensitivity())
      {
        planetList[currentPlanet].ambushModeBest[BEST_JEDI_INDEX] = currentDeploy;    
      } // if see better jedi    
    } // if seen both state && see jedi  
  } // else if seen both state

  else if (planetList[currentPlanet].ambushModeState == State::MaybeBetter)
  {
    if (currentSide == "SITH")
    {
      if (planetList[currentPlanet].ambushModeBest[MAYBE_BEST_SITH_INDEX].getForceSensitivity()
          < currentDeploy.getForceSensitivity())
      {
        planetList[currentPlanet].ambushModeBest[MAYBE_BEST_SITH_INDEX] = currentDeploy;    
      } // if see better sith      
    } // if maybe better state && see sith

    else
    {
      if ((planetList[currentPlanet].ambushModeBest[BEST_SITH_INDEX].getForceSensitivity()
           - planetList[currentPlanet].ambushModeBest[BEST_JEDI_INDEX].getForceSensitivity())
           < 
          (planetList[currentPlanet].ambushModeBest[MAYBE_BEST_SITH_INDEX].getForceSensitivity())
           - currentDeploy.getForceSensitivity())
      {
        planetList[currentPlanet].ambushModeState = State::SeenBoth;
        planetList[currentPlanet].ambushModeBest[BEST_JEDI_INDEX]
        = currentDeploy;
        planetList[currentPlanet].ambushModeBest[BEST_SITH_INDEX] 
        = planetList[currentPlanet].ambushModeBest[MAYBE_BEST_SITH_INDEX];   
      } // if see better pair    
    } // if maybe better state && see jedi
  } // else if maybe better state

}