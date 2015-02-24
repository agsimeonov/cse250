/** File "DequeClientAGS.cpp", by Alexander Georgiev Simeonov
    for CSE250, Fall 2010. Assignment 4.
*/
#include "DequeAGS.h" //Includes Queue and Deque template classes
#include <fstream>
#include "StringWrap.h"

using namespace std;

/** Determines whether two strings are neighbors (have edit distance of 1)
    @param x the first string
    @param y the second string
    @return true if and only if x and y are neighbors
*/
bool ed1(const string& x, const string& y) {
  int xlen = x.length();
  int ylen = y.length();
  bool t = true;
  
  if(xlen > ylen) {
    //Makes sure x is not more than 1 character longer than y
    if((xlen-ylen) > 1) {
      t = false;
    }
    //x is exactly 1 character longer than y
    else {
      //Determines whether the strings are neighbors
      for(int i=0; (i<ylen) && (t==true); i++) {
        t = (x.at(i) != y.at(i)) ? x.substr(i+1) == y.substr(i) : t;
      }
    }
  }
  else if(xlen < ylen) {
    //Makes sure y is not more than 1 character longer than y
    if((ylen-xlen) > 1) {
      t = false;
    }
    //y is exactly 1 character longer than y
    else {
      //Determines whether the strings are neighbors
      for(int i=0; (i<xlen) && (t==true); i++) {
        t = (y.at(i) != x.at(i)) ? y.substr(i+1) == x.substr(i) : t;
      }
    }
  }
  //Where xlen == ylen
  else {
    int count = 0;
    for(int i=0; i<xlen; i++) {
      //Compares the strings and adds up the number of differences
      count = (x.at(i) != y.at(i)) ? (count+1) : count;
    }
    //Returns true if there is exactly 1 difference
    t = (count==1);
  }
  return t;
}

int main(int argc, char* argv[]) {
  vector<Deque<StringWrap>*>* chains = new vector<Deque<StringWrap>*>();
  string infileName = (argc==1) ? "" : argv[1];
  ifstream* INFILEp = new ifstream(infileName.c_str(), ios_base::in);
  int n = (argc==2) ? 1 : atoi(argv[2]); //Intended for debugging
  int numItems = 0; //Intended for debugging
  //Intended for debugging - sets the vector size of each deque in chains
  int numDeque = (argc==4) ? atoi(argv[3]) : 100;
  //Holds the index of the longest deques in chains
  vector<int>* longestWord = new vector<int>;
  int longestWordLength = 0;
  string word;

  while((*INFILEp) >> word && numItems < n) {
    StringWrap sw(word);
    bool attachedWord;
    //Makes sure that the length of word is at least 3
    if(word.length()<3) {
      attachedWord = true;
    }
    else {
      sw.makeLower();
      sw.trimNonAlpha();
      attachedWord = ((sw.str().length()<3 || (!sw.isAlpha()))) ? true : false;
    }
    numItems = (argc==2) ? numItems : ++numItems;
   
    //Attaches a word to an already existing chain if possible
    for(int i=0; (i<chains->size())&&(attachedWord == false); i++) {
      //Makes sure a word chain never contains sequences such as "hip hop hip hop"
      if((sw.str().compare(chains->at(i)->previousFront().str())==0) || 
         (sw.str().compare(chains->at(i)->previousRear().str()))==0) {
        attachedWord = true;
      }
      else if(ed1(chains->at(i)->front().str(), sw.str()) == true) {
        chains->at(i)->pushFront(sw);
        attachedWord = true;
      }
      else if(ed1(chains->at(i)->rear().str(), sw.str()) == true) {
        chains->at(i)->pushRear(sw);
        attachedWord = true;
      }
      else {
        //Do Nothing
      }
      
      //Helps determine the chains with the longest words
      switch(attachedWord) {
        case true :
          if(sw.str().length() > longestWordLength) {
            longestWord->clear();
            longestWord->push_back(i);
            longestWordLength = sw.str().length();
          }
          else if(sw.str().length() == longestWordLength) {
            if(chains->at(i) == chains->at(longestWord->back())) {
              //Do Nothing
            }
            else {
              longestWord->push_back(i);
            }
          }
          break;
        default :
          break;
      }
    }
    
    //Creates a new chain when word can not be attached to any
    if(!attachedWord) {
      Deque<StringWrap>* sq = new Deque<StringWrap>(numDeque);
      sq->pushFront(sw);
      chains->push_back(sq);
      cout << "Added chain number " << chains->size() << endl;
      
      //Helps determine the chains with the longest words
      if(sw.str().length() > longestWordLength) {
        longestWord->clear();
        longestWord->push_back(chains->size()-1);
        longestWordLength = sw.str().length();
      }
      else if(sw.str().length() == longestWordLength) {
        longestWord->push_back(chains->size()-1);
      }
      else {
        //Do Nothing
      }
    }
  }

  if(argc == 1) {
    cerr << "Please specify an appropriate filename!" << endl;
  }
  //Makes sure that an appropriate number of words is specified in argv[2]
  else if((argc==3) && (atoi(argv[2])<1)) {
    cerr << "No word chains were formed." << endl;
    cerr << "Please make sure to specify a number of words higher than 0!"<< endl;
  }
  else if(chains->size() == 0) {
    cout << "No word chains were formed from the words read in " <<  argv[1] << endl;
  }
  else {
    vector<Deque<StringWrap>*>* report = new vector<Deque<StringWrap>*>();
    cout << "Longest chain(s) found:" << endl;
    //Determines what the longest word chains are
    for(int i=0; i<chains->size(); i++) {
      if(report->empty()) {
        report->push_back(chains->at(i));
      }
      else if(chains->at(i)->size() > report->back()->size()) {
        report->clear();
        report->push_back(chains->at(i));
      }
      else if(chains->at(i)->size() == report->back()->size()) {
        report->push_back(chains->at(i));
      }
      else {
        //Do Nothing
      }
    }
    //Outputs the longest word chains
    for(int i=0; i<report->size(); i++) {
      cout << report->at(i)->toString() << endl;
    }
    delete report;

    cout << "Chain(s) with the longest word(s):" << endl;
    //Outputs the chains with the longest words
    for(int i=0; i<longestWord->size(); i++) {
      cout << chains->at(longestWord->at(i))->toString() << endl;    
    }
  }
  INFILEp->close();
  return (0);
}
