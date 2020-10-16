#include "ccdb_reader.h"
#include <iostream>
namespace clas12 {

   
  ccdb_reader::ccdb_reader(const std::string& path, int runNb){
    /* 
    Constructor to connect to local sqlite database
    */
    if(path.empty()) return;
    _path=path;

    setRun(runNb);

#ifdef CLAS_CCDB

    ccdb::CalibrationGenerator gen;
    _calib.reset( gen.MakeCalibration(_path, 0, "default", 0 ) );
#endif

  }

#ifdef CLAS_CCDB

  //open the connection
  void ccdb_reader::open(){_calib->Connect(_path);}

  //close the connection
  void ccdb_reader::close(){_calib->Disconnect();}

  //check if connection is valid
  bool ccdb_reader::check(){return _calib->IsConnected();}

 
  //
 
#else
    //open the connection
  void ccdb_reader::open(){}

  //close the connection
  void ccdb_reader::close(){}

  //check if connection is valid
  bool ccdb_reader::check(){}

#endif

 const TableOfDoubles_t& ccdb_reader::requestTableDoubles(std::string tableName){
    /* 
    Get a table from the database, will reconnect to database automatically
    Store in localTable and return reference so values will update when new run
    */
    
    uint entry=_localTable.size();
    _localTable.resize(entry+1);
 
    auto& tableEntry=_localTable[entry];
    tableEntry.first=tableName;

#ifdef CLAS_CCDB
  
    if(_runNb)_calib->GetCalib(tableEntry.second, nameWithRun(tableName) );
    
#endif

    return _localTable[entry].second; //reference to the TableOfDoubles
  }
  
  void   ccdb_reader::updateTables(){
  #ifdef CLAS_CCDB
    for(auto& tableEntry: _localTable){
      //tableEntry : name, values
      tableEntry.second.clear(); //empty the vector (required for GetCalib)
      _calib->GetCalib(tableEntry.second, nameWithRun(tableEntry.first) );
    }
#endif
    
    
  }
  
  //add the run number to the data request
  std::string ccdb_reader::nameWithRun(const std::string& tableName){
    auto temp = tableName+":";
    temp +=_srunNb;
    return temp;
  }

}
