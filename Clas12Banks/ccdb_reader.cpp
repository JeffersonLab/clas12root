#include "ccdb_reader.h"

namespace clas12 {
  ccdb_reader::ccdb_reader(){}
  ccdb_reader::~ccdb_reader(){}
    //if (check()){close();}
  //}

  void ccdb_reader::connect(int runNb){
    /* 
    Function to connect to mysql database online
    */
    _calib.reset( _gen.MakeCalibration(
      "mysql://clas12reader@clasdb.jlab.org/clas12", runNb, "default", 0)
    );
  }
  
  void ccdb_reader::connect_local(std::string path, int runNb){
    /* 
    Function to connect to local sqlite database
    */
    std::string prefix = "sqlite://";
    std::string full_path = prefix + path;
    _calib.reset( _gen.MakeCalibration(
      full_path, runNb, "default", 0)
    );
  }

  //close the connection
  void ccdb_reader::close(){_calib->Disconnect();}

  //check if connection is valid
  bool ccdb_reader::check(){return _calib->IsConnected();}

  vector<vector<double>> ccdb_reader::getTableDoubles(std::string value){
    /* 
    Get a table from the database, will reconnect to database automatically
    */
    vector<vector<double>> result;
    _calib->GetCalib(result, value);
    return result;
  }
}
