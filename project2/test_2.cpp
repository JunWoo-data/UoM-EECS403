// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882


#include <iostream>
#include "UnorderedPQ.h"
#include "SortedPQ.h"
#include "BinaryPQ.h"
#include <vector>
#include <algorithm>
using namespace std;

void fixUp(vector<int> &data, size_t k)
{
  while ((k > 0) && (data[(k - 1) / 2] < data[k]))
  {
    swap(data[k], data[(k - 1) / 2]);
    k = (k - 1) / 2;        
  }    
} 

// void fixDown(vector<int> &data, size_t k)
// {
//   while(((2 * k) + 1) <= data.size())
//   {
//     size_t j = (2 * k) + 1;
//     if ((j < data.size()) && (data[j] < data[j + 1]))
//     {
//       ++j;    
//     }    

//     if (data[k] >= data[j])
//     {
//       break;    
//     }

//     swap(data[k], data[j]);
//     k = j; 
//   }      
// }

void fixDown(vector<int> &data, size_t k)
{
  size_t maxIndex = k;

  size_t l = (k * 2) + 1;
  if ((l <= data.size()) && (data[l] > data[maxIndex]))
  {
    maxIndex = l;    
  }   

  size_t r = (k * 2) + 2;
  if ((r <= data.size()) && (data[r] > data[maxIndex]))
  {
    maxIndex = r;    
  } 

  if (k != maxIndex)
  {
    swap(data[k], data[maxIndex]);
    fixDown(data, maxIndex);    
  }  

}


void myPush(vector<int> &data, int inVal)
{
  data.push_back(inVal);
  fixUp(data, data.size() - 1);    
}

void myPop(vector<int> &data)
{
  swap(data[0], data[data.size() - 1]);
  data.pop_back();
  fixDown(data, 0);    
}

void printVector(vector<int> data)
{
  cout << "Firt-";    
  for (size_t i = 0; i < data.size(); i++)
  {
    std::cout << data[i] << "-";     
  }    
  cout << "End" << endl;   
}

int main()
{
  UnorderedPQ<int> testUnorderedPQ;
  SortedPQ<int> testSortedPQ;
  BinaryPQ<int> testBinaryPQ;
  

  cout << "<< UnorderedPQ test >>" << endl;
  cout << testUnorderedPQ.size() << endl; 
  cout << endl;

  cout << "<< SortedPQ test >>" << endl;
  cout << "size: " << testSortedPQ.size() << endl; 
  cout << "print element: " << endl;
  testSortedPQ.printElement();
  cout << "insert 3" << endl;
  testSortedPQ.push(3);
  testSortedPQ.printElement();
  cout << "insert 9" << endl;
  testSortedPQ.push(9);
  testSortedPQ.printElement();
  cout << "insert 5" << endl;
  testSortedPQ.push(5);
  testSortedPQ.printElement(); 
  cout << "insert 1" << endl;
  testSortedPQ.push(1);
  testSortedPQ.printElement();
  cout << "pop" << endl;
  testSortedPQ.pop();
  testSortedPQ.printElement();
  cout << "top: " << testSortedPQ.top() << endl;
  cout << endl;

  cout << "<< BinaryPQ test >>" << endl;
  cout << "size: " << testBinaryPQ.size() << endl; 
//   cout << "print element: " << endl;
//   testBinaryPQ.printElement();
//   cout << "insert 3" << endl;
//   testSortedPQ.push(3);
//   testSortedPQ.printElement();
//   cout << "insert 9" << endl;
//   testSortedPQ.push(9);
//   testSortedPQ.printElement();
//   cout << "insert 5" << endl;
//   testSortedPQ.push(5);
//   testSortedPQ.printElement();  
//   cout << "insert 1" << endl;
//   testSortedPQ.push(1);
//   testSortedPQ.printElement();
//   cout << "pop" << endl;
//   testSortedPQ.pop();
//   testSortedPQ.printElement();
//   cout << "top: " << testSortedPQ.top() << endl;
  cout << endl;




  cout << "<< Vector test >>" << endl;
  vector<int> testVector;
  testVector.push_back(81);
  testVector.push_back(18);
  testVector.push_back(14);
  testVector.push_back(9);
  testVector.push_back(7);
  testVector.push_back(1);
  testVector.push_back(5);
  testVector.push_back(2);
  testVector.push_back(4);
  printVector(testVector);
  cout << "do pop" << endl;
  myPop(testVector); 
  printVector(testVector);
  
//   swap(testVector[0], testVector[testVector.size() - 1]);
//   printVector(testVector);
//   testVector.pop_back();
//   printVector(testVector);
//   cout << "size:" << testVector.size() << endl;
//   cout << testVector[testVector.size() - 1] << endl;
//   cout << testVector[testVector.size()] << endl;
//   fixDown(testVector, 0);
//   printVector(testVector);

  //fixUp(testVector, testVector.size());

//   cout << "size:" << testVector.size() << endl;
//   testVector[testVector.size() + 1] = 123;
//   printVector(testVector);

  
 
}
