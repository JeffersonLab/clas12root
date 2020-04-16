#include <cstdlib>
#include <iostream>
#include "rcdb_reader.h"
#include <string>
#include <chrono>
#include <ctime>

using namespace clas12;
using namespace std;

void Ex8_RcdbReader(){
  //This example shows how to use rcdb_reader class
  //It is not expected that this will be required for
  //general user who should just use clas12reader::queryRcdb()

  /* Creates the interface to RCDB header library.
   * Immediately opens a connection to rcdb@clasdb.jlab.org/rcdb.
   */
  rcdb_reader rcdb;

  /* Examples of how to access different conditions for run 12277.
   * If the wrong type is asked for a given condition the code will 
   * stop running.
   * If the run or condition doesn't exist an error message is given 
   * and a wrong value is returned.
   * The database and conditions can be viewed at 
   * https://clasweb.jlab.org/rcdb/ .
   */
  int val = rcdb.getIntValue(12277, "event_count");
  double val2 = rcdb.getDoubleValue(12277, "beam_energy");
  double val3 = rcdb.getDoubleValue(12277, "beam_current");
  std::string val4 = rcdb.getStringValue(12277, "target");
  bool val5 = rcdb.getBoolValue(12277, "is_valid_run_end");

  cout<<"Event count: "<<val<<" Beam energy: "<<val2<<endl;
  cout<<"Beam current: "<<val3<<" Target: "<<val4<<endl;
  cout<<"Run is valid: "<<val5<<endl;


}
