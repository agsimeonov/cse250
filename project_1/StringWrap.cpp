/** File "StringWrap.cpp", by KWR for CSE250, Fall 2009.  
    Coded answer to Assignment 2, problem (3), together with "StringSeqs.cpp"
 */

#include "StringWrap.h"

StringWrap::StringWrap(string s) : st(s) { }   //no "explicit" here
StringWrap::StringWrap() : st("") { }

void StringWrap::trimNonAlpha() {              //no "virtual" here
   int sz = st.size();                      //size_t is like unsigned int
   int left = 0;
   int right = sz-1;

   // LOOP INV: all alpha chars are between left and right.
   // Note: right is on-end, rather than past-the-end, of this range

   while ((left < sz)
           && !( ('a' <= st[left] && st[left] <= 'z')
                || ('A' <= st[left] && st[left] <= 'Z') ) ) {
      left++;
   }
   // POST: if there are any alpha characters, left is on one
   while ((right >= 0)
           && !( ('a' <= st[right] && st[right] <= 'z')
                || ('A' <= st[right] && st[right] <= 'Z') ) ) {
      right--;
   }
   // POST: if any alpha chars, right is on one and >= left.
   // But if st is all-non-alpha, then right < left is possible.
   // Since the behavior of substr with a negative length is 
   // **formally undefined** in the C++ standard, we need one more test.

   int alphaLength = right - left + 1;  // + 1 since on-end

   st = (alphaLength <= 0) ? "" : st.substr(left,alphaLength);
}

void StringWrap::makeLower() {
   int sz = st.size();
   for (int i = 0; i < sz; i++) {
      if ('A' <= st[i] && st[i] <= 'Z') {
         st[i] += 32;
      }
   }
}

bool StringWrap::isAlpha() const {
   int sz = st.size();
   for (int i = 0; i < sz; i++) {
      if (!( ('a' <= st[i] && st[i] <= 'z')
            || ('A' <= st[i] && st[i] <= 'Z') ) ) {
         return false;
      }
   }
   return true;
}

bool StringWrap::isAllCaps() const {
   int sz = st.size();
   for (int i = 0; i < sz; i++) {
      if (! ('A' <= st[i] && st[i] <= 'Z') ) {
         return false;
      }
   }
   return true;
}

string StringWrap::str() const {
   return st;
}

size_t StringWrap::size() const {
   return st.size();
}

ostream& operator<< (ostream& out, const StringWrap& item) {
   out << item.str();
   return out;
}
