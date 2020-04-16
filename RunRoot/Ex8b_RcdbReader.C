#include <cstdlib>
#include <iostream>
#include "clas12reader.h"
#include <string>
#include <chrono>
#include <ctime>

using namespace clas12;
using namespace std;

void Ex8b_RcdbReader(){

  /* The clas12reader opens a connection to the RCDB when it initialises.
   * This connection is then closed but some run conditions are stored.
   * This is done to avoid overloading the database.
   */
  clas12reader c12("/a/hipo/file.hipo");
  c12.queryRcdb(); //only check rcdb values when asked to (and RCDB_HOME was set)
  
  auto rcdbData=c12.getRcdbVals();//struct with all relevent rcdb values
  
  //The following run conditions can be returned directly by c12
  cout<<"Event count: "<<rcdbData.event_count<<endl;
  cout<<"Beam energy: "<<rcdbData.beam_energy<<endl;
  cout<<"Beam current: "<<rcdbData.beam_current<<endl;


}
