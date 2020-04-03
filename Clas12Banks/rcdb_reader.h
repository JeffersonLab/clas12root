/*
 * File:   rcdb_reader.h
 * Author: rtyson
 */

#ifndef RCDB_READER_H
#define RCDB_READER_H

#include "RCDB/Connection.h" 
#include <iostream>

#include <chrono>
#include <string>

namespace clas12 {
  using std::cout;
  using std::endl;

  class rcdb_reader {

  public:

    rcdb_reader();
    virtual ~rcdb_reader()=default;

    bool getBoolValue(int runNb, std::string value);
    int getIntValue(int runNb, std::string value);
    double getDoubleValue(int runNb, std::string value);
    std::string getStringValue(int runNb, std::string value);
    std::chrono::time_point<std::chrono::system_clock> getTimeValue(int runNb, std::string value);
    void Close(){_connection->Close();};

  private:

    rcdb::Connection *_connection = new rcdb::Connection("mysql://rcdb@clasdb.jlab.org/rcdb", true);    

  };

}

#endif /* RCDB_READER_H */
