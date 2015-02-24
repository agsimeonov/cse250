#include <iostream>
#include <vector>
#include <string>

using namespace std;

/** File QueueTestAGS.cpp for CSE250 Assignment 2.  Translation from QueueTest.java */
class StringQueue {
  vector<string>* elements;
  int frontSpace;
  int rearItem;
  int maxAdds;

  public:
    /** Creates a StringQueue
	@param maxAdds The amount of space allocated for the vector 
    */
    StringQueue(int maxAdds) : elements(new vector<string>(maxAdds)), frontSpace(0),
                               rearItem(0) {
      this->maxAdds = maxAdds;
    }
   
    virtual bool isEmpty() {
      return frontSpace == rearItem;
    }

    virtual bool isFull() {
      return frontSpace == maxAdds;
    }

    virtual int size() const {
      return frontSpace - rearItem;
    }
    
    /** Adds a string element to the front of the StringQueue
	@param newItem the string to be added
    */
    virtual void pushFront(string newItem) {
      elements->at(frontSpace++) = newItem;
    }
    
    /** Removes an element from the end of the StringQueue */
    virtual string popRear() {
      if(this->isEmpty()) {
        cerr << "Later we'll define and throw an EmptyQException" << endl;
      }
      else {
        return elements->at(rearItem++);
      }
    }

    /** Gives a string representation of the contents of the StringQueue */
    virtual string toString() {
      string out = "";
      for(int i = rearItem; i < frontSpace; i++) {
        out += elements->at(i) + " ";
      }
      return out;
    }
};

int main() {
  StringQueue* sq = new StringQueue(100);
  sq->pushFront("oh");
  sq->pushFront("say");
  sq->pushFront("can");
  sq->pushFront("you");
  sq->pushFront("see") ;
  cout << sq->toString() << endl;
  string pop2 = sq->popRear() + " " + sq->popRear();
  cout << pop2 + ", did this print in the right order?" << endl;
  sq->pushFront("me?");
  cout << "Final queue: " + sq->toString() << endl;
  return (0);
}
