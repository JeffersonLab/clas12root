#include "ccdb_reader.h"

namespace clas12 {
  ccdb_reader::ccdb_reader(){}
  ccdb_reader::~ccdb_reader(){
    if (check()){close();}
  }

  void ccdb_reader::connect(int runNb){
    auto _calib = _gen.MakeCalibration(
      "mysql://clas12reader@clasdb.jlab.org/clas12", runNb, "default", 0);
  }

  void ccdb_reader::close(){_calib->Disconnect();}

  bool ccdb_reader::check(){_calib->IsConnected();}

  int ccdb_reader::getIntValue(std::string value){
    int result;
    _calib->GetCalib(result, value);
    return result;
  }

  vector<vector<double>> ccdb_reader::getVecVecValue(std::string value){
    vector<vector<double>> result;
    _calib->GetCalib(result, value);
    return result;
  }
}
