#include <cstdlib>
#include <iostream>
#include "ccdb_reader.h"
#include <string>
#include <chrono>
#include <ctime>

using namespace clas12;
using namespace std;

void Ex10_ccdbReader(){
  //An example of how to use the ccdb reader to get values from
  //the calibration database

  //connect to a local copy of ccdb with a specific run number
  //get the run number from a hipo file
  clas12reader c12;
  int runNb = c12.readQuickRunConfig("/work/RunRoot/skim3_005032.hipo");
  //int runNb = 5032; //set it manually
  ccdb_reader c("/work/RunRoot/ccdb_2020-09-13.sqlite", runNb);

  //or alternatively using the online mysql version
  //ccdb_reader c(runNb);

  //test the connection
  cout<<c.check()<<endl;

  //this causes a seg fault
  auto val1 = c.getTableDoubles("/calibration/eb/electron_sf");

  //checking againt ccdb online for this run number
  //https://clasweb.jlab.org/cgi-bin/ccdb/objects
  cout<< val1[0][5]<<endl; //-0.036 
  cout<< val1[1][5]<<endl; // 0.139

  //close the connection at end
  c.close();
}
