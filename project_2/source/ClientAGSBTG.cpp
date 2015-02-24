#include <vector>
#include <iostream>
#include <fstream>
#include "UserBaseBTG.h"
#include "MovieBaseAGS.h"
#include "DataParserAGSBTG.h"

#define MB_SIZE 1682
#define UB_SIZE 943

int main(int argc, char* argv[]) {
  std::cout << "***\n";
  std::cout << "Creating and populating databases...\t";

  UserBase* ub;
  switch(argc) {
    case 2:
      if(atoi(argv[1]) != 0) {
        ub = new UserBase(UB_SIZE, atoi(argv[1]));
        break;
      }
    default:
      std::cout << "\nUsing default recommendation size of 50...\t";
      ub = new UserBase(UB_SIZE, 50);
      break;
  }

  MovieBase* mb = new MovieBase("u.item", MB_SIZE);
  DataParser* dp = new DataParser("u.data");

  size_t uindex;
  size_t mindex;
  size_t rating;
  while(dp->hasNext()) {
    uindex = dp->next();
    mindex = dp->next();
    rating = dp->next();
    ub->addRating(uindex, mindex, rating, mb->getMovie(mindex)->getGenres());
    mb->getMovie(mindex)->rate(rating);
  }

  std::cout << "Done!\n";
  std::cout << "Calculating preference factors...\t";

  User* u;
  std::vector<size_t>* moviesRated;
  //The total of overall averages for each genre in all the movies rated by a user
  std::vector<float>* totalAvgGenre = new std::vector<float>(6, 0);
  //Acquires and assigns the preference factor
  for(size_t x = 1; x <= UB_SIZE; ++x) {
    u = ub->getUser(x);
    moviesRated = u->getRated();
    for(size_t i = 0; i < moviesRated->size(); ++i) {
      for(size_t b = 0; b < 6; ++b) {
        if(mb->getMovie(moviesRated->at(i))->getGenres()->test(b)) {
          totalAvgGenre->at(b) += mb->getMovie(moviesRated->at(i))->getRating();
        }
      }
    }
    for(size_t p = 0; p < 6; ++p) {
      if(u->getAvg(p) != 0) {
        u->setPref(p, (u->getAvg(p)/(totalAvgGenre->at(p)/u->getGenreNumber(p))));
      }
      totalAvgGenre->at(p) = 0;
    }
  }
  delete totalAvgGenre;

  std::cout << "Done!\n";
  std::cout << "Calculating top " <<  ub->getUser(1)->getRecom()->size();
  std::cout << " recommendations...\t";

  Movie* m;
  float desired;
  std::vector<float>::iterator di;
  //std::vector<size_t>::iterator iter;
  //Determines the top movie recommendations for each user
  for(size_t usr = 1; usr <= UB_SIZE; ++usr) {
    u = ub->getUser(usr);
    for(size_t mv = 1; mv <= MB_SIZE; ++mv) {
      m = mb->getMovie(mv);
      desired = 0;
      if(u->seen(mv) || (m->getVotes() < 10)) {
        //Do nothing
      }
      else {
        for(size_t g = 0; g < 6; ++g) {
          if(m->getGenres()->test(g)) {
            if(desired < (u->getPref(g) * m->getRating())) {
              desired = (u->getPref(g) * m->getRating());
            }
          }
        }

        di = u->getDesire()->begin();
        for(std::vector<size_t>::iterator iter = u->getRecom()->begin(); 
            iter != u->getRecom()->end(); ++iter) {
          if((*iter == 0) || (desired > *di)) {
            u->getRecom()->insert(iter, mv);
            u->getRecom()->pop_back();
            u->getDesire()->insert(di, desired);
            u->getDesire()->pop_back();
            break;
          }
          ++di;
        }
      }
    }
  }  
  std::cout << "Done!\n";
  std::cout << "\nDatabases created, populated, preferences and recommendations calculated!\n\n";
  std::cout << "Dumping output to \"ClientOutput.txt\"...\t";

  std::ofstream* ofs = new std::ofstream("ClientOutput.txt", std::ios_base::out);
  for(size_t o = 1; o <= UB_SIZE; ++o) {
    *ofs << "User #" << o << " recommendations:\n";
    for(int mov = 0; mov < ub->getUser(o)->getRecom()->size(); ++mov) {
      *ofs << "\t" << (mov + 1) << ")\n";
      *ofs << "\t\tIndex - ";
      *ofs << mb->getMovie(ub->getUser(o)->getRecom()->at(mov))->getIndex();
      *ofs << "\n\t\tTitle - "; 
      *ofs << *(mb->getMovie(ub->getUser(o)->getRecom()->at(mov))->getTitle());
      *ofs << "\n\t\tYear - "; 
      *ofs << mb->getMovie(ub->getUser(o)->getRecom()->at(mov))->getYear();
      *ofs << "\n";
    }
  }
  ofs->close();

  std::cout << "Done!\n";
  std::cout << "***\n";

  delete ub, mb, dp;
  return 0;
}
