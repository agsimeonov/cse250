/** File "DequeAGS.h", by Alexander Georgiev Simeonov
    for CSE250, Fall 2010. Assignment 4.
*/
#ifndef DEQUEAGS_H_
#define DEQUEAGS_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Required on timberlake.cse.buffalo.edu
template <typename T>
class Deque;

/** Creates a simple Queue */
template <typename T>
class Queue {
  private:
    vector<T>* elements;
    int frontSpace;
    int rearItem;
    int maxAdds;

  public:
    friend class Deque<T>;

    /** Creates a Queue
        @param maxAdds the amount of space allocated for the vector
    */
    Queue(int maxAdds)
      : elements(new vector<T>(maxAdds)),
        frontSpace(0),
        rearItem(0),
        maxAdds(maxAdds) {
    }
    
    /** Accessor method for the rear element
        @return the rear item
    */
    virtual T rear() const {
      return elements->at(rearItem);
    }

    /** Accessor method for the item previous to rear
        @return the item previous to rear
    */
    virtual T previousRear() const {
      return elements->at(rearItem+1);
    }
    
    /** Verifies whether the Queue is empty
        @return ture if and only if the Queue is empty
    */
    virtual bool isEmpty() const {
      return frontSpace == rearItem;
    }

    /** Verifies whether the Queue is full
        @return ture if and only if the Queue is full
    */
    virtual bool isFull() const {
      return frontSpace == maxAdds;
    }

    /** Supplies the current size of the Queue
        @return the current size of the Queue
    */
    virtual int size() const {
      return frontSpace - rearItem;
    }
   
    /** Adds an element to the front of the Queue
        @param newItem the element to be added
    */
    virtual void pushFront(T newItem) {
      if(this->isFull()) {
        cerr << "Later we'll define and throw a FullQException" << endl;
      }
      else {
        elements->at(frontSpace++) = newItem;
      }
    }

    /** Removes an element from the rear of the Queue
        @return the element that was removed
    */
    virtual T popRear() {
      if(this->isEmpty()) {
        cerr << "Later we'll define and throw an EmptyQException" << endl;
        return T();
      }
      else {
        return elements->at(rearItem++);
      }
    }

    /** Supplies a string representation of the Queue
        @return a string with all the elements in the Queue
    */
    virtual string toString() const {
      ostringstream out;
      for(int i = rearItem; i < frontSpace; i++) {
        out << elements->at(i) << " ";
      }
      return out.str();
    } 
};

/** A derived class Deque which creates a Queue that can access both
    front and rear elements
*/
template <class T>
class Deque : public Queue<T> {
  private:
    int rearSpace;
    int frontItem;

  /** Creates a Deque
      @param maxAdds maximum elements to be added at front or rear
  */
  public:
    Deque<T>(int maxAdds)
      : Queue<T>(2*maxAdds),
        rearSpace(maxAdds),
        frontItem(maxAdds) {
      this->frontSpace = maxAdds;
      this->rearItem = maxAdds;

    }
    
    /** Accessor method for the front item
        @return the front item in the Deque
    */
    virtual T front() const {
      return this->elements->at(frontItem);
    }

    /** Accessor method for the item previous to front
        @return the item previous to front
    */
    virtual T previousFront() const {
      return this->elements->at(frontItem-1);
    }
    
    /** Determines whether the Deque is full
        @return true if and only if the Deque is full at either end
    */
    virtual bool isFull() const {
      if((this->frontItem==(this->maxAdds-1)) || (this->rearItem==0)) {
        return true;
      }
      else {
        return false;
      }
    }
    
    /** Adds an element to the front of the Deque
        @param newItem the element to be added
    */
    virtual void pushFront(T newItem) {
      if(this->isFull()) {
        cerr << "Later we'll define and throw a FullQException" << endl;
      }
      else {
        if(this->isEmpty() == true) {
          rearSpace--;
        }
        else {
          frontItem++;
        }
        this->elements->at(this->frontSpace++) = newItem;
      }
    }

    /** Adds an element to the rear of the Deque
        @param newItem the element to be added
    */
    virtual void pushRear(T newItem) {
      if(this->isFull()) {
        cerr << "Later we'll define and throw a FullQExcception" << endl;
      }
      else {
        if(this->isEmpty() == true) {
          this->frontSpace++;
        }
        else {
          this->rearItem--;
        }
        this->elements->at(rearSpace--) = newItem;
      }
    }

    /** Removes an element from the front of the Deque
        @return the item that was removed
    */
    virtual T popFront() {
      T something;
      if(this->isEmpty()) {
        cerr << "Later we'll define and throw an EmptyQException" << endl;
        return T();
      }
      else {
       this->frontSpace--;
        something = this->elements->at(frontItem--);
        //Restarts initial Deque parameters if the Deque is once again empty  
        if(frontItem == rearSpace) {
          this->frontSpace = (this->maxAdds/2);
          rearSpace = (this->maxAdds/2);
          frontItem = (this->maxAdds/2);
          this->rearItem = (this->maxAdds/2);
        }
        else {
          //Do Nothing
        }
      return something;
      }
    }
   
    /** Removes an element from the rear of the Deque
        @return the item that was removed
    */
    virtual T popRear() {
      T something;
      if(this->isEmpty()) {
        cerr << "Later we'll define and throw an EmptyQException" << endl;
        return T();
      }
      else {
        rearSpace++;
        something = this->elements->at(this->rearItem++);
        //Restarts initial Deque parameters if the Deque is once again empty 
        if(this->rearItem == this->frontSpace) {
          this->frontSpace = (this->maxAdds/2);
          rearSpace = (this->maxAdds/2);
          frontItem = (this->maxAdds/2);
          this->rearItem = (this->maxAdds/2);
        }
        else {
          //Do Nothing
        }
        return something;
      }
    }
};

#endif
