/** File "MovieBaseAGS.h", by AGS for CSE250, Fall 2010, Project 2. */

#ifndef MOVIEBASEAGS_H_
#define MOVIEBASEAGS_H_

#include <vector>
#include <cstdlib>
#include "MovieAGS.h"
#include "PipeDelimAGSBTG.h"

#define UNKNOWN_STR "unknown"
#define UNKNOWN_YR ""
#define IS_UNKNOWN 0
#define TITLE_CROP 7
#define YEAR_START 7
#define YEAR_DIGITS 4
#define IS_GENRE 1
#define ACTION 0
#define NOIR 1
#define LIGHT 2
#define SERIOUS 3
#define FANTASY 4
#define HISTORY 5

/** A database of Movies. */
class MovieBase {
  private:
    std::vector<Movie*>* movies;
    PipeDelim* pd;

  public:
    /** Creates a new MovieBase
        @param fileName the name of a given file used to generate the MovieBase
        @param movieBaseSize the needed size of the MovieBase */
    MovieBase(char* fileName, size_t movieBaseSize)
      : movies(new std::vector<Movie*>())
      , pd(new PipeDelim(fileName)) {
      movies->reserve(movieBaseSize);
      while(pd->hasNext()) {
        //Index
        std::string* sIndex = pd->next();
        size_t index = atoi(sIndex->c_str());
        delete sIndex;
        //Title
        std::string* title = pd->next();
        if(!(title->compare(UNKNOWN_STR) == IS_UNKNOWN)) {
          title->resize(title->size() - TITLE_CROP);
        }
        //Year
        std::string* sYear = pd->next();
        size_t year = (*sYear == UNKNOWN_YR)
                    ? IS_UNKNOWN
                    : atoi(sYear->substr(YEAR_START, YEAR_DIGITS).c_str());       
        delete sYear;
        //Skip
        delete pd->next();
        delete pd->next();
        delete pd->next();			//Bit position 00 as of u.genre
        //Genres
        std::string* sAction = pd->next();	//Bit position 01 as of u.genre
        std::string* sAdventure = pd->next();	//Bit position 02 as of u.genre
        std::string* sAnimation = pd->next();	//Bit position 03 as of u.genre
        std::string* sChildrens = pd->next();	//Bit position 04 as of u.genre
        std::string* sComedy = pd->next();	//Bit position 05 as of u.genre
        std::string* sCrime = pd->next();	//Bit position 06 as of u.genre
        std::string* sDocumentary = pd->next();	//Bit position 07 as of u.genre
        std::string* sDrama = pd->next();	//Bit position 08 as of u.genre
        std::string* sFantasy = pd->next();	//Bit position 09 as of u.genre
        std::string* sFilmNoir = pd->next();	//Bit position 10 as of u.genre
        std::string* sHorror = pd->next();	//Bit position 11 as of u.genre
        std::string* sMusical = pd->next();	//Bit position 12 as of u.genre
        std::string* sMystery = pd->next();	//Bit position 13 as of u.genre
        std::string* sRomance = pd->next();	//Bit position 14 as of u.genre
        std::string* sSciFi = pd->next();	//Bit position 15 as of u.genre
        std::string* sThriller = pd->next();	//Bit position 16 as of u.genre
        std::string* sWar = pd->next();		//Bit position 17 as of u.genre
        std::string* sWestern = pd->next();	//Bit position 18 as of u.genre
        //Condensed Genres
        std::bitset<GENRE_BITCOUNT>* genres = new std::bitset<GENRE_BITCOUNT>();
        if((atoi(sAction->c_str())
          ||atoi(sAdventure->c_str())
          ||atoi(sThriller->c_str())
          ||atoi(sWestern->c_str())) == IS_GENRE) {
          genres->flip(ACTION);
        }
        if((atoi(sCrime->c_str())
          ||atoi(sFilmNoir->c_str())
          ||atoi(sHorror->c_str())
          ||atoi(sMystery->c_str())) == IS_GENRE) {
          genres->flip(NOIR);
        }
        if((atoi(sAnimation->c_str())
          ||atoi(sChildrens->c_str())
          ||atoi(sComedy->c_str())
          ||atoi(sMusical->c_str())) == IS_GENRE) {
          genres->flip(LIGHT);
        }
        if((atoi(sDrama->c_str())
          ||atoi(sRomance->c_str())) == IS_GENRE) {
          genres->flip(SERIOUS);
        }
        if((atoi(sSciFi->c_str())
          ||atoi(sFantasy->c_str())) == IS_GENRE) {
          genres->flip(FANTASY);
        }
        if((atoi(sWar->c_str())
          ||atoi(sDocumentary->c_str())) == IS_GENRE) {
          genres->flip(HISTORY);
        }
        delete sAction, sAdventure, sAnimation, sChildrens, sComedy, sCrime,
               sDocumentary, sDrama, sFantasy, sFilmNoir, sHorror, sMusical,
               sMystery, sRomance, sSciFi, sThriller, sWar, sWestern;
        //Adding Movie to MovieBase
        Movie* m = new Movie(index, title, year, genres);
        movies->push_back(m);
      }
    }

    /** MovieBase destructor. */
    virtual ~MovieBase() {
      delete movies;
      delete pd;
    }

    /** Acquires a pointer to a Movie in the MovieBase.
        @param movieIndex the index of the needed Movie
        @return the needed Movie */
    Movie* getMovie(size_t movieIndex) const {
      return movies->at(--movieIndex);
    }
};

#endif
