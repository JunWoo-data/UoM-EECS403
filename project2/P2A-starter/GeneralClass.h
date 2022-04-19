// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef _GENERALCLASS_H_
#define _GENERALCLASS_H_

#include <iostream>
using namespace std;

class GeneralClass
{
  private:
    int numOfTotalJedi;
    int numOfTotalSith;
    int numOfTotalDeath;
  
  public:
    GeneralClass()
    {
      numOfTotalJedi = 0;
      numOfTotalSith = 0;
      numOfTotalDeath = 0;    
    }    

    int getTotalJedi() const
    {
      return numOfTotalJedi;     
    }

    void addTotalJedi(int inTotalJedi)
    {
      numOfTotalJedi = numOfTotalJedi + inTotalJedi;
    }

    int getTotalSith() const
    {
      return numOfTotalSith;     
    }

    void addTotalSith(int inTotalSith)
    {
      numOfTotalSith = numOfTotalSith + inTotalSith;
    }

    int getTotalDeath() const
    {
      return numOfTotalDeath;     
    }

    void addTotalDeath(int inTotalDeath)
    {
      numOfTotalDeath = numOfTotalDeath + inTotalDeath;    
    }

    void printGeneral() const
    {
      cout << "Num of total Jedi: " << numOfTotalJedi << '\n';     
      cout << "Num of total Sith: " << numOfTotalSith << '\n';     
      cout << "Num of total death: " << numOfTotalDeath << '\n';     
    }
};

#endif