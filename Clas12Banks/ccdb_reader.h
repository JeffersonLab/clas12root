#ifndef CCDB_READER_H
#define CCDB_READER_H

#ifdef CLAS_CCDB
#include "CCDB/CalibrationGenerator.h"
#include "CCDB/Calibration.h"
#endif

#include <string>
#include <vector>

namespace clas12 {

  using TableOfDoubles_t = std::vector<std::vector<double>>;
  using TableRecord_t =    std::pair<std::string,TableOfDoubles_t>;
  using CCDBTable = const clas12::TableOfDoubles_t*;
  class ccdb_reader {

  public:

    //    ccdb_reader(int runNb);
    ccdb_reader(const std::string& path, int runNb=0);
    
    bool check();
    void close();
    void open();
    
    void setRun(int nb){_runNb=nb; _srunNb=std::to_string(nb);}
    std::string nameWithRun(const std::string& tableName);
    
 
 private:
    
    ccdb_reader()=default;


    std::string _path;//!
    std::string _srunNb;//!

    int _runNb={0};//!

    //ifdefs must go last , or can lead to issues with PROOF
    //i.e. refences are slighty shifted
  

  public:
    const TableOfDoubles_t& requestTableDoubles(std::string tableName);
    void updateTables();
    
  private:

    std::vector< TableRecord_t > _localTable;

#ifdef CLAS_CCDB
   
    std::unique_ptr<ccdb::Calibration> _calib ={nullptr};


      
#endif

  };

}
#endif /* CCDB_READER_H */
