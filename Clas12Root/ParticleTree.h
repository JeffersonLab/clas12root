#pragma once


#include "HipoTreeMaker.h"
#include "clas12reader.h"
#include <map>

namespace clas12root {

  
  class ParticleTree : public HipoTreeMaker {


  public :
    ParticleTree(TString filename, TString treefilename);
    virtual ~ParticleTree();


    void Loop() final;
    void PreCompileAction() final;
    
    void AddExactPid(short pid,short n){
      _pidSelectExact[pid]=n;
    }
    void AddAtLeastPid(short pid,short n){
      _pidSelect[pid]=n;
    }
    void AddZeroOfRestPid(){_zeroOfRestPid=true;}
    using c12_uptr = std::unique_ptr<clas12::clas12reader>;
    
    c12_uptr clas12(){
      return std::move(c12_uptr
		       (new clas12::clas12reader(HipoFileName().Data())));
    }

    void UseEventData(){_useEventData=kTRUE;}
  private :
   std::map<short,short> _pidSelect;
   std::map<short,short> _pidSelectExact;
   bool _zeroOfRestPid=false;
   bool _useEventData=kFALSE;
 
  };
}
