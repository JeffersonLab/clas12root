#include "rcdb_reader.h"

namespace clas12 {

  /* Empty constructor needed to link _connection to database.
   */
  rcdb_reader::rcdb_reader():_connection{"mysql://rcdb@clasdb.jlab.org/rcdb", true}{}
  /* explicit close in destructor.
   */
  rcdb_reader::~rcdb_reader(){Close();};
  
  /* Function to return a bool type value, given the value's name and a run
   * number.
   * Will return false if the condition doesn't exist for this run, or the run
   * number isn't recorded.
   * If the value is not of type bool the program will stop.
   */
  bool rcdb_reader::getBoolValue(int runNb, std::string value){
    auto cnd = _connection.GetCondition(runNb, value);
    bool val = false;
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToBool();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not a bool."<<std::endl;
	}
    }
    return val;
  }

  /* Function to return an integer type value, given the value's name and a run
   * number.
   * Will return -1 if the condition doesn't exist for this run, or the run
   * number isn't recorded.
   * If the value is not of type integer the program will stop.
   */
  int rcdb_reader::getIntValue(int runNb, std::string value){
    auto cnd = _connection.GetCondition(runNb, value);
    int val = -1;
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToInt();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not an int."<<endl;
	}
    }
    return val;
  }

  /* Function to return a double type value, given the value's name and a run
   * number.
   * Will return -1 if the condition doesn't exist for this run, or the run
   * number isn't recorded.
   * If the value is not of type double the program will stop.
   */
  double rcdb_reader::getDoubleValue(int runNb, std::string value){
    auto cnd = _connection.GetCondition(runNb, value);
    double val = -1;
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToDouble();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not a double."<<endl;
	}
    }
    return val;
  }

  /* Function to return a string type value, given the value's name and a run
   * number.
   * Will return Error if the condition doesn't exist for this run, or the run
   * number isn't recorded.
   * If the value is not of type string the program will stop.
   */
  std::string rcdb_reader::getStringValue(int runNb, std::string value){
    auto cnd = _connection.GetCondition(runNb, value);
    std::string val = "Error";
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToString();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not a double."<<endl;
	}
    }
    return val;
  }
  
  /* Function to return a time_point type value, given the value's name and a 
   * run number.
   * Will return the current time if the condition doesn't exist for this run,
   * or if the run number isn't recorded.
   * If the value is not of type time_point the program will stop.
   */
  std::chrono::time_point<std::chrono::system_clock> rcdb_reader::getTimeValue(int runNb, std::string value){
    auto cnd = _connection.GetCondition(runNb, value);
    std::chrono::time_point<std::chrono::system_clock> val = std::chrono::system_clock::now();
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToTime();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not a double."<<endl;
	}
    }
    return val;
    }
}
