// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include <iostream>
#include "xcode_redirect.hpp"
#include "WarfareSimulation.h"
#include "P2random.h"
using namespace std;

int main(int argc, char** argv) 
{
  ios_base::sync_with_stdio(false);
  xcode_redirect(argc, argv); 
  
  priority_queue<int> test;

  WarefareSimulation simulation; 
 
  //cout << "<< Get option >>" << endl;
  simulation.get_options(argc, argv); 
  //simulation.printOptions();
  
  //cout << "<< Read simulation input >>" << endl;
  simulation.readSimulationInput();
  //simulation.printSimulationInput();    
  
  // cout << "<< Print random test file >>" << endl;
  //simulation.printRandomTestfile();

  //cout << "<< Start simulation >>" << endl;
  simulation.runSimulation(); 
}
