#include "qadb_reader.h"

namespace clas12 {

#ifdef CLAS_QADB
  
  qadb_reader::qadb_reader(const string& pass,int runNb):_qa{pass}{_runNb=runNb;};

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
    // std::cout<<"DEBUG qadb_reader::passQAReqs "<<_runNb<<" "<<evNb<<std::endl;
    if(_runNb==0) return true;//e.g. simulation
    
    //isOkForAsymmetry already queries _QA, want to avoid doing it twice
    bool passAsymReq=true;
    bool queried=false;
    // std::cout<<"DEBUG qadb_reader::passQAReqs "<<_runNb<<" "<<evNb<<"isgolden "<<_reqGolden<<" "<<isGolden(_runNb,evNb)<<" "<<isOkForAsymmetry(_runNb,evNb)<<std::endl;
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
	//	std::cout<<"DEBUG qadb_reader::passQAReqs "<<_runNb<<" "<<evNb<<"isgolden "<<isGolden(_runNb,evNb)<<std::endl;
  	  
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
    
    //Event passes all requirements
    return true;
  }

 /*This function will take in a list of runs and return the accumulated charge
    past specified requirements.
    Note: This is done on a file per file basis*/
  double qadb_reader::getChargeForRunlist(std::set<int> Runs){
    //loop over runs in run list
    for (auto runNb:Runs){
      setRun(runNb);
      std::cout<<"qadb_reader::getChargeForRunlist Reading Run "<<_runNb<<std::endl;
      int evNb=-1;
      //files go in increments of 5, hope this doesn't change
      for(int fileNb=0; fileNb<=_qa.GetMaxFilenum(_runNb); fileNb+=5) {
	//query the QADB can ready this run/file
	if(_qa.QueryByFilenum(_runNb,fileNb)) {
	  
	  // we need an event number within this file, to pass to QA criteria
	  // checking methods, such as Golden; no additional Query will be called
	  evNb = _qa.GetEvnumMin();
	  //this will check the requirements and accumulate charge
	  passQAReqs(evNb);

	}//query QADB for file/run combination, checks it exists
      }//loop over files in runs
    }//loop over runs
    _prevSumCharge = _qa.GetAccumulatedCharge();
    return _qa.GetAccumulatedCharge();
  }

#else
  qadb_reader::qadb_reader(const string& pass,int runNb){
    _runNb=runNb;
  }


  ///////////////////////////////////////////////////////
  ///Checks if an event passes all the QA requirements

  bool qadb_reader::passQAReqs(int evNb){
    return true;
  }
   /*This function will take in a list of runs and return the accumulated charge
    past specified requirements.
    Note: This is done on a file per file basis*/
  double qadb_reader::getChargeForRunlist(std::set<int> Runs){
    return 0; //qadb not defined
  }

#endif
  void qadb_reader::copySettings(const qadb_reader& other){
    _reqsQA=other._reqsQA;
    _reqOKAsymmetry=other._reqOKAsymmetry;
    _reqGolden=other._reqGolden;
  }
 
}
