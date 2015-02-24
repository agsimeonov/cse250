/**File Deque2FlexI.h, by KWR for CSE250, Fall 2010.
   Iterator version of Deque2Flex.h

   Adapter widget so clients can use FlexArray implementation
   of the Deque operations by changing only an include line.
   Note that since we don't intend to override the adapter widget
   itself, the methods are not "virtual"---any "virtuality" would
   be obtained by the implementation class itself, of which this
   class "knows" only the public interface.

   Depending on your names for "optional" methods in Queue<T> and/or Deque<T>,
   YOU MAY NEED TO EDIT THIS CLASS.  
   Because you need to edit the #include line anyway, directions say to
   submit it for the 11/5 checkpoint, alongside your FlexArray and client.
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
   bool ifFull() const { return myImpl->full(); }

   //Item Accessors: if you used different names, substitute them here.
   T front() const { return *(myImpl->begin()); }  //now cell 0 is the front
   T rear() const { return *(myImpl->rbegin()); }
   T front2nd() const { return *(++(myImpl->begin())); }
   T rear2nd() const { return  myImpl->at(myImpl->size()-2); }
   //Note that responsibility is on caller to guarantee that the container
   //has at least 1 or 2 items, to avoid an exception that might be raised
   //by the implementing class.  (If you code a full bi-directional iterator,
   //you may re-code rear2nd() to use it.)

   void pushFront(const T& newItem) {
      myImpl->insert(myImpl->begin(),newItem);
   }

   void pushRear(const T& item) {
      myImpl->insert(myImpl->end(), item);
   }

   T popFront() {
      T item = *(myImpl->begin());
      myImpl->erase(myImpl->begin());
      return item;
   }

   T popRear() {
      if (myImpl->empty()) { cerr << "Popping on empty!" << endl; }
      T item = *(myImpl->rbegin());
      myImpl->erase(myImpl->rbegin());
      return item;
   }

   string toString() const {
      string out;
      //size_t sz = myImpl->size();
      for (typename FlexArray<T>::iterator itr = myImpl->begin();
           itr != myImpl->end(); ++itr) {
         out += (*itr).str() + " ";
      }   
      return out;
   }
};
      
#endif
