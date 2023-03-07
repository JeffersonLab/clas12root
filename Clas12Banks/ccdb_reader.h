#ifndef CCDB_READER_H
#define CCDB_READER_H

#include "region_particle.h"
#ifdef CLAS_CCDB
#include "CCDB/CalibrationGenerator.h"
#include "CCDB/Calibration.h"
#endif

#include <string>
#include <vector>
#include <memory>

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
    
    void getSamplingFactionPars();
    
    double getFDPhotonEnergy(region_particle*  p){
      if(p->getRegion()!=FD) return 0.;
      auto edep =  p->getDetEnergy();
      return edep/getMeanSF(edep);
    }
    double getMeanSF(float  Edep){
      return _sfPa*(_sfPb+(_sfPc/Edep)+(_sfPd/(Edep*Edep)));
    }

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
    int requestTableEntryFor(const std::string& item,const std::string& tableName);
    double requestTableValueFor(int row,const std::string& item,const std::string& tableName);
    
  private:

    std::vector< TableRecord_t > _localTable;

    // Photon Sampling Fraction Parameters
    double _sfPa={0};
    double _sfPb={0};
    double _sfPc={0};
    double _sfPd={0};
 
#ifdef CLAS_CCDB
   
    std::unique_ptr<ccdb::Calibration> _calib ={nullptr};
      
#endif

  };

}
#endif /* CCDB_READER_H */
