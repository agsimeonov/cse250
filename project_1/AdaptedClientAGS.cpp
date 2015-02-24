/** File "AdaptedClientAGS.cpp", by KWR for CSE250, Fall 2010, edited by AGS.  
    Assignment 4 answer.  Has lines that can be edited to implement
    various strategies.  As released on 11/3/10, it relies on accessor
    methods "front()" and "rear()" to implement the strategy of avoiding
    immediate repetitions, /and/ also "front2nd()" and "rear2nd()" to
    avoid switchback repetitions like "hip hop hip hop hip...".
    Hence if you use this client, you will need to adapt these methods.
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cmath>
#include "StringWrap.h"
//#include "DequeKWR.h"
//#include "Deque2FlexAGS.h"
#include "Deque2FlexIAGS.h"

using namespace std;

///*----------------------------------------------------
bool ed1(const string& x, const string& y) {
   int xleft = 0; int yleft = 0;
   int xright = x.length() - 1; int yright = y.length() - 1;
   while (xleft <= xright && yleft <= yright && x[xleft] == y[yleft]) {
      xleft++; yleft++;
   }
   //Control here means all chars have matched on LHS up to xleft,yleft.
   //If one has gone off the string, accept iff other's length is 1 more.
   if (xleft > xright) { 
      return (xleft == yright);  //INCLUDES case x == y, return false.
   } 
   if (yleft > yright) {
      return (yleft == xright);
   }
   //Control here means we have a mismatched character

   while (xright >= xleft && yright >= yleft && x[xright] == y[yright]) {
      xright--; yright--;
   }
   return (xright <= xleft && yright <= yleft);
}
//----------------------------------------------------*/
/*-----------------------------------------------------
bool ed1(const string& x, const string& y) {
   int xlen = x.length(); int ylen = y.length();
   if (fabs(double(xlen - ylen)) >= 2) { return false; }
   if (xlen == ylen) {
      int count = 0;
      for (int i = 0; i < xlen; i++) {
         if (x[i] != y[i]) { count++; }
      }
      return (count == 1);
   } else if (xlen == ylen + 1) {
      for (int i = 0; i < ylen; i++) {
         if (x[i] != y[i]) { 
            return (x.substr(i+1) == y.substr(i));
         }
      } //all-match means erase less char  in x.
      return true;
   } else {   //ylen == xlen + 1
     for (int i = 0; i < xlen; i++) {
         if (x[i] != y[i]) { 
            return (y.substr(i+1) == x.substr(i));
         }
      } //all-match means erase less char  in x.
      return true;
   }
}
--------------------------------------------------------*/

/** True if switching 2 adjacent unequal letters in x leaves y.
 */
bool transpose(const string& x, const string& y) {
   if (x.length() != y.length()) { return false; }
   // else
   int xlenm1 = x.length() - 1;
   for (int i = 0; i < xlenm1; i++) {
      if (x[i] != y[i]) {
         if (x[i] == y[i+1] && x[i+1] == y[i] && x[i] != x[i+1]
                  && x.substr(i+2) == y.substr(i+2)) {
            cout << "transpose relevant for x = " << x << ", y = " << y << endl;
            return true;
         } else {
            return false;
         }
      }
   } //control here means x == y
   return false;
}

/** Damerau's distance-one criterion, typos correctable by 1 edit.
 */
bool dd1(const string& x, const string& y) {
   return ed1(x,y) || transpose(x,y);
}
      
int main(int argc, char** argv) {
   // transfer command-line arguments to string array
   // remember unlike Java the executable name is the first argument
   //
   vector<string>* args = new vector<string>(argc);
   for (int i = 0; i < argc; i++) {
      args->at(i) = *(argv + i);         //equivalent to argv[i]
   }

   istringstream INS;
   int n = 10000;                        // arbitrary default value
   string INFILEName;                    // no default for file name.
   ifstream* INFILEp;

   if (argc >= 3) {                      // numerical argument "n" exists
      INS.str(args->at(2));              // safe since argc >= 3
      if (INS >> n) {
         //OK
      } else {
         cerr << "Second argument given but not a valid integer" << endl;
         return(1);
      }
   }
   if (argc >= 2) {   //note that this is *not* an "else" of the first "if"
      INFILEName = args->at(1);
      INFILEp = new ifstream(INFILEName.c_str(), ios_base::in);
      if (! INFILEp->is_open()) {
         cerr << "Unable to open file: " << INFILEName << endl;
         return(1);
      }
   } else {    // argc == 1, i.e. no arguments given
      cerr << "Usage: " << args->at(0) << " file n    with n >= 0" << endl;
      return(1);
   }

   //Deque<StringWrap>* sq = new Deque<StringWrap>(100);

   //cout << "ed1(the,the) is " << ed1("the","the") << endl;
   vector<Deque<StringWrap>*>* chains = new vector<Deque<StringWrap>*>();
   //held by pointer

   int numItems = 0;
   string word;
   while((*INFILEp) >> word && numItems < n) {       
      numItems++;
      bool attachedWord = false;
      //StringWrap* swp = new StringWrap(word);  //Not recommended here
      StringWrap sw(word);
      sw.trimNonAlpha();
      sw.makeLower();

      if (!(sw.size() > 2)) {   // && sw.isAlpha())) {
         continue;
      }
//legal but avoid swp->makeLower();   Deque<StringWrap
      word = sw.str();
      for (size_t i = 0; i < chains->size(); i++) {
         Deque<StringWrap>* sq = chains->at(i);
         bool wordPresent = (word == sq->rear().str()
                             || word == sq->front().str() );
         wordPresent = wordPresent || (sq->size() >= 2 &&
            (word == sq->rear2nd().str() || word == sq->front2nd().str() ) );
         attachedWord = attachedWord || wordPresent;
         if (sq->isEmpty()) {
            sq->pushFront(sw);
            attachedWord = true;
         } else if ((!wordPresent) && dd1(sw.str(),sq->front().str())) {
            //if (word.size() >= sq->front().str().size()) 
            {
               sq->pushFront(sw);
               attachedWord = true;
            }
         } else if ((!wordPresent) && dd1(sw.str(),sq->rear().str())) {
            //if (word.size() >= sq->rear().str().size()) 
            {
               sq->pushRear(sw);
               attachedWord = true;
            }
         } //else move on
      }
      if (!attachedWord) {   //start a new chain with the word
         Deque<StringWrap>* sq = new Deque<StringWrap>(200);
         sq->pushFront(sw);
         chains->push_back(sq);
         cout << "Added chain number " << chains->size() << endl;
      }
   }

   //now test state of stream upon termination

   if (INFILEp->eof()) {
      cout << "Read all " << numItems << " whitespace-separated items"
           << " in file " << INFILEName << endl;
   } else if (INFILEp->bad()) {
      cerr << "Unrecoverable i/o error, sorry, after " << numItems
           << " items." << endl;
   } else {
      // POST: stream is still good, so we must have hit n items
      cout << "Read " << numItems << " whitespace-separated items from file "
           << INFILEName << endl;
   }

   //Now find longest chain; choose first of equal-length ones.

   int indexLongest = -1;
   size_t longestLength = 0;
   size_t longestWord = 0;
   int indexLongestWord = -1;
   size_t minChainSize = 2;
   for (size_t i = 0; i < chains->size(); i++) {
      if (chains->at(i)->size() > longestLength) {
         indexLongest = i;
         longestLength = chains->at(i)->size();
      }
      Deque<StringWrap>* temp = new Deque<StringWrap>(100);
      while (!chains->at(i)->isEmpty()) {
         StringWrap sw2 = chains->at(i)->popRear();
         temp->pushFront(sw2);
         if (sw2.size() > longestWord && chains->at(i)->size() >= minChainSize)
         {
            longestWord = sw2.size();
            indexLongestWord = i;
         }
      }
      chains->at(i) = temp;
   }
   cout << "Results with strategy of forbidding both immediate and alternating repetitions,"
        << endl << "and allowing shorter words to be attached to longer ones." << endl;
   if (indexLongest > -1) {
      cout << "Longest chain: " + chains->at(indexLongest)->toString() << endl;
   }
   if (indexLongestWord > -1) {
      cout << "Chain with longest word: " 
           << chains->at(indexLongestWord)->toString() << endl;
   }
   //delete[] chains;

   for (size_t k = 0; k < chains->size(); k++) {
      delete(chains->at(k));
   }

   INFILEp->close();

   //Deque<int>* di = new Deque<int>(100);
   //di->pushFront(2);
   //di->pushRear(3);
   //cout << di->toString() << endl;
   return 0;
}
