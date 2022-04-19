// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
    } // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
          while (start != end)
          {
            data.push_back(*start);    
            ++start;
          }

          updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        for(int i = (int)data.size() - 1; i >= 0; i--)
        {
          fixDown(i);    
        }
    } // updatePriorities()


    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val) {
        data.push_back(val);
        if (data.size() > 1)
        {
          fixUp((int)data.size() - 1);
        }
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        std::swap(data[0], data[data.size() - 1]);
        data.pop_back();
        if (data.size() > 1)
        {
          fixDown(0);    
        }
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return data.front();
    } // top()


    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()
    
    // //TODO: delete this print function
    // virtual void printElement(bool pointerOption)
    // {
    //   if (!pointerOption)
    //   {
    //     std::cout << "Firt-";  
    //     for (size_t i = 0; i < data.size(); i++)
    //     {
    //       std::cout << data[i] << "-";    
    //     }    
    //     std::cout << "End" << std::endl;
    //   }

    //   else
    //   {
    //     std::cout << "Firt-";  
    //     for (size_t i = 0; i < data.size(); i++)
    //     {
    //       std::cout << &data[i] << "-";    
    //     }    
    //     std::cout << "End" << std::endl;    
    //   }
    // }

private:
    // Note: This vector *must* be used for your heap implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables.  You don't need
    //       a "heapSize", since you can call your own size() member function,
    //       or check data.size().
    void fixUp(int index)
    {
      while ((index > 0) && (this->compare(data[((size_t)index - 1) / 2], data[(size_t)index])))
      {
        std::swap(data[(size_t)index], data[((size_t)index - 1) / 2]);
        index = (index - 1) / 2;    
      }     
    }

    void fixDown(int index)
    {
      while(((2 * index) + 1) < (int)data.size())
      {
        int j = (2 * index) + 1;
        if ((j < (int)data.size() - 1) && (this->compare(data[(size_t)j], data[(size_t)j + 1])))
        {
          ++j;    
        }    
    
        if (!this->compare(data[(size_t)index], data[(size_t)j]))
        {     
          break;    
        }
    
        std::swap(data[(size_t)index], data[(size_t)j]);
        index = j;
      }     
    }


}; // BinaryPQ


#endif // BINARYPQ_H
