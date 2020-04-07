/*
 * File:   rcdb_reader.h
 * Author: rtyson
 */

#ifndef RCDB_READER_H
#define RCDB_READER_H

#include "rcdb_vals.h"

#include "RCDB/Connection.h" 
#include <iostream>

#include <chrono>
#include <string>

namespace clas12 {
  using std::cout;
  using std::endl;
  using std::string;

  
  class rcdb_reader {

  public:

    rcdb_reader();
    virtual ~rcdb_reader();

    bool getBoolValue(int runNb, std::string value);
    int getIntValue(int runNb, std::string value);
    double getDoubleValue(int runNb, std::string value);
    std::string getStringValue(int runNb, std::string value);
    std::chrono::time_point<std::chrono::system_clock> getTimeValue(int runNb, std::string value);
    void close(){_connection.Close();};

    rcdb_vals readAll(int runNb,const string& filename);
    
  private:

    rcdb::Connection _connection;    
  };
  

}
#endif /* RCDB_READER_H */
