// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include <iostream>
#include "PlanetClass.h"
using namespace std;

void PlanetClass::pushLostTroops(int inNumTroops)
{
  // Push number to the property queue  
  if ((lowerLostTroops.size() == 0) && (upperLostTroops.size() == 0))
  {
    lowerLostTroops.push(inNumTroops);    
  }

  else if ((upperLostTroops.size() > 0) && 
           (inNumTroops >= upperLostTroops.top()))
  {
    upperLostTroops.push(inNumTroops);    
  }    

  else
  {
    lowerLostTroops.push(inNumTroops);    
  }

  // Rebalance
  if (upperLostTroops.size() - lowerLostTroops.size() == 2)
  {
    lowerLostTroops.push(upperLostTroops.top());
    upperLostTroops.pop();    
  }

  else if (lowerLostTroops.size() - upperLostTroops.size() == 2)
  {
    upperLostTroops.push(lowerLostTroops.top());
    lowerLostTroops.pop();    
  }
}

int PlanetClass::getMedian() const
{
  int median;

  if (upperLostTroops.size() == lowerLostTroops.size())
  {
    median = ((upperLostTroops.top() + lowerLostTroops.top())/2);   
    return  median;
  }    

  else if (upperLostTroops.size() > lowerLostTroops.size())
  {
    median = upperLostTroops.top();
    return median;    
  }

  else
  {
    median = lowerLostTroops.top();
    return median;    
  }
}