// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef _GENERALDEPLOYMENTCLASS_H__
#define _GENERALDEPLOYMENTCLASS_H__

#include <iostream>
using namespace std;

class GeneralDeploymentClass
{
    uint32_t generalID;
    int forceSensitivity;
    int numOfTroops;
    int timestamp;
    int deployOrder;

  public:   
    GeneralDeploymentClass()
    {
      generalID = 0;
      forceSensitivity = 0;
      numOfTroops = 0;
      timestamp = 0;
      deployOrder = 0;    
    }

    void setDeployTo(uint32_t inID, int inForce, int inTroops, int inTime, int inOrder)
    {
      generalID = inID;
      forceSensitivity = inForce;
      numOfTroops = inTroops;
      timestamp = inTime;
      deployOrder = inOrder;
    }

    uint32_t getGeneralID() const
    {
      return generalID;    
    }

    int getForceSensitivity() const
    {
      return forceSensitivity;    
    }

    void setNumOfTroops(int inTroops)
    {
      numOfTroops = inTroops;
    }

    int getNumOfTroops() const
    {
      return numOfTroops;    
    }

    int getTimestamp() const
    {
      return timestamp;    
    }

    int getDeployOrder() const
    {
      return deployOrder;    
    }

    void printDeploy() const
    {
      cout << "-- General info --" << '\n';
      cout << "General ID: " << generalID << '\n';
      cout << "Force sensitivity: " << forceSensitivity << '\n';
      cout << "Number of troops: " << numOfTroops << '\n';
      cout << "Timestamp: " << timestamp << '\n';
      cout << "Deploy order:" << deployOrder << '\n';
      cout << '\n';
    }
};

struct jediSort 
{
  bool operator()(const GeneralDeploymentClass &left, 
                  const GeneralDeploymentClass &right) const
  {
    bool result;

    if (left.getForceSensitivity() > right.getForceSensitivity())
    {
      result = true;    
    } // if left.force > right.force    

    else if (left.getForceSensitivity() == right.getForceSensitivity())
    {
      if (left.getDeployOrder() > right.getDeployOrder())
      {
        result = true;    
      } // if left.order > right.order

      else
      {
        result = false;
      } // else    
    } // lese if left.force == right.force

    else
    {
      result = false;
    } // else 

    return result;
  }    
};

struct sithSort
{
  bool operator()(const GeneralDeploymentClass &left, 
                  const GeneralDeploymentClass &right) const
  {
    bool result;

    if (left.getForceSensitivity() < right.getForceSensitivity())
    {
      result = true;    
    } // if left.force > right.force    

    else if (left.getForceSensitivity() == right.getForceSensitivity())
    {
      if (left.getDeployOrder() > right.getDeployOrder())
      {
        result = true;    
      } // if left.order > right.order

      else
      {
        result = false;
      } // else    
    } // lese if left.force == right.force

    else
    {
      result = false;
    } // else 

    return result;
  }    
};

#endif