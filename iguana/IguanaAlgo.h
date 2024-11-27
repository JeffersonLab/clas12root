#pragma once

#include "clas12reader.h"
#include "region_particle.h"

#include <iguana/algorithms/Algorithm.h>
#include <Math/Vector4D.h>

#include <vector>
#include <functional>
#include <optional>

namespace clas12root{

  using FourVector = ROOT::Math::PxPyPzMVector;
  using concurrent_key_t = std::size_t;
  
  class IguanaAlgo{

  public:
    
    void SetClas12( const std::unique_ptr<clas12::clas12reader>* c12){_c12=c12;}

    void Start(){

      for(auto* algo: _algos){
	std::cout<<" start "<<algo->GetName()<<endl;
	algo->Start();
      }
      
    }
    void Stop(){

      for(auto* algo: _algos){
	algo->Stop();
      }
      
    }
        
 
  protected :
    
    friend class Iguana;
    
    void AddAlgo(iguana::Algorithm* algo){
      _algos.push_back(algo);
    }
    
    template <typename OPTION_TYPE>
     OPTION_TYPE  SetOptionAll(std::string const& key, const OPTION_TYPE val){

      for(auto* algo: _algos){
	algo->SetOption(key,val);
      }
      return val;
    }
    
   clas12::clas12reader* C12() const {return _c12->get();}

  private:
    
    std::vector<iguana::Algorithm*> _algos;
    const std::unique_ptr<clas12::clas12reader>* _c12=nullptr;
 
    
  };

}
