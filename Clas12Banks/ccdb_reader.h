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

    vector<vector<double>> getTableDoubles(std::string);

  private:

    ccdb::CalibrationGenerator _gen;
    //ccdb::Calibration* _calib; 
    std::unique_ptr<ccdb::Calibration> _calib ={nullptr};

  };

}
#endif /* CCDB_READER_H */
