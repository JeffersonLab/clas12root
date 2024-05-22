#pragma once

#include "IguanaAlgo.h"
#include <iguana/algorithms/clas12/ZVertexFilter/Algorithm.h>

namespace clas12root{


  class Filters : public IguanaAlgo {

  public:

    iguana::clas12::ZVertexFilter* ZVertexFilter(){return _zVer.get();}

    void Use(const string& name){
	
      //Initialise the z-vertex filter algorithm
      if(name=="clas12::ZVertexFilter"){
	_zVer.reset(new iguana::clas12::ZVertexFilter);
	AddAlgo(_zVer.get());
      }
    }
    //interface to vector transforms
    bool doFilters(const std::vector<const clas12::region_particle*>& parts,
		   std::function< bool ( const clas12::region_particle *p ) > fun) const{
      bool filt = true;
      for(auto* p:parts) filt*=fun(p);
      return filt;
    }


    //specific filters
    //zvertex filter
    bool doZVertexFilter(const clas12::region_particle *p) const{
      return _zVer->Filter(p->par()->getVz());
    }
    bool doZVertexFilter(const std::vector<const clas12::region_particle*> parts) const{
      return doFilters(parts,
		       [this](const clas12::region_particle *p){return doZVertexFilter(p);});
    }
    

  private:
	
    std::unique_ptr<iguana::clas12::ZVertexFilter> _zVer;
 	
  };
      
}

