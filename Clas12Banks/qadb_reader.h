#ifndef QADB_READER_H
#define QADB_READER_H

#ifdef CLAS_QADB
#include "QADB.h"
#endif

#include <iostream>
#include <string>
#include <vector>

namespace clas12 {
  using std::string;
  
  class qadb_reader {

  public:
    qadb_reader(string jsonFilePath, int runNo=0);
    //virtual ~qadb_reader()=default;  



  public:
    bool isValid();
    
    void addQARequirement(string req){_reqsQA.push_back(req);};
    void setQARequirements( std::vector<string> reqs){_reqsQA=reqs;};
    
    void requireOkForAsymmetry(bool ok){_reqOKAsymmetry=ok;};
    void requireGolden(bool ok){_reqGolden=ok;};
    bool passQAReqs(int evNo);

    void setRun(int runNb){_runNo=runNb;}

    void copySettings(const qadb_reader& other);

    
  private:
    
    qadb_reader()=default;

    
    std::vector<string> _reqsQA;//!
    bool _reqOKAsymmetry{false};//!
    bool _reqGolden{false};//!
    int _runNo{0};//!

    //ifdefs must go last , or can lead to issues with PROOF
    //i.e. refences are slighty shifted
  
#ifdef CLAS_QADB

    bool query(int runNb, int evNb){return _qa.Query(runNb,evNb);};
    int getFileNb(){return _qa.GetFilenum();};
    bool isGolden(){return _qa.Golden();};
    int getMinEventNb(){return _qa.GetEvnumMin();};
    int getMaxEventNb(){return _qa.GetEvnumMax();};
    int getDefect(){return _qa.GetDefect();};
    int getDefectForSector(int sector){return _qa.GetDefectForSector(sector);};
    bool hasDefect(const char * defectName, int sector){return _qa.HasDefect(defectName,sector);};
    bool hasDefect(const char * defectName){return _qa.HasDefect(defectName);};
    string getComment(){return _qa.GetComment();};

    bool isOkForAsymmetry(int runNb, int evNb){return _qa.OkForAsymmetry(runNb,evNb);};

  private: 
    QADB _qa;//!  
 
#endif


  
  };

}
#endif /* QADB_READER_H */
