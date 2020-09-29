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

  ccdb_reader c;

  //connect to ccdb with a specific run number
  int runNb = 5051;
  c.connect(runNb);

  //this causes a seg fault
  auto val1 = c.getTableDoubles("/calibration/eb/electron_sf");

}
