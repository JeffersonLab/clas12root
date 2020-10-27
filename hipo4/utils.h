/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   utils.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include<chrono>

namespace hipo {

  class utils {
  private:

  public:
    utils();
    ~utils();
    static void tokenize(const std::string& str,
			                   std::vector<std::string>& tokens,
			                   const std::string& delimiters = " ");

    static std::string substring(const std::string &str,
                                 const char *start_delim,
                                 const char *end_delim, int order);

    static int findposition(const std::string &str,
                            const char *delim, int order);

    static std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
      str.erase(0, str.find_first_not_of(chars));return str;
    }
    static std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
       str.erase(str.find_last_not_of(chars) + 1);return str;
    }
    static std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
      return ltrim(rtrim(str, chars), chars);
    }

    static void printLogo();

    static std::string getHeader();
    static std::string getFileHeader();
    static std::string getFileTrailer(const char *code);
    static std::string getSConstruct();

    static void writeInt(  char *buffer, int position, int     value);
    static void writeLong( char *buffer, int position, long    value);
    static void writeByte( char *buffer, int position, uint8_t value );
  };

  class benchmark {
     private:

       std::chrono::high_resolution_clock clock;
       std::chrono::time_point<std::chrono::high_resolution_clock> first, second;
       std::string  benchmarkName;

       long running_time;
       int  counter;
       int  printoutFrequency;

     public:

        benchmark(const char *name){
           benchmarkName = name;
           running_time = 0;counter = 0; printoutFrequency = -1;
         }
        benchmark(){ running_time = 0;counter = 0; printoutFrequency = -1;}
        benchmark(int freq){ running_time = 0;counter = 0; printoutFrequency = freq;}
       ~benchmark(){}

       void     setName(const char *name){ benchmarkName = name;}
       void     resume();
       void     pause();
       long     getTime();
       double   getTimeSec();
       int      getCounter();
       void     show();
  };
}

#endif /* UTILS_H */
