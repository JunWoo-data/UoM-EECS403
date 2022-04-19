// identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <iostream>
#include "xcode_redirect.hpp"
#include "HuntSimulationClass.h"
#include "TreasureMap.h"
#include "RowColumnClass.h"
using namespace std;



int main(int argc, char** argv)
{
  ios_base::sync_with_stdio(false);
  xcode_redirect(argc, argv);

  HuntSimulationClass treasureHunt;   
  
  // get argument
  //cout << "<< get option from command line >>" << endl;
  treasureHunt.get_options(argc, argv);
  //treasureHunt.printOptionArgument();
  
  // read map from txt file
  //cout << "<< read map from txt file >>" << endl;
  treasureHunt.readMapFromFile();
  //treasureHunt.getMap().printTreasureMap();
  
  // start simulation
  treasureHunt.setSimulation();  
  treasureHunt.runHuntProcess();
  //treasureHunt.getMap().printTreasureMap();
  //treasureHunt.printPathInfo();
}



