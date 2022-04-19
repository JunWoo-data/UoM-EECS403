// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

/*
 * Compile this test against your .h files to make sure they compile. Note how
 * the eecs281 priority queues can be constructed with the different types. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly.  If you do not call a
 * function from here, it is NOT compiled due to templates!  So for instance,
 * if you don't add code to call updatePriorities(), you could have compiler
 * errors that you don't even know about.
 *
 * Our makefile will build an executable named testPQ if you type 'make testPQ'
 * or 'make alltests' (without the quotes).
 *
 * Notice that testPairing() tests the range-based constructor but main() and
 * testPriorityQueue() do not.  Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * This is NOT a complete test of your priority queues, you have to add code
 * to do more testing!
 *
 * You do not have to submit this file, but it won't cause problems if you do.
 */

#include <cassert>
#include <iostream>
#include <string>  
#include <vector>

#include "Eecs281PQ.h"
#include "BinaryPQ.h"
#include "UnorderedPQ.h"
#include "PairingPQ.h"
#include "SortedPQ.h"

using namespace std;


// Used to test a priority queue containing pointers to integers.
struct IntPtrComp {
    bool operator() (const int *a, const int *b) const {
        return *a < *b;
    }
};


void printVector(vector<int> &inVector)
{   
  cout << "Firt-";  
  for (size_t i = 0; i < inVector.size(); i++)
  {
    cout << inVector[i] << "-";    
  }    
  cout << "End" << endl;      
}

// TODO: Make sure that you're using this-compare() properly, and everywhere
// that you should.  Complete this function by adding a functor that compares
// two HiddenData structures, create a PQ of the specified type, and call
// this function from main().
void testHiddenData(const string &pqType) {
    struct HiddenData {
        int data;
    };
    struct HiddenDataComp {
        bool operator()(const HiddenData &/*a*/, const HiddenData &/*b*/) const {
            // TODO: Finish this functor; when you do, uncomment the
            // parameters in the line above
            return false;
        }
    };

    cout << "Testing " << pqType << " with hidden data" << endl;

    // TODO: Add code here to actually test with the HiddenData type.
} // testHiddenData()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePrioritiesHelper(Eecs281PQ<int *, IntPtrComp> *pq) {
    vector<int> data;
    data.reserve(100);
    cout << "push 1 to vector" << endl;
    data.push_back(1);
    printVector(data);
    cout << "push 5 to vector" << endl;
    data.push_back(5);
    printVector(data);
    cout << "push 7 to vector" << endl;
    data.push_back(7);
    printVector(data);
    cout << "push 9 to vector" << endl;
    data.push_back(9);
    printVector(data);
    cout << "push 3 to vector" << endl;
    data.push_back(3);
    printVector(data);
    cout << "data size: " << data.size() << endl;

    // NOTE: If you add more data to the vector, don't push the pointers
    // until AFTER the vector stops changing size!  Think about why.
    // You can add up to 100 values, or change the reserve if you want more.

    for (size_t i = 0; i < data.size(); ++i) {
        cout << "push data[" << i << "]: " << data[i] << " element to pq" << endl; 
        pq->push(&data[i]);
        cout << "pq size: " << pq->size() << endl;
    } // for

    cout << "print pq elements" << endl;
    pq->printElement(true); 
  
    // Change the first value (which is pointed to by the pq), and check it.
    cout << "change data[0] to 10" << endl;
    data[0] = 10;  
    cout << "update priorities" << endl;
    pq->updatePriorities();
    cout << "top: " << *pq->top() << endl; // assert(*pq->top() == 10);
    
} // testUpdatePrioritiesHelper()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePriorities(const string &pqType) {
    Eecs281PQ<int *, IntPtrComp> *pq = nullptr;
    cout << "Testing updatePriorities() on " << pqType << endl;

    if (pqType == "Unordered") {
        pq = new UnorderedPQ<int *, IntPtrComp>;
    } // if
    // TODO: Add more types here inside 'else if' statements, like in main().

    else if (pqType == "Sorted")
    {
        pq = new SortedPQ<int *, IntPtrComp>;    
    }

    else if (pqType == "Binary")
    {
        pq = new BinaryPQ<int *, IntPtrComp>;    
    }

    else if (pqType == "Pairing")
    {
        pq = new PairingPQ<int *, IntPtrComp>;    
    }

    if (!pq) {
        cout << "Invalid pq pointer; did you forget to create it?" << endl;
        return;
    } // if

    testUpdatePrioritiesHelper(pq);
    delete pq;
} // testUpdatePriorities()


// Very basic testing.
void testPriorityQueue(Eecs281PQ<int> *pq, const string &pqType) {
    cout << "Testing priority queue: " << pqType << endl;
    
    cout << "push 18 to pq" << endl;
    pq->push(6);
    pq->printElement(false);

    cout << "push 6 to pq" << endl;
    pq->push(18);
    pq->printElement(false);

    cout << "push 81" << endl;
    pq->push(32);
    pq->printElement(false);

    cout << "pop" << endl;
    pq->pop();
    pq->printElement(false);



    
    

   
    //pq->printElement(false);

    // cout << "size: " << pq->size() << endl;
    // cout << "top: " << pq->top() << endl;
    // cout << "is empty: " << pq->empty() << endl; // assert(!pq->empty());

    // cout << "push 3" << endl;
    // pq->push(3);
    // pq->printElement(false);
    // cout << "push 5" << endl;
    // pq->push(5);
    // pq->printElement(false);
    // cout << "push 1" << endl;
    // pq->push(1);
    // pq->printElement(false);
    // cout << "push 7" << endl;
    // pq->push(7);
    // pq->printElement(false);
    // cout << "push 2" << endl;
    // pq->push(2);
    // pq->printElement(false);
    // cout << "push 4" << endl;
    // pq->push(4);
    // pq->printElement(false);

    // cout << "pop 6 times" << endl;
    // pq->pop();
    // pq->printElement(false);
    // pq->pop();  
    // pq->printElement(false);
    // pq->pop();  
    // pq->printElement(false);
    // pq->pop();  
    // pq->printElement(false);
    // pq->pop();  
    // pq->printElement(false);
    // pq->pop();  
    // pq->printElement(false);
    // pq->size();  
    // pq->printElement(false);
    // cout << "size: " << pq->size() << endl;

    // cout << "push 81 to vector" << endl;
    // pq->push(81);
    // pq->printElement(false);
    // cout << "push 18 to vector" << endl;
    // pq->push(18);
    // pq->printElement(false);
    // cout << "push 14 to vector" << endl;
    // pq->push(14);
    // pq->printElement(false);
    // cout << "push 9 to vector" << endl;
    // pq->push(9);
    // pq->printElement(false);
    // cout << "push 7 to vector" << endl;
    // pq->push(7);
    // pq->printElement(false);
    // cout << "push 1 to vector" << endl;
    // pq->push(1);
    // pq->printElement(false);
    // cout << "push 5 to vector" << endl;
    // pq->push(5);
    // pq->printElement(false);
    // cout << "push 2 to vector" << endl;
    // pq->push(2);
    // pq->printElement(false);
    // cout << "push 4 to vector" << endl;
    // pq->push(4);
    // pq->printElement(false);

    // cout << "pop" << endl;
    // pq->pop();
    // pq->printElement(false);
    // cout << "size: " << pq->size() << endl;
    
    cout << "testPriorityQueue() succeeded!" << endl;
    
} // testPriorityQueue()


// Test the pairing heap's range-based constructor, copy constructor,
// and operator=().
//
void testPairing(vector<int> & vec) {
    cout << "Testing Pairing Heap separately" << endl;
    Eecs281PQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    
    pq1->printElement(false);
    cout << "qp1 size:" << pq1->size() << endl;

    cout << "push 6 to pq" << endl;
    pq1->push(6);
    pq1->printElement(false);

    cout << "push 18 to pq" << endl;
    pq1->push(18);
    pq1->printElement(false);

    cout << "push 32" << endl;
    pq1->push(32);
    pq1->printElement(false);
    cout << "qp1 size:" << pq1->size() << endl;

    cout << "make q2 by copy q1" << endl;
    Eecs281PQ<int> * pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));
    pq2->printElement(false);
    cout << "qp2 size:" << pq2->size() << endl;
    
    cout << "q1 top: " << pq1->top() << endl;
    cout << "q2 top: " << pq2->top() << endl;
    cout << "pop pq1, pq2" << endl;
    pq1->pop();
    pq2->pop();
    cout << "pq1 size: " << pq1->size() << endl;
    cout << "pq2 size: " << pq2->size() << endl;
    cout << "==============" << endl;
    cout << "q1 top: " << pq1->top() << endl;
    cout << "q2 top: " << pq2->top() << endl;
    cout << "pop pq1, pq2" << endl;
    pq1->pop();
    pq2->pop();
    cout << "pq1 size: " << pq1->size() << endl;
    cout << "pq2 size: " << pq2->size() << endl;
    cout << "==============" << endl;
    cout << "q1 top: " << pq1->top() << endl;
    cout << "q2 top: " << pq2->top() << endl;
    cout << "pop pq1, pq2" << endl;
    pq1->pop();
    pq2->pop();
    cout << "pq1 size: " << pq1->size() << endl;
    cout << "pq2 size: " << pq2->size() << endl;
    cout << "==============" << endl;
    cout << "q1 top: " << pq1->top() << endl;
    cout << "q2 top: " << pq2->top() << endl;
    cout << "pop pq1, pq2" << endl;
    pq1->pop();
    pq2->pop();
    cout << "pq1 size: " << pq1->size() << endl;
    cout << "pq2 size: " << pq2->size() << endl;
    cout << "==============" << endl;
    cout << "q1 top: " << pq1->top() << endl;
    cout << "q2 top: " << pq2->top() << endl;
    cout << "pop pq1, pq2" << endl;
    pq1->pop();
    pq2->pop();
    cout << "pq1 size: " << pq1->size() << endl;
    cout << "pq2 size: " << pq2->size() << endl;
    cout << "==============" << endl;

  
    

    // // This line is different just to show two different ways to declare a
    // // pairing heap: as an Eecs281PQ and as a PairingPQ. Yay for inheritance!
    // PairingPQ<int> * pq3 = new PairingPQ<int>();
    // *pq3 = *((PairingPQ<int> *)pq2);
    
    
    
    // pq2->pop();
    // assert(pq1->size() == 3);
    // assert(!pq1->empty());
    // assert(pq1->top() == 3);
    // pq2->push(pq3->top());
    // assert(pq2->top() == pq3->top());

    cout << "Basic tests done." << endl;
    // TODO: Add more code to test addNode(), updateElt(), etc.

    cout << "Calling destructors" << endl;
    delete pq1;
    // delete pq2;
    // delete pq3;

    cout << "testPairing() succeeded" << endl;
} // testPairing()


int main() {
    // Basic pointer, allocate a new PQ later based on user choice.
    Eecs281PQ<int> *pq;
    vector<string> types{ "Unordered", "Sorted", "Binary", "Pairing" };
    unsigned int choice;

    cout << "PQ tester" << endl << endl;
    for (size_t i = 0; i < types.size(); ++i)
        cout << "  " << i << ") " << types[i] << endl;
    cout << endl;
    cout << "Select one: ";
    cin >> choice;

    if (choice == 0) {
        pq = new UnorderedPQ<int>;
    } // if
    else if (choice == 1) {
        pq = new SortedPQ<int>;
    } // else if
    else if (choice == 2) {
        pq = new BinaryPQ<int>;
    } // else if
    else if (choice == 3) {
        pq = new PairingPQ<int>;
    } // else if
    else {
        cout << "Unknown container!" << endl << endl;
        exit(1);
    } // else
   
    testPriorityQueue(pq, types[choice]);
    testUpdatePriorities(types[choice]);

    if (choice == 3) {
        vector<int> vec;
        vec.push_back(0);
        vec.push_back(1);
        testPairing(vec);
    } // if

    // Clean up!
    delete pq;

    return 0;
} // main()
