// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the 'priority queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, 
                  parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent;
    }; // Node


    // Description: Construct an empty pairing heap with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        
        root = nullptr;
        numOfNode = 0;
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        
        root = nullptr;
        numOfNode = 0;

        while (start != end)
        {
          push(*start);    
          start++;
        }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare } {
        
        std::deque<Node*> nodeList;
        
        root = nullptr;
        numOfNode = 0;

        nodeList.push_back(other.root);
        
        while (nodeList.size() > 0){
            if (nodeList.front()->child != nullptr){
                nodeList.push_back(nodeList.front()->child);
            }

            if (nodeList.front()->sibling != nullptr){
                nodeList.push_back(nodeList.front()->sibling);
            }

            push(nodeList.front()->elt);
            nodeList.pop_front();
        }
        // NOTE: The structure does not have to be identical to the original,
        //       but it must still be a valid Pairing Heap.
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // HINT: Use the copy-swap method from the "Arrays and Containers" lecture.

        PairingPQ temp(rhs);

        std::swap(numOfNode, temp.numOfNode);
        std::swap(root, temp.root);

        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        std::deque<Node*> nodeList;
        
        if (root != nullptr){
            nodeList.push_back(root);
        
            while (nodeList.size() > 0){
                if (nodeList.front()->child != nullptr){
                    nodeList.push_back(nodeList.front()->child);
                }

                if (nodeList.front()->sibling != nullptr){
                    nodeList.push_back(nodeList.front()->sibling);
                }

                delete nodeList.front();
                nodeList.front() = nullptr;
                nodeList.pop_front();
            }
        }
        
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out of order and
    //              'rebuilds' the pairing heap by fixing the pairing heap invariant.
    //              You CANNOT delete 'old' nodes and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        std::deque<Node*> nodeList;
        Node* firstNode = root;

        nodeList.push_back(firstNode);
        root = nullptr;

        while (nodeList.size() > 0){
            if (nodeList.front()->child != nullptr){
                nodeList.push_back(nodeList.front()->child);
            }

            if (nodeList.front()->sibling != nullptr){
                nodeList.push_back(nodeList.front()->sibling);
            }

            nodeList.front()->child = nullptr;
            nodeList.front()->sibling = nullptr;
            nodeList.front()->parent = nullptr;
            root = meld(root, nodeList.front());
            nodeList.pop_front();
        }
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already done.
    //              You should implement push functionality entirely in the addNode()
    //              function, and this function calls addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the pairing heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the pairing heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        std::deque<Node*> targetNodes;    
        
        if (root->child == nullptr){
            delete root;
            root = nullptr;
        }

        else {
            targetNodes.push_back(root->child);

            delete root;
            root = nullptr;

            while (targetNodes.back()->sibling != nullptr) {
                targetNodes.push_back(targetNodes.back()->sibling);
            }
        
            while (targetNodes.size() > 1)
            {   
                targetNodes[0]->parent = nullptr;
                targetNodes[0]->sibling = nullptr;
                targetNodes[1]->parent = nullptr;
                targetNodes[1]->sibling = nullptr;
                targetNodes.push_back(meld(targetNodes[0], targetNodes[1]));
                targetNodes.pop_front();
                targetNodes.pop_front();
            }

            root = targetNodes[0];
        }

        numOfNode -= 1;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return numOfNode;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return root == nullptr;
    } // empty()


    // Description: Updates the priority of an element already in the pairing heap by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        node->elt = new_value;
        updatePriorities();
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().s
    virtual Node* addNode(const TYPE &val) {
        Node* newNode = new Node(val);

        if (root == nullptr) root = newNode;

        else {
            root = meld(root, newNode);    
        }
        
        numOfNode += 1;
        return newNode; 
    } // addNode()

    // //TODO: delete this print function
    // virtual void printElement(bool pointerOption){
    //     std::deque<Node*> dqForNodes;
        
    //     dqForNodes.push_back(root);
        
    //     if (!pointerOption) {
    //         std::cout << "First-";
        
    //         while (dqForNodes.size() > 0){
    //             if (dqForNodes.front()->child != nullptr){
    //                 dqForNodes.push_back(dqForNodes.front()->child);
    //             }

    //             if (dqForNodes.front()->sibling != nullptr){
    //                 dqForNodes.push_back(dqForNodes.front()->sibling);
    //             }
                
    //             std::cout << dqForNodes.front()->elt << "-";
    //             dqForNodes.pop_front();
    //         }
    //     }
    //      std::cout << "Last" << std::endl;   
    // }

private:
    // NOTE: For member variables, you are only allowed to add a "root pointer"
    //       and a "count" of the number of nodes.  Anything else (such as a deque)
    //       should be declared inside of member functions as needed.
    Node* root;
    size_t numOfNode = 0;

    Node* meld(Node* &lhs, Node* &rhs){
        if (lhs == nullptr) return rhs;
        if (rhs == nullptr) return lhs;

        if (this->compare(lhs->elt, rhs->elt)){    
            lhs->parent = rhs;
            lhs->sibling = rhs->child;
            rhs->child = lhs;  
            return rhs;
        } // if lhs < rhs        

        else {
            rhs->parent = lhs;
            rhs->sibling = lhs->child;
            lhs->child = rhs;
            
            return lhs;        
        } // else if rhs < lhs
    }
};


#endif // PAIRINGPQ_H


 // std::cout << "front: " << dqForNodes.front()->elt << std::endl;

        // if (dqForNodes.front()->child != nullptr){
        //     std::cout << "there is a child" << std::endl;
        //     std::cout << "front-child: " << dqForNodes.front()->child->elt << std::endl;
        //     std::cout << "push front-child" << std::endl;
        //     dqForNodes.push_back(dqForNodes.front()->child);
        // }

        // if (dqForNodes.front()->sibling != nullptr){
        //     std::cout << "there is a sibling" << std::endl;
        //     std::cout << "front-sibling: " << dqForNodes.front()->sibling->elt << std::endl;
        //     std::cout << "push front-sibling" << std::endl;
        //     dqForNodes.push_back(dqForNodes.front()->sibling);
        // }

        // std::cout << "before size: " << dqForNodes.size() << std::endl;
        // std::cout << "pop front" << std::endl;
        // dqForNodes.pop_front();
        // std::cout << "after size: " << dqForNodes.size() << std::endl;
        // std::cout << "=========================" << std::endl;


        // std::cout << "front: " << dqForNodes.front()->elt << std::endl;

        // if (dqForNodes.front()->child != nullptr){
        //     std::cout << "there is a child" << std::endl;
        //     std::cout << "front-child: " << dqForNodes.front()->child->elt << std::endl;
        //     std::cout << "push front-child" << std::endl;
        //     dqForNodes.push_back(dqForNodes.front()->child);
        // }

        // if (dqForNodes.front()->sibling != nullptr){
        //     std::cout << "there is a sibling" << std::endl;
        //     std::cout << "front-sibling: " << dqForNodes.front()->sibling->elt << std::endl;
        //     std::cout << "push front-sibling" << std::endl;
        //     dqForNodes.push_back(dqForNodes.front()->sibling);
        // }

        // std::cout << "before size: " << dqForNodes.size() << std::endl;
        // std::cout << "pop front" << std::endl;
        // dqForNodes.pop_front();
        // std::cout << "after size: " << dqForNodes.size() << std::endl;