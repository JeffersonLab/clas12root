#pragma once


#include "HipoHist.h"
#include "clas12reader.h"
#include <map>

namespace clas12root {

  
  class ParticleHist : public HipoHist {


  public :
    ParticleHist(TString filename);
    virtual ~ParticleHist();


    void Loop() final;
    
    
    using c12_uptr = std::unique_ptr<clas12::clas12reader>;
    
    c12_uptr clas12(){
      return std::move(c12_uptr
		       (new clas12::clas12reader(HipoFileName().Data())));
    }
    
  private :
 
 
  };
}
