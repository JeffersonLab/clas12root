#pragma once

#include "clas12reader.h"

#include "Transformers.h"
#include "Filters.h"
#include "Creators.h"


namespace clas12root{


  class Iguana {

  public:
    
    void SetClas12( const std::unique_ptr<clas12::clas12reader>* c12 ){
      _transform.SetClas12(c12);
      _filter.SetClas12(c12);
      _creator.SetClas12(c12);
    }
    
     template <typename OPTION_TYPE>
     OPTION_TYPE  SetOptionAll(std::string const& key, const OPTION_TYPE val){

       _transform.SetOptionAll(key,val);
       _filter.SetOptionAll(key,val);
       _creator.SetOptionAll(key,val);
       
       return val;
    }
    
   void Start(){

      _transform.Start();
      _filter.Start();
      _creator.Start();
      
    }
    
    void Stop(){

      _transform.Stop();
      _filter.Stop();
      _creator.Stop();
    
    }
  
 
    Transformers& GetTransformers(){return _transform;}
    Filters& GetFilters(){return _filter;}
    Creators& GetCreators(){return _creator;}


    void GoodToBad(const ROOT::Math::PxPyPzMVector& good, TLorentzVector &bad){
      bad.SetXYZT(good.X(),good.Y(),good.Z(),good.T());
    }
    
    private:
     
    clas12root::Transformers _transform;
    clas12root::Filters _filter;
    clas12root::Creators _creator;

    
    
  
    
   };






}
