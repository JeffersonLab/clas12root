#pragma once

#include <string>

namespace clas12 {
  using std::string;

  struct rcdb_vals{
    string	file_name{""};	//data file name used to read the rcdb
    string	beam_current_request{""};	//Beam Current Request
    string	daq_comment{""};	//DAQ comment
    string	daq_config{""};	//Run config on main page
    string	daq_setup{""};	//Run type on main page
    string	daq_trigger{""};	//Trigger file
    string	operators{""};	
    string	run_config{""};	//DAQ/Trigger config file
    string	run_type{""};	//Run Type
    string	target{""};	//Target
    string	user_comment{""};//
    float	beam_current{0};	//Beam current
    float	beam_energy{0};	//Beam Energy
    float	events_rate{0};	//Daq events rate
    float	solenoid_current{0};	//Solenoid current
    float	solenoid_scale{0};  //Solenoid scale factor
    float	target_position{0};	//Target position
    float	target_polarization{0};	//Target position
    float	test{0};	//Beam test
    float	torus_current{0};	//Torus current
    float	torus_scale{0};	//Torus scale factor
    int         run_number{0}; //given run number (not actually from rcdb)
    int	        event_count{0};	//Number of events in run
    int 	evio_files_count{0};	//The number of data files
    int 	half_wave_plate{0};	//Half Wave Plate position
    int 	megabyte_count{0};	//The number of megabytes in run
    int 	status{0};	//Run Status
    int 	temperature{0};	//Temperature of the Sun
    bool	is_valid_run_end{1};	//True if a run has valid run-end record. False means the run was aborted/crashed at some point
  };
}
