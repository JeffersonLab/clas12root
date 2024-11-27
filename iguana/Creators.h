#pragma once

#include "IguanaAlgo.h"
#include <iguana/algorithms/physics/InclusiveKinematics/Algorithm.h>

namespace clas12root{
  

  class Creators : public IguanaAlgo {

  public:
      
    iguana::physics::InclusiveKinematics* InclusiveKinematics(){return _incKin.get();}


    void Use(const string& name){
      //Initialise the inclusive kinemtics algorithm
      if(name=="physics::InclusiveKinematics"){
	_incKin.reset(new iguana::physics::InclusiveKinematics );
	AddAlgo(_incKin.get());
      }

      
    }
   
    //Inclusive Kinematics
    iguana::physics::InclusiveKinematicsVars doInclusiveKinematics(const clas12::region_particle *electron){
      return _incKin->ComputeFromLepton(electron->getPx(),electron->getPy(),electron->getPz(),_concurrent_key); //0 => concurrent key
    }
   
    iguana::physics::InclusiveKinematicsVars doInclusiveKinematics(const FourVector& electron){
      return _incKin->ComputeFromLepton(electron.X(),electron.Y(),electron.Z(),_concurrent_key); //0 => concurrent key
    }
    iguana::physics::InclusiveKinematicsVars doInclusiveKinematics(const TLorentzVector& electron){
      return _incKin->ComputeFromLepton(electron.X(),electron.Y(),electron.Z(),_concurrent_key); //0 => concurrent key
    }
   //Reload
    void Reload() const{
      auto run = C12()->runconfig()->getRun();
      if(_runNb==run) return;
      _runNb=run;
 
      if(_incKin.get())_concurrent_key=_incKin->PrepareEvent(run);
    }
 
  private:
      
    std::unique_ptr<iguana::physics::InclusiveKinematics> _incKin;

    mutable Long64_t _runNb =-1;
    mutable int _concurrent_key=0;

  };

}
