/**File Deque2FlexAGS.h, by KWR for CSE250, Fall 2010, edited by AGS.
   Adapter widget so clients can use FlexArray implementation
   of the Deque operations by changing only an include line.
   Note that since we don't intend to override the adapter widget
   itself, the methods are not "virtual"---any "virtuality" would
   be obtained by the implementation class itself, of which this
   class "knows" only the public interface.

   Depending on your names for "optional" methods in Queue<T> and/or Deque<T>,
   YOU MAY NEED TO EDIT THIS CLASS.  
   Because you need to edit the #include line anyway, directions say to
   submit it for the 10/29 checkpoint, alongside your FlexArray and client.
 */

#ifndef DEQUE2FLEX_H_
#define DEQUE2FLEX_H_

#include "FlexArrayAGS.h"   //substitute your implementation

template <class T>          //REQ: T() and T.str()
class Deque {               //pretends to be a Deque
   FlexArray<T>* myImpl;
 public:
   Deque<T>(int maxAdds) : myImpl(new FlexArray<T>()) { }
   //ignore maxAdds since now there is no upper size limit
   virtual ~Deque() { delete(myImpl); }

   size_t size() const { return myImpl->size(); }
   bool isEmpty() const { return myImpl->empty(); }
   bool isFull() const { return myImpl->full(); }

   T front() const { return myImpl->at(0); }  //now cell 0 is the front
   T rear() const { return myImpl->at(myImpl->size()-1); }
   T front2nd() const { return myImpl->at(1); }
   T rear2nd() const { return  myImpl->at(myImpl->size()-2); }
   //Note that responsibility is on caller to guarantee that the container
   //has at least 1 or 2 items, to avoid an exception that might be raised
   //by the implementing class.  If clients used different optional names such
   //as "getFront()" or "peek---", you can add or substitute those names here.

   void pushFront(const T& newItem) {
      myImpl->insert(0,newItem);
   }

   void pushRear(const T& item) {
      myImpl->insert(myImpl->size(), item);
   }

   T popFront() {
      T item = myImpl->at(0);
      myImpl->erase(0);
      return item;
   }

   T popRear() {
      if (myImpl->empty()) { cerr << "Popping on empty!" << endl; }
      size_t lastIndex = myImpl->size() - 1;
      T item = myImpl->at(lastIndex);
      myImpl->erase(lastIndex);
      return item;
   }

   string toString() const {
      string out;
      size_t sz = myImpl->size();
      for (size_t i = 0; i < sz; i++) {
         out += myImpl->at(i).str() + " ";
      }   
      return out;
   }
};
      
#endif
