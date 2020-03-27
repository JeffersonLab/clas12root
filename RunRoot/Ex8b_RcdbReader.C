#include <cstdlib>
#include <iostream>
#include "rcdb_reader.h"
#include "clas12reader.h"
#include <string>
#include <chrono>
#include <ctime>

using namespace clas12;
using namespace std;

void Ex8b_RcdbReader(){


  clas12reader c12("/a/hipo/file.hipo",{0});//needs tag-0 event
  c12.next();//need to read first event
  auto runNo=c12.runconfig()->getRun(); //get run number for this file
  c12.getReader().gotoRecord(0); //reset back to start
  
  /* Creates the interface to RCDB header library.
   * Immediately opens a connection to rcdb@clasdb.jlab.org/rcdb.
   */
  rcdb_reader rcdb;

  /* Examples of how to access different conditions for runNo.
   * If the wrong type is asked for a given condition the code will 
   * stop running.
   * If the run or condition doesn't exist an error message is given 
   * and a wrong value is returned.
   * The database and conditions can be viewed at 
   * https://clasweb.jlab.org/rcdb/ .
   */
  int val = rcdb.getIntValue(runNo, "event_count");
  double val2 = rcdb.getDoubleValue(runNo, "beam_energy");
  double val3 = rcdb.getDoubleValue(runNo, "beam_current");
  std::string val4 = rcdb.getStringValue(runNo, "target");
  bool val5 = rcdb.getBoolValue(runNo, "is_valid_run_end");

  cout<<"Run number : "<<runNo<<endl;
  cout<<"Event count: "<<val<<" Beam energy: "<<val2<<endl;
  cout<<"Beam current: "<<val3<<" Target: "<<val4<<endl;
  cout<<"Run is valid: "<<val5<<endl;


}
