#include "ccdb_reader.h"
#include <iostream>
#include <algorithm>

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
  bool ccdb_reader::check(){return true;}

#endif

  double ccdb_reader::requestTableValueFor(int row,const std::string& item,const std::string& tableName){
    
    const TableOfDoubles_t* table=nullptr;

    //Find the requested table
    auto it = std::find_if(_localTable.begin(),_localTable.end(),
		  [&tableName](const  TableRecord_t& element)
		  { return element.first == tableName;} );
    if(it==_localTable.end()){
      //need to create table
      table = &requestTableDoubles(tableName);
    }

    else table=&(it->second);
    
    //Find the column index for this item in this table
    auto ipos=requestTableEntryFor(item,tableName);
    if(ipos<0){
      std::cerr<<" ccdb_reader::requestTableValueFor no "<<item<<" in row "<<row<< " of "<<tableName<<std::endl;
      return 0.;
    }
    return table->at(row)[ipos];
    
  }
  int ccdb_reader::requestTableEntryFor(const std::string& item,const std::string& tableName){
    
    int entry=-1;
#ifdef CLAS_CCDB
    const auto& columns = _calib->GetAssignment(tableName)->GetTypeTable()->GetColumnNames();
    entry= std::distance(
            columns.begin(),
            std::find(columns.begin(), columns.end(), item) );
#endif
    return entry;
  }
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
    //Hack as the collaboration did not want to store
    //the photon energy for FD in the case the neutral was
    //PIDed as a photon
    getSamplingFactionPars();
 
#endif
    
    
  }
  
  //add the run number to the data request
  std::string ccdb_reader::nameWithRun(const std::string& tableName){
    auto temp = tableName+":";
    temp +=_srunNb;
    return temp;
  }

  void ccdb_reader::getSamplingFactionPars(){
    auto& ccdbPhSF = requestTableDoubles("/calibration/eb/photon_sf");
    _sfPa=ccdbPhSF[0][3]; //0.250 
    _sfPb=ccdbPhSF[0][4]; //1.029
    _sfPc=ccdbPhSF[0][5]; //-0.015
    _sfPd=ccdbPhSF[0][6]; // 0.00012
  }
}
