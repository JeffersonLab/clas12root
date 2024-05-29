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
      return _incKin->ComputeFromLepton(electron->getPx(),electron->getPy(),electron->getPz());
    }
   
    iguana::physics::InclusiveKinematicsVars doInclusiveKinematics(const FourVector& electron){
      return _incKin->ComputeFromLepton(electron.X(),electron.Y(),electron.Z());
    }
    iguana::physics::InclusiveKinematicsVars doInclusiveKinematics(const TLorentzVector& electron){
      return _incKin->ComputeFromLepton(electron.X(),electron.Y(),electron.Z());
    }

  private:
      
    std::unique_ptr<iguana::physics::InclusiveKinematics> _incKin;
 
  };

}
