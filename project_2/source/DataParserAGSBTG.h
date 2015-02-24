/** File "DataParserAGSBTG.h", by AGS for CSE250, Fall 2010, Project 2. */

#ifndef NUMPARSERAGSBTG_H_
#define NUMPARSERAGSBTG_H_

#include <cstdio>

#define TAB '\t'
#define SKIP_TS 11
#define DIGIT_MERGE(x, y) ((x*10)+y)
#define READ_ERROR -1L
#define INITIAL_NEXT 0
#define UINDEX_STATE 1
#define MINDEX_STATE 2
#define RATING_STATE 3
#define EOF_STATE 4

/** A parser for u.data with the advantage of working while a file stream
    is being read, rather then after the stream has been read. */
class DataParser {
  private:
    FILE* read;
    size_t state;

    /** Converts a single character to it's decimal equivalent.
        @param c the character to be converted
        @return the decimal equivalent of c or 10 if c is not a valid number */
    size_t ctd(char c) const {
      switch(c) {
        case '0':
          return 0;
          break;
        case '1':
          return 1;
          break;
        case '2':
          return 2;
          break;
        case '3':
          return 3;
          break;
        case '4':
          return 4;
          break;
        case '5':
          return 5;
          break;
        case '6':
          return 6;
          break;
        case '7':
          return 7;
          break;
        case '8':
          return 8;
          break;
        case '9':
          return 9;
          break;
        default:
          return 10;
          break;
      }
    }

  public:
    /** Creates a new DataParser which can read though u.data and parse out
        all the numbers skipping over the unix timestamp.
        @param fileName the name of a given file */
    DataParser(char* fileName)
      : read(fopen(fileName, "r")),
        state(UINDEX_STATE) {
      if(read == NULL) {
        perror("Error opening file");
      }
      else {
        char temp = fgetc(read);
        if(temp == EOF) {
          fclose(read);
          state = EOF_STATE;
        }
        else {
          ungetc(temp, read);
          state = UINDEX_STATE;
        }
      }
    }

    /** Acquires the next number in u.data skipping over the unix timestamp.
        @return the next number in u.data */
    size_t next() {
      size_t nxt;
      char temp;
      switch(state) {
        case UINDEX_STATE:
          nxt = ctd(char(fgetc(read)));
          temp = fgetc(read);
          while(temp != TAB) {
            nxt = DIGIT_MERGE(nxt, ctd(temp));
            temp = fgetc(read);
          }
          state = MINDEX_STATE;
          break;
        case MINDEX_STATE:
          nxt = ctd(char(fgetc(read)));
          temp = fgetc(read);
          while(temp != TAB) {
            nxt = DIGIT_MERGE(nxt, ctd(temp));
            temp = fgetc(read);
          }
          state = RATING_STATE;
          break;
        case RATING_STATE:
          nxt = ctd(char(fgetc(read)));
          fseek(read, SKIP_TS, SEEK_CUR);
          temp = fgetc(read);
          if(temp == EOF) {
            fclose(read);
            state = EOF_STATE;
          }
          else {
            ungetc(temp, read);
            state = UINDEX_STATE;
          }
          break;
        default:
          break;
      }
      return nxt;
    }

    /** Determines whether the stream is open (holds more numbers).
        @return true if and only if the stream is open (holds more numbers) */
    bool hasNext() const {
      if(ftell(read) == READ_ERROR) {
        return false;
      }
      else { 
        return true; 
      }
    }
};

#endif
