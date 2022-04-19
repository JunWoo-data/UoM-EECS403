// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef _PLANETCLASS_H_
#define _PLANETCLASS_H_

#include <iostream>
#include <queue>
#include <vector>
#include "GeneralDeploymentClass.h"
using namespace std;

enum class State {Initial, SeenOne, SeenBoth, MaybeBetter};

class PlanetClass
{   
  public:    
    vector<GeneralDeploymentClass> attackModeBest; // [0]: track best jedi
                                                   // [1]: track best sith
                                                   // [2]: track maybe best jedi
    vector<GeneralDeploymentClass> ambushModeBest; // [0]: track best jedi
                                                   // [1]: track best sith
                                                   // [2]: track maybe best sith 
    priority_queue<GeneralDeploymentClass, 
                   vector<GeneralDeploymentClass>, jediSort> jediPQ;
    priority_queue<GeneralDeploymentClass, 
                   vector<GeneralDeploymentClass>, sithSort> sithPQ;
    priority_queue<int, vector<int>, greater<int>> upperLostTroops;
    priority_queue<int, vector<int>, less<int>> lowerLostTroops;
    State ambushModeState;
    State attackModeState;               
    void pushLostTroops(int inNumTroops);
    int getMedian() const;  
};

#endif 