/** File "UserBaseBTG.h", by Bethany Griswold. Template class for Project 2
in CSE250, Fall 2010.*/

#ifndef USERBASEBTG_H_
#define USERBASEBTG_H_

#include <bitset>
#include <vector>

class User {
   private:
     //A vector of the ids of movies the user has rated.
     std::vector<size_t>* moviesRated;
     //A vector which corresponds index-to-index to moviesRated giving star ratings given for each
     std::vector<size_t>* moviesStars;
     //Gives number in genre rated for each genre by index - in float to avoid heavy type casting
     std::vector<float>* genreNumber;
     //Gives the total number of stars for each genre - in float to avoid heavy type casting
     std::vector<float>* genreStars;
     //Gives average rating for each genre by index.
     std::vector<float>* avgRatings;
     //Gives the preference ratings for each genre
     std::vector<float>* prefRating;
     //Gives an ordered list of top movie recomendations index 0 being the top
     std::vector<size_t>* movieRecom;
     //Gives a corresponding list of desirablity to movieRecom
     std::vector<float>* movieDesire;
     size_t index;

   public:
     User(size_t givenIndex, size_t top)
       : moviesRated(new std::vector<size_t>())
       , moviesStars(new std::vector<size_t>())
       , genreNumber(new std::vector<float>(6, 0))
       , genreStars(new std::vector<float>(6, 0))
       , avgRatings(new std::vector<float>(6, 0))
       , prefRating(new std::vector<float>(6, 0))
       , movieRecom(new std::vector<size_t>(top, 0))
       , movieDesire(new std::vector<float>(top, 0))
       , index(givenIndex) {
       movieRecom->reserve(50);
       movieDesire->reserve(50);
     }

     virtual ~User() {
       delete moviesRated, moviesStars, genreNumber, genreStars, 
              avgRatings, prefRating, movieRecom, movieDesire;
     }

     size_t getIndex() const {
       return index;
     }

     //gets a vector of the ids of all movies the user has rated
     std::vector<size_t>* getRated() const {
       return moviesRated;
     }

     std::vector<size_t>* getStars() const {
       return moviesStars;
     }

     //adds a rating subsequently calculating average ratings for each genre by user
     void addRating(size_t movie, size_t rating, std::bitset<6>* genres) {
       moviesRated->push_back(movie);
       moviesStars->push_back(rating);
       for(size_t b = 0; b < 6; ++b) {
         if(genres->test(b)) {
           genreNumber->at(b) = ++(genreNumber->at(b));
           genreStars->at(b) += float(rating);
           avgRatings->at(b) = (genreStars->at(b))/(genreNumber->at(b));
         }
       }
     }

     //returns an arrayList of floats, the preference rating of each genre
     // by index
     float getAvg(size_t genre) const {
        return avgRatings->at(genre);
     }

     //sets the preference rating for genre
     void setPref(size_t genre, float pref) {
       prefRating->at(genre)=pref;
     }

     float getPref(size_t genre) const {
       return prefRating->at(genre);
     }

     float getGenreNumber(size_t genre) const {
       return genreNumber->at(genre);
     }

     std::vector<size_t>* getRecom() const {
       return movieRecom;
     }

     std::vector<float>* getDesire() const {
       return movieDesire;
     }

     //determines if a movie has been seen by user
     bool seen(size_t mindex) const {
       bool b = false;
       for(size_t i = 0; i < moviesRated->size(); ++i) {
         if(mindex == moviesRated->at(i)) {
           b = true;
           break;
         }
       }
       return b;
     }
};

class UserBase {
 private:
   std::vector<User*>* users;

 public:
   UserBase(size_t x, size_t top)
    : users(new std::vector<User*>()) {
    users->reserve(x);
    for(int i = 1; i <= x; ++i) {
      users->push_back(new User(i, top));
    }
   }

   virtual ~UserBase() {
     delete users;
   }

   //Will delegate to the User at element user of users to add a new rating
   //for a movie.
   void addRating(size_t user,size_t movie,size_t rating,
         std::bitset<6>* genres){ 
      users->at(user-1)->addRating(movie, rating, genres);
   }

   //gets the User with id of user at location on users of user-1
   //assumes users were added in order of id.
   User* getUser(size_t user) const { 
      return users->at(user-1);
   }
};

#endif
