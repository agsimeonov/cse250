/** File "FlexArrayAGS.h", by AGS for CSE250, Fall 2010, Project 1. */

#ifndef FLEXARRAYAGS_H_
#define FLEXARRAYAGS_H_

#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <iostream>

template <typename T>
class FlexArray;

template <typename T>
class Node {
  friend class FlexArray<T>;

  std::vector<T>* elements;
  Node<T>* prev;
  Node<T>* next;
  
  Node(Node* givenPrev = NULL, Node* givenNext = NULL)
    : elements(new std::vector<T>()),
      prev(givenPrev),
      next(givenNext) {
  }
};

template <typename T>
class FlexArray {
  private:
    static const size_t DEFAULT_CAPACITY = 10;
    //The actual node capacity set by the construcror
    size_t nodeCapacity;
    //The current size of the FlexArray - Make sure to keep up to date
    size_t currentSize;
    //Dummy node
    Node<T>* endNode;
    Node<T>* firstNode;
    void split(Node<T>* current) {
      Node<T>* newNode = new Node<T>(current, current->next);
      //Hooks up as previous to the former next node
      newNode->next->prev = newNode;
      //Hooks up as next to the former current node
      current->next = newNode;

      //Takes half or rounded up at half of the node capacity
      size_t half = ((nodeCapacity % 2) == 1) ? (nodeCapacity+1)/2 : nodeCapacity/2;

      //Copies half the elements in current node to newNode
      for(size_t i = half; i < current->elements->size(); i++) {
        newNode->elements->push_back(current->elements->at(i));
      }

      //Erases already copied elements
      for(size_t j = current->elements->size() - 1; j >=half; j--) {
        current->elements->erase(current->elements->begin()+(j));
      }
    }

  public:
    explicit FlexArray(size_t nodeCap)
      : firstNode(endNode),
        endNode(new Node<T>()),
        nodeCapacity(nodeCap),
        currentSize(0) {
    }

    explicit FlexArray()
      : firstNode(endNode),
        endNode(new Node<T>()),
        nodeCapacity(DEFAULT_CAPACITY),
        currentSize(0) {
    }

    virtual ~FlexArray() {
      while(firstNode != NULL) {
        Node<T>* current = firstNode;
        firstNode = firstNode->next;
        current->elements->clear();
        delete current->elements;
        delete current;
      }
      currentSize = 0;
    }

    class iterator {
      friend class FlexArray<T>;
      
      private:
        const FlexArray<T>* myFlex;
        Node<T>* node;
        size_t localIndex;

        iterator(const FlexArray<T>* givenFlex, Node<T>* givenNode, size_t givenLocalIndex)
          : myFlex(givenFlex),
            node(givenNode),
            localIndex(givenLocalIndex) {
        }

      public:
        T& operator*() const {
          if(node == NULL) {
            throw std::runtime_error("can' dereference null node");
          }
          else if(node == myFlex->endNode) {
            throw std::runtime_error("can't dereference endNode");
          }
          else {
            return node->elements->at(localIndex);
          }
        }

        T* operator->() const {
          if(node == NULL) {
            throw std::runtime_error("can't dereference null node");
          }
          else if(node ==  myFlex->endNode) {
            throw std::runtime_error("can't dereference endNode");
          }
          else {
            return &(node->elements->at(localIndex));
          }
        }

        //For preincrement
        iterator& operator++() {
          //For bi-directional iterator - NULL node is concidered the node before firstNode
          //thereby here operator++ gives us firstNode at index 0
          if(node == NULL) {
            node = myFlex->firstNode;
            localIndex = 0;  
            return *this;         
          }
          else if(node == myFlex->endNode) {
            throw std::runtime_error("attempt to move past end position");
          }
          else {
            //Makes sure you get to the next valid position that isn't past end() or within endNode
            if((localIndex==(node->elements->size()-1)) && (node->next!=myFlex->endNode)) {
              localIndex = 0;
              node = node->next; 
            }
            else {
              localIndex++;
            }
            return *this;
          }
        }

        //For postincrement
        iterator operator++(int ignore) {
          iterator returnValue = *this;
          ++(*this);
          return returnValue;
        }

        //For preincrement
        iterator& operator--() {
          if(node == NULL) {
            throw std::runtime_error("attempt to move before null position");
          }
          else {
            //Makes sure you get to the next valid position that isn't past rbegin() or within rend()
            if((localIndex==0) && (node->prev!=NULL)) {
              localIndex = (node->prev->elements->size()-1);
              node = node->prev;
            }
            else {
              localIndex--;
            }
            return *this;
          }
        }

        //For postincrement
        iterator operator--(int ignore) {
          iterator returnValue = *this;
          --(*this);
          return returnValue;
        }

        //EXTRA CREDIT - Random Access Iterator
        iterator& operator+=(int i) {
          for(int a = 0; a < i; ++a) {
            ++(*this);
          }
          return *this;
        }

        //EXTRA CREDIT - Random Access Iterator
        iterator operator+(int i) {
          iterator returnValue = *this;
          returnValue += i;
          return returnValue;
        }

        //EXTRA CREDIT - Random Access Iterator
        iterator& operator-=(int i) {
          for(int a = 0; a < i; ++a) {
            --(*this);
          }
          return *this;
        }

        //EXTRA CREDIT - Random Access Iterator
        iterator operator-(int i) {
          iterator returnValue = *this;
          returnValue -= i;
          return returnValue;
        }

        bool operator==(const iterator& iter) {
            return (node==iter.node) && (localIndex==iter.localIndex) && (myFlex==iter.myFlex);
        }

        bool operator!=(const iterator& iter) {
          return !this->operator==(iter);
        }
    };

    //Goes on first real element - would produce end() on empty
    iterator begin() {
      return iterator(this, firstNode, 0);
    }

    //Goes one step past last real element
    iterator end() {
      if(this->empty()) {
        return iterator(this, endNode, 0);
      }
      else {
        return iterator(this, endNode->prev, endNode->prev->elements->size());
      }
    }

    //Goes on the last real element - would produce rend() on empty
    iterator rbegin() {
      if(this->empty()) {
        return iterator(this, NULL, 0); //NULL dealt with in iterator
      }
      else {
        return iterator(this, endNode->prev, endNode->prev->elements->size()-1);
      }
    }
    
    //Goes one step before first real element
    iterator rend() {
      return iterator(this, NULL, 0); //NULL dealt with in iterator
    }

    size_t size() const {
      return currentSize;
    }

    bool empty() const {      
      return (firstNode==endNode);
    }
    
    //Determines which chunk contains given index and gives us the element at that index
    T& at(size_t i) {
      if((i>=this->size()) || this->empty()) {
        throw std::out_of_range("given index for at or operator[] is out of range");
      }
      else {
        //Helps one aquire the node contatining given global index
        Node<T>* desiredNode = firstNode;
        //Counter which helps determine the exact location of a global index
        size_t counter = firstNode->elements->size();
        bool searching = true;
        while(searching) {
          if(i >= counter) {
            desiredNode = desiredNode->next;
            counter += desiredNode->elements->size();
          }
          else {
            //The actual localIndex acquired though the given index
            size_t localIndex = desiredNode->elements->size() - (counter-i);
            //Returns the element at the given index
            return desiredNode->elements->at(localIndex);
            searching = false;
          }
        }
      }
    }

    //operator[] version of at
    T& operator[](size_t i) {
      return this->at(i);
    }

    /** Returns iterator to *new* element, not the one inserted-before.
        New element has same co-ordinates as "beforeMe", *unless* node splits.
        Allows beforeMe == end(), which means appending item as the last element.
    */
    iterator insert(iterator beforeMe, const T& item) {
      if(this->empty() && beforeMe==this->begin()) {
        //Initializes the firstNode makes sure endNode is next
        firstNode = new Node<T>(NULL, endNode);
        //Makes sure firstNode is now previous to endNode
        endNode->prev = firstNode;
        //Adjusts the iterators node to the newly created node
        beforeMe.node = beforeMe.node->prev;
        //Inserts the item
        //Uses constant push_back over linear insert
        beforeMe.node->elements->push_back(item);
        currentSize = currentSize + 1;
        return beforeMe;
      }
      //Makes sure that you can't insert in a random arbitrary position - keeps sequntial pattern of FlexArray
      else if(beforeMe.localIndex > beforeMe.node->elements->size()) {
        throw std::out_of_range("given iterator is not in a sequential position");
      }
      //Checks whether splitting is necessary
      else if(beforeMe.node->elements->size() == nodeCapacity) {
        //Half of the nodeCapcity, rounded up if needed
        size_t half = ((nodeCapacity % 2) == 1) ? (nodeCapacity+1)/2 : nodeCapacity/2;
        //Splits the full node
        this->split(beforeMe.node);
        //Re-adjusts the iterator if needed
        if(beforeMe.localIndex <= half) { 
          //Do nothing
        }
        else {
          //Adjusts the iterators node to the newly created node during the split
          beforeMe.node = beforeMe.node->next;
          //Adjusts the localIndex of the iterator
          beforeMe.localIndex = beforeMe.localIndex - half;
        }
        //Inserts the item
        //Uses constant push_back over linear insert whenever possible
        if(beforeMe.node->elements->size() == beforeMe.localIndex) {
          beforeMe.node->elements->push_back(item);
        }
        else {
          beforeMe.node->elements->insert(beforeMe.node->elements->begin()+beforeMe.localIndex, item);
        }
        currentSize = currentSize + 1;
        return beforeMe;
      }
      else {
        //Inserts the item
        //Uses constant push_back over linear insert whenever possible
        if(beforeMe.node->elements->size() == beforeMe.localIndex) {
          beforeMe.node->elements->push_back(item);
        }
        else {
          beforeMe.node->elements->insert(beforeMe.node->elements->begin()+beforeMe.localIndex, item);
        }
        currentSize = currentSize + 1;
        return beforeMe;
      }
    }

    void insert(size_t i, const T& item) {
      if(this->empty() && (i==0)) {
        //Initializes the firstNode makes sure endNode is next
        firstNode = new Node<T>(NULL, endNode);
        //Makes sure firstNode is now previous to endNode
        endNode->prev = firstNode;
        //Inserts item at the begining of the first elements vector
        //Uses constant push_back over linear insert
        firstNode->elements->push_back(item);
        currentSize = currentSize + 1;
      }
      //Only > operator here because we can insert at the end of the FlexArray
      else if(i > this->size()) {
        throw std::out_of_range("given index for insert is out of range");
      }
      else {
        //Helps one aquire the node contaitning given global index
        Node<T>* desiredNode = firstNode;
        //Counter which helps determine the exact location of given global index
        size_t counter = firstNode->elements->size();
        bool inserting = true;
        while(inserting) {
          if(i > counter) {
            desiredNode = desiredNode->next;
            counter += desiredNode->elements->size();
          }
          else {
            //The actual localIndex acquired though the given index
            size_t localIndex = desiredNode->elements->size() - (counter - i);
            //Checks wheter the desired node is at full capacity
            if(desiredNode->elements->size() == nodeCapacity) {
              //Half of the nodeCapacity, rounded up if needed
              size_t half = ((nodeCapacity % 2) == 1) ? (nodeCapacity+1)/2 : nodeCapacity/2;
              
              //Makes sure we have the correct desiredNode
              if(localIndex <= half) {
                //Splits the full node
                this->split(desiredNode);
                //Does not change desiredNode or localIndex  because the index is in the first half of the split node
                //Inserts the item before the element with index i
                //Uses constant push_back over linear insert whenever possible
                if(desiredNode->elements->size() == localIndex) {
                  desiredNode->elements->push_back(item);
                }
                else {
                  desiredNode->elements->insert(desiredNode->elements->begin()+localIndex, item);
                }
              }
              else {
                this->split(desiredNode);
                //Changes the desiredNode because the index is in the second half of the split node
                desiredNode = desiredNode->next;
                //New localIndex after the split
                localIndex = localIndex - half;
     
                //Inserts the item before the element with index i
                //Uses constant push_back over linear insert whenever possible
                if(desiredNode->elements->size() == localIndex) {
                  desiredNode->elements->push_back(item);
                }
                else {
                  desiredNode->elements->insert(desiredNode->elements->begin()+localIndex, item);
                }
              }              
            }
            else {
              //Inserts item before the element with index i
              //Uses constant push_back over linear insert whenever possible
              if(desiredNode->elements->size() == localIndex) {
                desiredNode->elements->push_back(item);
              }
              else {
                desiredNode->elements->insert(desiredNode->elements->begin()+localIndex, item);
              }
            }
            currentSize = currentSize + 1;
            inserting = false;
          }
        }
      }
    }

    /** Splice-out element of "me", return valid iterator to the next element
        so that one can continue iterating.  REQ: "me" *cannot* equal end().
        Does allow me == rbegin(), which means erasing the last element and
        returning end().
    */
    iterator erase(iterator me) {
      if((me.localIndex>=me.node->elements->size()) || this->empty()) {
        throw std::out_of_range("given iterator for erase is out of range");
      }
      //Checks whether deletion of node would be needed after erasing
      else if(me.node->elements->size() == 1) {
        //Erases the element at me - uses constant pop_back over linear erase
        me.node->elements->pop_back();
        if(me.node->elements->empty()) {
          if(me.node->prev != NULL) {
            //Hooks the previous node to the next node
            me.node->prev->next = me.node->next;
            //Hooks the next node to the previous node
            me.node->next->prev = me.node->prev;
          }
          else {
            //Makes sure firstNode still points to the first node in FlexArray
            firstNode = me.node->next;
          }
          Node<T>* deletable = me.node;
          me.localIndex = 0;
          me.node = me.node->next;
          delete deletable;
          currentSize = currentSize - 1;
          return me;
        }
      }
      else {
        //Erases the element at me - uses constant pop_back over linear erase whenever possible
        if(me.node->elements->size() == (me.localIndex+1)) {
          me.node->elements->pop_back();
        }
        else {
          me.node->elements->erase(me.node->elements->begin()+me.localIndex);
        }
        currentSize = currentSize - 1;
        return me;
      }
    }

    void erase(size_t i) {
      if((i>=this->size()) || this->empty()) {
        throw std::out_of_range("given index for erase is out of range");
      }
      else {
        //Helps one aquire the node containing the given index
        Node<T>* desiredNode = firstNode;
        //Counter which helps detrime the exact location of a given global index
        size_t counter = firstNode->elements->size();
        bool erasing = true;
        while(erasing) {
          if(i >= counter) {
            desiredNode = desiredNode->next;
            counter += desiredNode->elements->size();
          }
          else {
            //The actual localIndex acquired though the given index
            size_t localIndex = desiredNode->elements->size() - (counter - i);
            //Erases the element with index i - uses the constant pop_back over linear erase whenever possible
            if(desiredNode->elements->size() == (localIndex+1)) {
              desiredNode->elements->pop_back();
            }
            else {
              desiredNode->elements->erase(desiredNode->elements->begin()+localIndex);
            }
            if(desiredNode->elements->empty()) {
              if(desiredNode->prev != NULL) {
                //Hooks the previous node to the next node
                desiredNode->prev->next = desiredNode->next;
                //Hooks the next node to the previous node
                desiredNode->next->prev = desiredNode->prev;
              }
              else {
                //Makes sure firstNode still points to the first node in FlexArray
                firstNode = desiredNode->next;
              }
              delete desiredNode;
            }
            else {
              //Do nothing
            }
            currentSize = currentSize - 1;
            erasing = false;
          }
        }
      }
    }

    void pushFront(const T& newItem) {
      this->insert(this->begin(), newItem);
    }

    void pushRear(const T& item) {
      this->insert(this->end(), item);
    }

    T popFront() {
      T item = *(this->begin());
      this->erase(this->begin());
      return item;
    }

    T popRear() {
      if(this->empty()) {
        throw std::out_of_range("can't use popRear on an empty FlexArray");
        return T();
      }
      else {
        T item = *(this->rbegin());
        this->erase(this->rbegin());
        return item;
      }
    }

    //For any T with a .str()
    std::string toString() const {
      std::ostringstream out;
      for(iterator i = iterator(this, firstNode, 0); 
          i != iterator(this, endNode->prev, endNode->prev->elements->size()); ++i) {
        out << (*i).str() << " ";
      }
      return out.str();
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const FlexArray<T>& ds) {
  out << ds.toString();
  return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const FlexArray<T>*& dsp) {
  out << dsp->toString();
  return out;
}

#endif
