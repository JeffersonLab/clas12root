#include "rcdb_reader.h"

namespace clas12 {

  /* Empty constructor needed to link _connection to database.
   */
  rcdb_reader::rcdb_reader(int runNb):_connection{"mysql://rcdb@clasdb.jlab.org/rcdb", true},_runNb{runNb}{}
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

  /* Function to read all conditions given in https://clasweb.jlab.org/rcdb/conditions/
     returns a copy of all vals
   */
  rcdb_vals rcdb_reader::readAll(){
    rcdb_vals fetcher;
    fetcher.beam_current=getDoubleValue(_runNb, "beam_current");
    fetcher.beam_energy=getDoubleValue(_runNb, "beam_energy");
    fetcher.events_rate=getDoubleValue(_runNb, "events_rate");
    fetcher.solenoid_current=getDoubleValue(_runNb, "solenoid_current");
    fetcher.solenoid_scale=getDoubleValue(_runNb, "solenoid_scale");
    fetcher.target_position=getDoubleValue(_runNb, "target_position");
    fetcher.test=getDoubleValue(_runNb, "test");
    fetcher.torus_current=getDoubleValue(_runNb, "torus_current");
    fetcher.torus_scale=getDoubleValue(_runNb, "torus_scale");
    
    fetcher.event_count=getIntValue(_runNb, "event_count");
    fetcher.evio_files_count=getIntValue(_runNb, "evio_files_count");
    fetcher.half_wave_plate=getIntValue(_runNb, "half_wave_plate");
    fetcher.megabyte_count=getIntValue(_runNb, "megabyte_count");
    fetcher.status=getIntValue(_runNb, "status");
    fetcher.temperature=getIntValue(_runNb, "temperature");

    
    fetcher.beam_current_request=getStringValue(_runNb, "beam_current_request");
    fetcher.daq_comment=getStringValue(_runNb, "daq_comment");
    fetcher.daq_config=getStringValue(_runNb, "daq_config");
    fetcher.daq_setup=getStringValue(_runNb, "daq_setup");
    fetcher.daq_trigger=getStringValue(_runNb, "daq_trigger");
    fetcher.operators=getStringValue(_runNb, "operators");
    fetcher.run_type=getStringValue(_runNb, "run_type");
    fetcher.target=getStringValue(_runNb, "target");
    fetcher.user_comment=getStringValue(_runNb, "user_comment");
     
    return fetcher;
  }

  
}
