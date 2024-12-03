#pragma once

#include "IguanaAlgo.h"
#include <iguana/algorithms/clas12/ZVertexFilter/Algorithm.h>
//#include <iguana/algorithms/clas12/FiducialFilter/Algorithm.h>
//#include <iguana/algorithms/clas12/PhotonGBTFilter/Algorithm.h>
#include <iguana/services/ConcurrentParam.h>

namespace clas12root{


  class Filters : public IguanaAlgo {

  public:

    iguana::clas12::ZVertexFilter* ZVertexFilter(){return _zVer.get();}
    //    iguana::clas12::FiducialFilter* FiducialFilter(){return _Fid.get();}
    //iguana::clas12::PhotonGBTFilter* PhotonGBTFilter(){return _PhotGBT.get();}

    void Use(const string& name){
	
      //Initialise the z-vertex filter algorithm
      if(name=="clas12::ZVertexFilter"){
	_zVer.reset(new iguana::clas12::ZVertexFilter);
	AddAlgo(_zVer.get());
      }
      //  if(name=="clas12::FiducialFilter"){
      // 	_Fid.reset(new iguana::clas12::FiducialFilter);
      // 	AddAlgo(_Fid.get());
      // }
       /*
      if(name=="clas12::PhotonGBTFilter"){
	_PhotGBT.reset(new iguana::clas12::PhotonGBTFilter);
	AddAlgo(_PhotGBT.get());
      }
       */
    }
      
    //interface to vector transforms
    void doFilters(std::function< bool ( const clas12::region_particle *p ) > fun) const{
      // bool filt = true;
      // for(auto* p:parts) filt*=fun(p);
      // return filt;

      //get the clas12root region particles vector for the event
      auto& particles = C12()->getDetParticles();
      //save particles to delete
      //(cannot to it actively when looping over same vector)
      std::vector<region_part_ptr> to_delete;
      for(auto* pa:particles){
	if(fun(pa)==false){//failed filter must remove
	  to_delete.push_back(pa);
	}
      }
      //remove failed particles from clas12root vector
      if(to_delete.empty()==false){
	for(auto* pa:to_delete){
	  //remove this particle pointer from detParticles
	  particles.erase(find(particles.begin(), particles.end(), pa));
	}
      }
      //all done
    }

    /////////////////
    //specific filters
    //zvertex filter
    bool doZVertexFilter(const clas12::region_particle *p) const{
      return _zVer->Filter(p->par()->getVz(),p->par()->getPid(),p->par()->getStatus(),_concurrent_key);// 0 => concurrent key
    }
    void doZVertexFilter() const{
      doFilters([this](const clas12::region_particle *p){return doZVertexFilter(p);});
    }
    /*
    ////////////////
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
    void doFiducialFilter() const{
      doFilters([this](const clas12::region_particle *p){return doFiducialFilter(p);});
    }
    */
    //////////////////
    //PhotonGBT filter
    /*
    bool doPhotonGBTFilter(const clas12::region_particle *p, const std::map<int, iguana::clas12::PhotonGBTFilter::calo_row_data>& calo) const{

      if(p->getPid()!=22) return true;

      //Need FiducialFitler to be public!
      
      //return _PhotGBT->Filter(*(p->par()),*(p->cal()),calo,p->getIndex(),C12()->getRunNumber() );

      return true;
      
    }
    void doPhotonGBTFilter() const{
      if(getNParticles()==0) return;
      auto calo = _PhotGBT->GetCaloMap(*(C12()->getDetParticles(0)->cal(-1)));
      doFilters([this,&calo](const clas12::region_particle *p){return doPhotonGBTFilter(p,calo);});
    }
    */
    ///////////////////////////////////////////////
    void doAllFilters(){

      if(_zVer.get())doZVertexFilter();
      // if(_Fid.get())doFiducialFilter();
      // if(_PhotGBT.get())doPhotonGBTFilter();
    }


    //Reload
    void Reload() const{
      auto run = C12()->runconfig()->getRun();
      if(_runNb==run) return;
      _runNb=run;
      
      if(_zVer.get())_concurrent_key=_zVer->PrepareEvent(run);
    
    }
      
  private:
	
    std::unique_ptr<iguana::clas12::ZVertexFilter> _zVer;
    //std::unique_ptr<iguana::clas12::FiducialFilter> _Fid;
    //std::unique_ptr<iguana::clas12::PhotonGBTFilter> _PhotGBT;

    mutable Long64_t _runNb =-1;
    mutable int _concurrent_key=0;
 
   };
      
}

