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
  using std::string;

  struct rcdb_vals;
  
  class rcdb_reader {

  public:

    rcdb_reader(int runNb);
    virtual ~rcdb_reader();

    bool getBoolValue(int runNb, std::string value);
    int getIntValue(int runNb, std::string value);
    double getDoubleValue(int runNb, std::string value);
    std::string getStringValue(int runNb, std::string value);
    std::chrono::time_point<std::chrono::system_clock> getTimeValue(int runNb, std::string value);
    void Close(){_connection.Close();};

    rcdb_vals readAll();
    
  private:

    //  rcdb::Connection *_connection = new rcdb::Connection("mysql://rcdb@clasdb.jlab.org/rcdb", true);    
    rcdb::Connection _connection;    
    int _runNb{0};
  };
  
  struct rcdb_vals{
    float	beam_current{0};	//Beam current
    string	beam_current_request{""};	//Beam Current Request
    float	beam_energy{0};	//Beam Energy
    string	daq_comment{""};	//DAQ comment
    string	daq_config{""};	//Run config on main page
    string	daq_setup{""};	//Run type on main page
    string	daq_trigger{""};	//Trigger file
    float	events_rate{0};	//Daq events rate
    int	event_count{0};	//Number of events in run
    int	evio_files_count{0};	//The number of data files
    int	half_wave_plate{0};	//Half Wave Plate position
    bool	is_valid_run_end{1};	//True if a run has valid run-end record. False means the run was aborted/crashed at some point
    //json	json_cnd	//Basic run info
    int	megabyte_count{0};	//The number of megabytes in run
    string	operators{""};	
    string	run_config{""};	//DAQ/Trigger config file
    //time	run_end_time{0};	//Run end time
    // time	run_start_time{0};	//Run start time
    string	run_type{""};	//Run Type
    float	solenoid_current{0};	//Solenoid current
    float	solenoid_scale{0};  //Solenoid scale factor
    int	status{0};	//Run Status
    string	target{""};	//Target
    float	target_position{0};	//Target position
    int	temperature{0};	//Temperature of the Sun
    float	test{0};	//Beam test
    float	torus_current{0};	//Torus current
    float	torus_scale{0};	//Torus scale factor
    string	user_comment{""};//
  };
}
#endif /* RCDB_READER_H */
