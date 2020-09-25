#include <cstdlib>
#include <iostream>
#include "ccdb_reader.h"
#include <string>
#include <chrono>
#include <ctime>

using namespace clas12;
using namespace std;

void Ex9_ccdbReader(){
  //An example of how to use the ccdb reader to get values from
  //the calibration database

  ccdb_reader ccdb;

  //connect to ccdb with a specific run number
  int runNb = 5051;
  clas12::ccdb_reader  ccdb.connect(runNb);

  auto val1 = ccdb.getVecVecValue("/calibration/eb/electron_sf");

  cout<<val1[0][0]<<endl;

}
