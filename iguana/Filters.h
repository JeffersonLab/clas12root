#pragma once

#include "IguanaAlgo.h"
#include <iguana/algorithms/clas12/ZVertexFilter/Algorithm.h>
#include <iguana/algorithms/clas12/FiducialFilter/Algorithm.h>

namespace clas12root{


  class Filters : public IguanaAlgo {

  public:

    iguana::clas12::ZVertexFilter* ZVertexFilter(){return _zVer.get();}
    iguana::clas12::FiducialFilter* FiducialFilter(){return _Fid.get();}

    void Use(const string& name){
	
      //Initialise the z-vertex filter algorithm
      if(name=="clas12::ZVertexFilter"){
	_zVer.reset(new iguana::clas12::ZVertexFilter);
	AddAlgo(_zVer.get());
      }
       if(name=="clas12::FiducialFilter"){
	_Fid.reset(new iguana::clas12::FiducialFilter);
	AddAlgo(_Fid.get());
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
    //fiducial filter
    bool doFiducialFilter(const clas12::region_particle *p) const
    {
      iguana::clas12::FiducialFilter::traj_row_data traj;
      auto c12=C12();
      traj.x1 = p->traj(clas12::DC,6)->getX();
      traj.x1 = p->traj(clas12::DC,6)->getY();
      traj.z1 = p->traj(clas12::DC,6)->getZ();
      traj.x2 = p->traj(clas12::DC,18)->getX();
      traj.x2 = p->traj(clas12::DC,18)->getY();
      traj.z2 = p->traj(clas12::DC,18)->getZ();
      traj.x3 = p->traj(clas12::DC,36)->getX();
      traj.x3 = p->traj(clas12::DC,36)->getY();
      traj.z3 = p->traj(clas12::DC,36)->getZ();
      traj.sector = p->getSector();

      //Need FiducialFitler to be public!
      // return _Fid->Filter(traj,c12->runconfig()->getTorus(),p->par()->getPid());
      return true;
    }
    bool doFiducialFilter(const std::vector<const clas12::region_particle*> parts) const{
      return doFilters(parts,
		       [this](const clas12::region_particle *p){return doFiducialFilter(p);});
    }
 
    ///////////////////////////////////////////////
    bool doAllFilters(const std::vector<const clas12::region_particle*> parts){

      bool OK = true;
      
      if(_zVer.get())OK*=doZVertexFilter(parts);
      if(_Fid.get())OK*=doFiducialFilter(parts);

      return OK;
    }
  private:
	
    std::unique_ptr<iguana::clas12::ZVertexFilter> _zVer;
    std::unique_ptr<iguana::clas12::FiducialFilter> _Fid;
 	
  };
      
}

