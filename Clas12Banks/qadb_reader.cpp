#include "qadb_reader.h"

namespace clas12 {

#ifdef CLAS_QADB
  
  qadb_reader::qadb_reader(int runNb):_qa{}{_runNb=runNb;};

  ///////////////////////////////////////////////////////
  /// Add masks for all QA requirements
  void qadb_reader::addAllMasks(){
    for(string req:_reqsQA){
      addMask(req.c_str(),true);
    }
    _masksAdded=true;
  };

  ///////////////////////////////////////////////////////
  ///Checks if an event passes all the QA requirements
  bool qadb_reader::passQAReqs(int evNb){
    //First event always has index 0 which doesn't exist in qaDB
    if(evNb!=0){
      //isOkForAsymmetry already queries _QA, want to avoid doing it twice
      bool passAsymReq=true;
      bool queried=false;
      if(_reqOKAsymmetry){
	passAsymReq = isOkForAsymmetry(_runNb,evNb);
	queried=true;
      } else {
	queried = query(_runNb,evNb);
      }
      if(!_masksAdded){
	addAllMasks();
      }
      //An event may be ok for asymmetry but have other defects
      if(passAsymReq && queried){
	//If an event is Golden it won't have other defects
	if(_reqGolden){
	  //If the event passes the requirements, add charge
	  if(isGolden(_runNb,evNb)){
	    _qa.AccumulateCharge();
	  }
	  return isGolden(_runNb,evNb);
	} else {
	  //If the event passes the requirements, add charge
	  if(_qa.Pass(_runNb,evNb)){
	    _qa.AccumulateCharge();
	  }
	  return _qa.Pass(_runNb,evNb);
	}
      } else {
	return false;
      }
      //event passes all reqs, just don't want to acc for event 0
      _qa.AccumulateCharge();
    }
    //Event passes all requirements
    return true;
  }

#else
  qadb_reader::qadb_reader(int runNb){
    _runNb=runNb;
  }


  ///////////////////////////////////////////////////////
  ///Checks if an event passes all the QA requirements

  bool qadb_reader::passQAReqs(int evNb){
    return true;
  }
  
#endif
  void qadb_reader::copySettings(const qadb_reader& other){
    _reqsQA=other._reqsQA;
    _reqOKAsymmetry=other._reqOKAsymmetry;
    _reqGolden=other._reqGolden;
  }

}
