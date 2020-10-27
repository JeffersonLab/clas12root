/*
 * File:   rcdb_reader.h
 * Author: rtyson
 */

#ifndef RCDB_READER_H
#define RCDB_READER_H

#include "rcdb_vals.h"
#ifdef CLAS_RCDB
#include "RCDB/Connection.h"
#endif

#include <iostream>

#include <chrono>
#include <string>

namespace clas12 {
  using std::cout;
  using std::endl;
  using std::string;

  
  class rcdb_reader {

  public:

    rcdb_reader(string connect_path);
    virtual ~rcdb_reader();
    rcdb_reader()=default;
  
   
    rcdb_vals readAll(int runNb,const string& filename);

    virtual void readRun(int runNb);
    const rcdb_vals& current()const {return _currentVals;}
    
    
    virtual void close();//{_connection.Close();};
    virtual void  open();//{_connection.Connect();};


  public:

    void setCurrentVals(const rcdb_vals& vals){
      _currentVals=vals;
    }
    
    
  private:
    
    rcdb_vals _currentVals;//!


    //ifdefs must go last , or can lead to issues with PROOF
    //i.e. refences are slighty shifted
#ifdef CLAS_RCDB
  public:
  
    bool getBoolValue(int runNb, std::string value);
    int getIntValue(int runNb, std::string value);
    double getDoubleValue(int runNb, std::string value);
    std::string getStringValue(int runNb, std::string value);
    std::chrono::time_point<std::chrono::system_clock> getTimeValue(int runNb, std::string value);

   private:

    std::unique_ptr<rcdb::Connection> _connection={nullptr};//!

    
#endif

     
  };
  


    class rcdb_root : public rcdb_reader {

  public:
    
    rcdb_root(string connect_path);
  
    void readRun(int runNb) final;
    void close()final{}
    void open()final{}

  private :
    rcdb_root()=default;
    
    string _rootFile;//!

    
  };


  }
#endif /* RCDB_READER_H */
