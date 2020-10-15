#include "qadb_reader.h"

namespace clas12 {

#ifdef CLAS_QADB
  
  qadb_reader::qadb_reader(string jsonFilePath, int runNo):_qa{jsonFilePath.c_str()}{_runNo=runNo;};


  ///////////////////////////////////////////////////////
  ///Checks if an event passes all the QA requirements

  bool qadb_reader::passQAReqs(int evNo){
    //First event always has index 0 which doesn't exist in qaDB
    if(evNo!=0){
      //isOkForAsymmetry already queries _QA, want to avoid doing it twice
      bool passAsymReq=true;
      bool queried=false;
      if(_reqOKAsymmetry){
	passAsymReq = isOkForAsymmetry(_runNo,evNo);
	queried=true;
      } else {
	queried = query(_runNo,evNo);
      }
    
      //An event may be ok for asymmetry but have other defects
      if(passAsymReq && queried){
	//If an event is Golden it won't have other defects
	if(_reqGolden){
	  return isGolden();
	} else {
	  //loop over all requirements asked by user
	  for(string req: _reqsQA){
	    if (hasDefect(req.c_str())){
	      return false;
	    }
	  }
	}
      } else {
	return false;
      }
    }
    //Event passes all requirements
    return true;
  }

#else
  qadb_reader::qadb_reader(string jsonFilePath, int runNo){
    _runNo=runNo;
  }


  ///////////////////////////////////////////////////////
  ///Checks if an event passes all the QA requirements

  bool qadb_reader::passQAReqs(int evNo){
    return true;
  }
  
#endif
  void qadb_reader::copySettings(const qadb_reader& other){
    _reqsQA=other._reqsQA;
    _reqOKAsymmetry=other._reqOKAsymmetry;
    _reqGolden=other._reqGolden;
  }

}
