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

    //multithreading
    concurrent_key_t PrepareEvent(int const runnum) const{
      /*
      if(o_runnum->NeedsHashing()) {
	std::hash<int> hash_ftn;
	auto hash_key = hash_ftn(runnum);

	if(!o_runnum->HasKey(hash_key)){
	  _concurrent_key=hash_key;
 	  // Reload(runnum, hash_key);
	  Reload();
	}
      } else {
	if(o_runnum->IsEmpty() || o_runnum->Load(0) != runnum){
	  //Reload(runnum, 0);
	  _concurrent_key=0;
	  Reload();
	}
      }
      */
      Reload();
      return 0;
    }
    int GetKey()const {return _concurrent_key;}

    void Reload() const{
      //_transforms->Reload(GetKey());
      //_filter->Reload(GetKey());
      _filter.Reload();
      _creator.Reload();
    }
    private:
     
    clas12root::Transformers _transform;
    clas12root::Filters _filter;
    clas12root::Creators _creator;

    int _concurrent_key=0;
    // mutable std::unique_ptr<ConcurrentParam<int>> o_runnum = {ConcurrentParamFactory::Create<int>()};
  
    
   };






}
