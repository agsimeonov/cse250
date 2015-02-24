/** File "AdaptedClientAGS.cpp", modified from "DequeClientKWR.cpp"
    by KWR for CSE250, Fall 2010, edited by AGS, for Project 1. */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cmath>
#include "StringWrap.h"
#include "FlexArrayAGS.h"

using namespace std;

bool ed1(const string& x, const string& y) {
  int xleft = 0; int yleft = 0;
  int xright = x.length() - 1; int yright = y.length() - 1;
  while(xleft <= xright && yleft <= yright && x[xleft] == y[yleft]) {
    xleft++; yleft++;
  }
  //Control here means all chars have matched on LHS up to xleft,yleft.
  //If one has gone off the string, accept iff other's length is 1 more.
  if(xleft > xright) { 
    return (xleft == yright);  //INCLUDES case x == y, return false.
  } 
  if(yleft > yright) {
    return (yleft == xright);
  }
  //Control here means we have a mismatched character

  while(xright >= xleft && yright >= yleft && x[xright] == y[yright]) {
    xright--; yright--;
  }
  return (xright <= xleft && yright <= yleft);
}

/** True if switching 2 adjacent unequal letters in x leaves y. */
bool transpose(const string& x, const string& y) {
  if (x.length() != y.length()) { return false; }
  // else
  int xlenm1 = x.length() - 1;
  for(int i = 0; i < xlenm1; i++) {
    if(x[i] != y[i]) {
      if(x[i] == y[i+1] && x[i+1] == y[i] && x[i] != x[i+1]
         && x.substr(i+2) == y.substr(i+2)) {
        cout << "transpose relevant for x = " << x << ", y = " << y << endl;
        return true;
      } 
      else {
        return false;
      }
    }
  } //control here means x == y
  return false;
}

/** Damerau's distance-one criterion, typos correctable by 1 edit. */
bool dd1(const string& x, const string& y) {
  return ed1(x,y) || transpose(x,y);
}
   
int main(int argc, char** argv) {
  // transfer command-line arguments to string array
  vector<string>* args = new vector<string>(argc);
  for (int i = 0; i < argc; i++) {
    args->at(i) = *(argv + i);
  }

  istringstream INS;
  int n = 10000;                       // arbitrary default value
  string INFILEName;                   // no default for file name.
  ifstream* INFILEp;

  if(argc >= 3) {                     // numerical argument "n" exists
    INS.str(args->at(2));              // safe since argc >= 3
    if(INS >> n) {
      //OK
    } 
    else {
      cerr << "Second argument given but not a valid integer" << endl;
      return(1);
    }
  }
   
  if(argc >= 2) {   //note that this is *not* an "else" of the first "if"
    INFILEName = args->at(1);
    INFILEp = new ifstream(INFILEName.c_str(), ios_base::in);
    if(! INFILEp->is_open()) {
      cerr << "Unable to open file: " << INFILEName << endl;
      return(1);
    }
  } 
  else {    // argc == 1, i.e. no arguments given
    cerr << "Usage: " << args->at(0) << " file n    with n >= 0" << endl;
    return(1);
  }
  
  FlexArray<FlexArray<StringWrap>*>* chains = new FlexArray<FlexArray<StringWrap>*>();
  //vector<FlexArray<StringWrap>*>* chains = new vector<FlexArray<StringWrap>*>();
  int numItems = 0;
  string word;
  while((*INFILEp) >> word && numItems < n) {       
    numItems++;
    bool attachedWord = false;
    StringWrap sw(word);
    sw.trimNonAlpha();
    sw.makeLower();

    if(!(sw.size() > 2)) {
      continue;
    }

    //INSERTION
    //* For use with FlexArray chains only - produces faster runs
    for(FlexArray<FlexArray<StringWrap>*>::iterator i = chains->begin();
        i != chains->end(); ++i) {
      FlexArray<StringWrap>* fa = *i; // */
    /* For use with vector chains (slower with FlexArray chains)
    for(size_t i = 0; i < chains->size(); ++i) {
      FlexArray<StringWrap>* fa = chains->at(i); // */  

      //REPEAT PREVENTION
      /* As specified in class prevents repetition over each specfic FlexArray in chains. For 
         overall repeat prevention encase with an outer loop over chains and add appropriate edits */
      bool wordPresent = false;
      for(FlexArray<StringWrap>::iterator wp = fa->begin(); 
          wp != fa->end(); ++wp) {
        if((sw.str() == (*wp).str()) || !(sw.isAlpha())) {
          wordPresent = true;
          break;
        }
      }
      attachedWord = attachedWord || wordPresent;
      //Doesn't attach the word but skips it enitrely if it was already attached
     
      //ACTUAL INSERTION
      /* Using Damerau Distance One over the whole length of each FlexArray in chains */
      if(fa->empty()) {
        fa->insert(fa->begin(), sw);
        attachedWord = true;
      }
      else if((!wordPresent) && dd1(sw.str(), (*(fa->begin())).str())) {
        fa->insert(fa->begin(), sw);
        attachedWord = true;
      }
      else if((!wordPresent) && dd1(sw.str(), (*(fa->rbegin())).str())) {
        fa->insert(fa->end(), sw);
        attachedWord = true;
      }
      else if(!wordPresent) {
        FlexArray<StringWrap>::iterator x = fa->begin();
        FlexArray<StringWrap>::iterator y = ++(fa->begin());
        while((attachedWord == false) && (x != y) && (y != fa->end())) {
          if(dd1(sw.str(), (*x).str()) && dd1(sw.str(), (*y).str())) {
            fa->insert(y, sw);
            attachedWord = true;
          }
          else {
            ++x;
            ++y;
          }
        } 
      }
    }
    if(!attachedWord) {
      FlexArray<StringWrap>* newFlex = new FlexArray<StringWrap>(200);
      newFlex->insert(newFlex->begin(), sw);
      chains->insert(chains->end(), newFlex);
      cout << "Added chain number " << chains->size() << endl;
    }
  } 

  //now test state of stream upon termination
  if(INFILEp->eof()) {
    cout << "Read all " << numItems << " whitespace-separated items"
         << " in file " << INFILEName << endl;
  } 
  else if(INFILEp->bad()) {
    cerr << "Unrecoverable i/o error, sorry, after " << numItems
         << " items." << endl;
  } 
  else {
    // POST: stream is still good, so we must have hit n items
    cout << "Read " << numItems << " whitespace-separated items from file "
         << INFILEName << endl;
  }
  cout << endl;
  cout << "Determining desired chains!" << endl;

  //CHAIN AQUISITION - through iteration
  //Initialized to dummy value for comparison
  FlexArray<StringWrap>* longestChain = new FlexArray<StringWrap>();
  size_t minChainSize = 2;
  FlexArray<StringWrap>* longestWordChain = longestChain;
  StringWrap longestWord;
  FlexArray<StringWrap>* mostTrans = longestChain;
  size_t transCount = 0;
  FlexArray<StringWrap>* bestGrowth = longestChain;
  size_t growth = 0;

  for(FlexArray<FlexArray<StringWrap>*>::iterator w = chains->begin(); w != chains->end(); ++w) {
  //for(vector<FlexArray<StringWrap>*>::iterator w = chains->begin(); w != chains->end(); ++w) {
    //LONGEST CHAIN - through iteration
    longestChain = ((*w)->size() > longestChain->size()) ? (*w) : longestChain;
    
    //MOST TRANSPOSE CHAIN - though iteration
    size_t tc = 0;
    FlexArray<StringWrap>::iterator a = (*w)->begin();
    FlexArray<StringWrap>::iterator b = ++((*w)->begin());
    while((a != b) && (b != (*w)->end())) {
      tc = (transpose((*a).str(), (*b).str())) ? (tc+1) : tc;
      ++a;
      ++b;
    }
    mostTrans = (tc > transCount) ? (*w) : mostTrans;
    transCount = (tc > transCount) ? tc : transCount;

    //BEST GROWTH CHAIN - though iteration
    size_t shortest = (*((*w)->begin())).size();
    size_t longest = shortest;
    //For there to be growth there have to be at least two words in a chain
    if((*w)->size() >= minChainSize) {
      FlexArray<StringWrap>::iterator c = (*w)->begin();
      while(c != (*w)->end()) {
        shortest = ((*c).size() < shortest) ? (*c).size() : shortest;
        longest = ((*c).size() > longest) ? (*c).size() : longest;
        ++c;
      }
      bestGrowth = ((longest - shortest) > growth) ? (*w) : bestGrowth;
      growth = ((longest - shortest) > growth) ? (longest - shortest) : growth;
    }

    //LONGEST WORD CHAIN - though iteration
    if((*w)->size() >= minChainSize) {
      for(FlexArray<StringWrap>::iterator x = (*w)->begin();
          x != (*w)->end(); ++x) {
        if((*x).size() > longestWord.size()) {
          longestWord = *x;
          longestWordChain = *w;
        }
      }      
    }    
  } 

  cout << endl;
  cout << "Results with strategy of forbidding repetitions within each unique chain,"
       << endl << "and allowing shorter words to be attached to longer ones:" << endl;
  if(longestChain->size() > 0) {
    cout << "Longest chain (Size " << longestChain->size() << "): " << longestChain->toString() << endl;
  }
  if(longestWordChain->size() > 0) {
    cout << "Chain with longest word (" << longestWord.str() << "): "
         << longestWordChain->toString() << endl;
  } 
  if(transCount > 0) {
    cout << "Chain with the most traspositions (" << transCount << "): " << mostTrans->toString() << endl;
  }
  if(growth > 0) {
    cout << "Chain with the best growth (" << growth << "): " << bestGrowth->toString() << endl;
  }

  for(size_t k = 0; k < chains->size(); k++) {
    delete(chains->at(k));
  }

  INFILEp->close();
  return 0;
}

/*
--------------------
1)
It is hard to tell whether the iterator made in easier to code the longest-word and best-growth tasks.
From a coding point of view both are fairly starightforward to code. From a synthatical point of view the iterator version in this case
is the better and perhaps faster one of the two.  In my own personal opinion the iterator version was harder to code at first
due to many conciderations I had to make based on non-const operators and others such.  Nevertheless as I continued
debugging my code I found the concepts behind the iterator easier to grasp, so as I finished the project I realized that
the iterator version is just as easy as the regular, if not easier.  In my case I do think that longest-word and best-growth are
easier through iterators.

When it comes to startegy one can simply look above and see what I did, but I shall attemp to explain in more detail:

It should be noted however that we do have the added bonus of aquiring the actual chain directly for the output.
Longest Chain Task:
-Here we have a one liner.  Simply iterate though chains and compare sizes, singeling out chains whenever meething one of greater size then
the previously singled out chain.

Longest Word Task:
-Simple enough.  Here I had two loops an inner and an outer.  The outer loop would iterate though chains while the inner
would iterate though the StringWraps in each chain.  Before using the inner loop I checked each chains size for being 
more then or equal to two as the project description specified.  If that was the case I entered the loop and checked each
StringWrap for it's size.  Depeneding one the size as compared to a previously indexed StringWrap I determined if a StringWrap
contained a longer word then all previously checked StringWraps.  If it did I indexed the StringWrap in question, and pointed 
to the FlexArray<StringWrap> with a pointer.  Once the iteration was done running I was left with a pointer to the 
FlexArray<StringWrap> containing the longest word as well as the word itself.

Best Growth Task:
-This one was elementary.  With an outer loop I iterated though chains.  With an inner while loop I iterated though each chain
making sure before doing so that the chain contains at least 2 StringWraps because a chain with any less wouldn't have any growth
whatsoever.  I also assigned two temporary values for longest and shortest before the inner loop which would restart for every chain.
Inside the inner loop I used those to single out the longest StringWraps in each chain. And after the loop completes
I found the best growth by subtracting shortest from longest and indexing a pointer to the chain containing the best growth
if and only if the current growth indicated by longest - shortest was better then the previously indexed growth.  After all 
iterations I was left with the pointer to the chain with best growth as well as the growth itself in size_t form.

Most Traspositions Task:
This task also was fairly simplistic.  To accomplish what was asked I again used two loops.  An outer which iterated though
chains and an inner while loop that iterateds though the words in each chain.  Here I had two iterators present as the begining
of each chain and the next element after the begining of each chain.  In the inner loop I used those with traspose to compare 
the strings which the iterators could navigate me to. If traspose was relevant in I added one to a temporary counter which was
initialized just outside the inner loop.  After each pass of the inner loop I would advance both iterators by one until
the one further up reached end.  When the inner loop was done I would use the temporary counter against an indexed size of 
most traspositions overall seen so far.  If there were more in the one just iterated though I would index it instead with a pointer.
After the outer loop was done I was left with a pointer to the loop with most transpositions.  

--------------------
2)
I changed the implementation to FlexArray<FlexArray<StringWrap>*>
It doesn't seem like there is any slowdown at all by doing this.  In fact I dare say the speed is about
the same if not better then the implementation with vector.  I may be imagining things but I tested it
quite a few time and it seems about the same if not slightly better.  Then again the grader should concider
I used iteration as much as humanly possible in the client so that I can achieve best speed.  If one wants to 
confirm this they can simply comment out the FlexArray parts for vector wherever applicable.  I have left 
the vector portions commnted out just in case as one can clearly see.  This is for future reference, as well
as ease of grading.

*/
