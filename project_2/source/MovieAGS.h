/* File "MovieAGS.h", by AGS for CSE250, Fall 2010, Project 2. */

#ifndef MOVIEAGS_H_
#define MOVIEAGS_H_

#include <stdexcept>
#include <bitset>
#include <string>

#define GENRE_BITCOUNT 6
#define DEFAULT_RATING 0
#define DEFAULT_VOTES 0
#define DEFAULT_STARS 0
#define MIN_STARS 1
#define MAX_STARS 5

/** A Movie which holds an index, title, year, a set of genres, and an average rating. */
class Movie {
  private:
   std::bitset<GENRE_BITCOUNT>* genres;
   std::string* title;
   size_t index; 
   size_t year;
   float votes;
   float totalStars;
   float rating;
 
  public:
    /** Creates a new Movie.
        @param givenIndex the index of the Movie
        @param givenTitle the title of the Movie
        @param givenYear the year of the Movie
        @param givenGenres a set of genres the Movie belongs to */
    Movie(size_t givenIndex, std::string* givenTitle, size_t givenYear, std::bitset<GENRE_BITCOUNT>* givenGenres)
      : index(givenIndex)
      , title(givenTitle)
      , year(givenYear)
      , genres(givenGenres)
      , rating(DEFAULT_RATING)
      , votes(DEFAULT_VOTES)
      , totalStars(DEFAULT_STARS) {
    }

    /** Movie desturctor. */
    virtual ~Movie() {
      delete genres;
      delete title;
    }

    /** Acquires the index of the Movie.
        @return the index of the Movie */
    size_t getIndex() const {
      return index;
    }

    /** Acquires the title of the Movie.
        @return the title of the Movie */
    std::string* getTitle() const {
      return title;
    }

    /** Acquires the year of the Movie.
        @return the year of the Movie */
    size_t getYear() const {
      return year;
    }

    /** Acquires the votes given to a Movie.
        @return the votes given to the Movie */
    float getVotes() const {
      return votes;
    }

    /** Returns the condensed genres associated with the Movie.
        A 1 means association with a genre while a 0 means no association.
        Bit postions association (counted from the rightmost bit):
        0 - Action
        1 - Noir
        2 - Light
        3 - Serious
        4 - Fantasy
        5 - History
        @return the condensed genres associated with the Movie */
    std::bitset<GENRE_BITCOUNT>* getGenres() const {
      return genres;
    }

    /** Allows for the Movie to be rated.  Computes the average rating of a Movie.
        @param stars the given rating from 1 to 5 stars */
    void rate(size_t stars) {
      if(MIN_STARS <= stars <= MAX_STARS) {
        ++votes;
        totalStars += stars;
        rating = totalStars/votes;
      }
      else {
        throw std::invalid_argument("Invalid star rating");
      }
    }

    /** Acquires the average rating of the Movie.
        @return the average rating of the Movie */
    float getRating() const {
      return rating;
    }
};

#endif
