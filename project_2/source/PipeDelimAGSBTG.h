/** File "PipeDelimAGSBTG.h", by AGS for CSE250, Fall 2010, Project 2. */

#ifndef PIPEDELIMAGSBTG_H_
#define PIPEDELIMAGSBTG_H_

#include <string>
#include <cstdio>

#define PIPE '|'
#define NEWLINE '\n'
#define NULL_CHAR '\0'
#define READ_ERROR -1L

/** A pipe delimiter with the advantage of working while a file stream 
    is being read, rather then after the stream has been read. */
class PipeDelim {
  private:
    FILE* read;

  public:
    /** Creates a new PipeDelim which can read through a given file stream
        using pipes and newlines as delimiting characters.
        @param fileName the name of a given file */
    PipeDelim(char* fileName)
      : read(fopen(fileName, "r")) {
      if(read == NULL) {
        perror("Error opening file");
      }
      else {
        this->hasNext();
      }
    }

    /** Acquires a pointer to the next string delimited by a pipe or a newline.
        @return a pointer to the next string in the sequence or an empty string on EOF */
    std::string* next() const {
      std::string* nxt = new std::string();
      if(this->hasNext()) {
        char temp = NULL_CHAR;
        while((temp != PIPE) && (temp != NEWLINE) && (temp != EOF)) {
          temp = fgetc(read);
          if((temp != PIPE) && (temp != NEWLINE) && (temp != EOF)) {
            nxt->push_back(temp);
          }
        }
      }
      return nxt;
    }

    /** Determines whether the stream holds more strings.
        @return true if and only if the stream holds more strings */
    bool hasNext() const {
      if(ftell(read) == READ_ERROR) {
        return false;
      }
      else {
        char temp = fgetc(read);
        if(temp == EOF) {
          fclose(read);
          return false;
        }
        else {
          ungetc(temp, read);
          return true;
        }
      }
    }
};

#endif
