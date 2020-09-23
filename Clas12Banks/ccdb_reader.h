#ifndef CCDB_READER_H
#define CCDB_READER_H

#include "ccdb_vals.h"
#include "CCDB/CalibrationGenerator.h"
#include "CCDB/Calibration.h"

#include <iostream>
#include <chrono>
#include <string>

namespace clas12 {
  using std::cout;
  using std::endl;
  using std::string;

  
  class ccdb_reader {

  public:

    ccdb_reader();
    virtual ~ccdb_reader();

    void connect(int runNb);
    bool check();
    void close();

    int getIntValue(std::string value);
    vector<vector<double>> getVecVecValue(std::string);

  private:

    ccdb::CalibrationGenerator _gen;
    ccdb::Calibration* _calib; 

  };

}
#endif /* CCDB_READER_H */
