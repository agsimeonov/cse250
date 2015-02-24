/** File "DequeTestAGS.cpp", by Alexander Georgiev Simeonov for CSE250, Fall 2010.  Assignment 3. */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

/** StrinQueue is a parent class which creates a simple Queue */
class StringQueue {
  private:
    vector<string>* elements;
    int frontSpace;
    int rearItem;
    int maxAdds;

  public:
    //Allows StringDeque to use the private variables within StringQueue
    friend class StringDeque;

    /** Creates a StringQueue
	@param maxAdds The amount of space allocated for the vector 
    */
    StringQueue(int maxAdds)
      : elements(new vector<string>(maxAdds)),
        frontSpace(0),
        rearItem(0),
        maxAdds(maxAdds) {
    }
    
    /** Accessor method for rearItem
        @return the rear item
    */
    virtual string rear() const {
      return elements->at(rearItem);
    }
    
    /** Verifies whether the Queue is empty
        @return true if and only if the Queue is empty
    */
    virtual bool isEmpty() const {
      return frontSpace == rearItem;
    }
    
    /** Verifies whether the Queue is full
        @return ture if an only if the Queue is full
    */
    virtual bool isFull() const {
      return frontSpace == maxAdds;
    }
   
    /** Gives you the size of the Queue
        @return the current size of the Queue
    */
    virtual int size() const {
      return frontSpace - rearItem;
    }
    
    /** Adds a string element to the front of the StringQueue
	@param newItem the string to be added
    */
    virtual void pushFront(string newItem) {
      if(this->isFull()) {
        cerr << "Later we'll define and throw a FullQException" << endl;
      }
      else {
        elements->at(frontSpace++) = newItem;
      }
    }
    
    /** Removes an element from the end of the StringQueue
        @return the string that was removed
    */
    virtual string popRear() {
      if(this->isEmpty()) {
        cerr << "Later we'll define and throw an EmptyQException" << endl;
        return "";
      }
      else {
        return elements->at(rearItem++);
      }
    }

    /** Gives a string representation of the contents of the StringQueue
        @return a string with all the elements in the Queue
    */
    virtual string toString() const {
      string out = "";
      for(int i = rearItem; i < frontSpace; i++) {
        out += elements->at(i) + " ";
      }
      return out;
    }
};

/** A derived class StringDeque which inherits from StringQueue. 
    It supplies a Queue with the ability to insirt and remove items
    both from its front and rear.
*/
class StringDeque: public StringQueue {
  private:
    int rearSpace;
    int frontItem;
  
  /** The constructor initializes the Deque, and sets it's initial start points
      in the middle, so that the Deque can grow in either direction
  */
  public:
    StringDeque(int maxAdds)
      : StringQueue(2*maxAdds),
        rearSpace((maxAdds)),
        frontItem(maxAdds) {
      frontSpace = maxAdds;
      rearItem = maxAdds;
    }
    
    /** An accesor method for frontItem
        @return the front item in the Deque
    */
    virtual string front() const {
      return elements->at(frontItem);
    }
    
    /** Determines whether the Deque is full - method overwritten to comply with the new defintion of full
        @return true if an only if the deque is full at either end
    */
    virtual bool isFull() const {
      if((frontItem==(maxAdds-1)) || (rearItem==0)) {
        return true;
      }
      else {
        return false;
      }
    }

    /** Inserts a new item at the front of the Deque - overwritten to comply with new rules set by the Deque
         @param newItem the string to be inserted
    */
    virtual void pushFront(string newItem) {
      if(this->isFull()) {
	cerr << "Later we'll define and throw a FullQException" << endl;	
      }
      //Makes sure  to set the correct rearSpace and frontItem based on the status of the Deque
      else {
        if(this->isEmpty() == true) {
          rearSpace--;
        }
        else {
          frontItem++;
        }
        elements->at(frontSpace++) = newItem;
      }
    }
    
    /** Insirets a new item at the rear of the Deque
        @param newItem the string to be inserted
    */
    virtual void pushRear(string newItem) {
      if(this->isFull()) {
	cerr << "Later we'll define and throw a FullQException" << endl;
      }
      //Makes sure to set the correct frontSpace and rearItem based on the status of the Deque
      else {
        if(this->isEmpty() == true) {
          frontSpace++;
        }
        else {
          rearItem--;
        }
        elements->at(rearSpace--) = newItem;
      }
    }
    
    /** Removes an item from the front of the Deque
         @return the item that was removed
    */
    virtual string popFront() {
      string s = "";
      if(this->isEmpty()) {
        cerr << "Later we'll define and throw and EmptyQException" << endl;
        return s;
      }
      else {
        frontSpace--;
        s = elements->at(frontItem--);
        /*  Ensures that once the last item in the Deque is removed
            the Deque would be restored to it's initial state - thus
            evading any errors and incosistencies on emptying
        */
        if(frontItem == rearSpace) {
          frontSpace = (maxAdds/2);
          rearSpace = (maxAdds/2);
          frontItem = (maxAdds/2);
          rearItem = (maxAdds/2);
        }
        else {
          //Do Nothing
        }
      return s;
      } 
    } 

    /** Removes an item from the rear of the Deque - overwritten to comply with new rules set by Deque
        @return the string that was removed
    */
    virtual string popRear() {
      string s = "";
      if(this->isEmpty()) {
        cerr << "Later we'll define and throw an EmptyQException" << endl;
        return s;
      }
      else {
        rearSpace++;
        s = elements->at(rearItem++);
        /*  Ensures that once the last item in the Deque is removed
            the Deque would be restored to it's initial state - thus
            evading any errors and inconsistencies on emptying
        */
            if(rearItem == frontSpace) {
              frontSpace = (maxAdds/2);
              rearSpace = (maxAdds/2);
              frontItem = (maxAdds/2);
              rearItem = (maxAdds/2);
            }
            else {
              //Do Nothing
            }
        return s;
      }
    }
};

/** Determines whether two strings are neigbors (have edit distance of 1)
    @param const string& x the first string
    @param const string& y the second string
    @return true if an only if x and y are neighbors
*/
bool ed1(const string& x, const string& y) {
  int xlen = x.length();
  int ylen = y.length();
  bool t = true;
 
  if(xlen > ylen) {
    //Makes sure x isn't more than 1 character larger than y
    if((xlen-ylen) > 1) {
      t = false;
    }
    //x is exactly 1 character larger than y
    else {
      //Compares both strings and makes sure that they are the same except for 1 character
      for(int i=0; (i<ylen) && (t==true); i++) {
        t = (x.at(i) != y.at(i)) ? x.substr(i+1) == y.substr(i) : t;
      }
    }
  }
  else if(xlen < ylen) {
    //Makes sure y isn't more than 1 character larger than x
    if((ylen-xlen) > 1) {
      t = false;
    }
    //y is exactly 1 character larger than x
    else {
      //Compares both strings and makes sure that they are the same except for 1 character
      for(int i=0; (i<xlen) && (t==true); i++) {
        t = (y.at(i) != x.at(i)) ? y.substr(i+1) == x.substr(i) : t;
      }
    }
  }
  //Where xlen == ylen
  else {
    int count = 0;
    //Compares both strings and adds up the number of differences
    for(int i=0; i<xlen; i++) {
      count = (x.at(i) != y.at(i)) ? (count+1) : count;
    }
    //Makes sure that there is exactly 1 different character between the strings
    t = (count==1);
  }
  return t;
}

int main() {
  StringDeque* sdq =  new StringDeque(100);
  string infileName = "words.txt";
  ifstream* INFILEp = new ifstream(infileName.c_str(), ios_base::in);
  string word;

  while(getline(*INFILEp, word)) {
    //Start out with the first word in words.txt
    if(sdq->isEmpty()) {
      sdq->pushFront(word);
    }
    //pushFront when word is neighbor to both the front and rear items
    else if((ed1(sdq->front(), word)==true) && (ed1(sdq->rear(), word)==true)) {
      sdq->pushFront(word);
    }
    else if(ed1(sdq->front(), word) == true) {
      sdq->pushFront(word);
    }
    else if(ed1(sdq->rear(), word) == true) {
      sdq->pushRear(word);
    }
    //word is not a neighbor of front or rear
    else {
      //Do Nothing
    }
  }
  cout << sdq->toString() << endl;
  INFILEp->close();

  /* Proof of concept tests. Makes sure that all of the functions are responding properly.  Uncomment if you would like to use it!
  //ed1 Tests
  string a = "abc";
  string b = "abc";
  cout << "0 expected, ed1 output is: " << ed1(a, b) << endl;
  string c = "abc";
  string d = "abd";
  cout << "1 expected, ed1 output is: " << ed1(c, d) << endl;
  string e = "abc";
  string f = "abcd";
  cout << "1 expected, ed1 output is: " << ed1(e, f) << endl;
  string g = "abcd";
  string h = "abc";
  cout << "1 expected, ed1 output is: " << ed1(g, h) << endl;
  string i = "abc";
  string j = "abcde";
  cout << "0 expected, ed1 output is: " << ed1(i, j) << endl;
  string k = "abcde";
  string l = "abc";
  cout << "0 expected, ed1 output is: " << ed1(k, l) << endl;

  //StringDeque Tests
  StringDeque* sd = new StringDeque(100);
  sd->pushFront("Test-1a");
  sd->pushFront("Test-1b");
  sd->pushRear("Test-1c");
  sd->popFront();
  sd->popRear();
  sd->popRear();
  sd->pushRear("Test-1d");
  sd->pushRear("Test-1e");
  sd->pushFront("Test-1f");
  cout << "2 cerr error messages expected:" << endl;
  sd->popRear();
  sd->popFront();
  sd->popFront();
  sd->popFront();
  sd->popRear();
  cout << "The Deque size should be 0 it is currently: " << sd->size() << endl;
  cout << "The Deque should now return 1 for true when isEmpty is called. It returns: " << sd->isEmpty() << endl;
  sd->pushFront("f1");
  sd->pushRear("r1");
  sd->pushFront("f2");
  sd->pushFront("f3");
  sd->pushRear("r2");
  sd->pushFront("f4");
  sd->pushRear("r3");
  cout << sd->toString() << "\nSize of StringDeque is: " << sd->size() << endl;
  // */
  return (0);
}
