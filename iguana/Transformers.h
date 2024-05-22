#pragma once

#include "IguanaAlgo.h"
#include <iguana/algorithms/clas12/MomentumCorrection/Algorithm.h>
#include <TLorentzVector.h>

namespace clas12root{

     
  class Transformers : public IguanaAlgo {

  public:
      
    iguana::clas12::MomentumCorrection* MomentumCorrection(){return _momCorr.get();}

    void Use(const string& name){

      //Initialise the momentum corrections algorithm
      if(name=="clas12::MomentumCorrection"){
	_momCorr.reset(new iguana::clas12::MomentumCorrection);
	AddAlgo(_momCorr.get());
      }
      // ...more algorithms
    }
   
    void doCorrections(std::vector<const clas12::region_particle*>& parts,
		       std::vector<FourVector*>& p4s,
		       std::function< void (const clas12::region_particle *p, FourVector& p4) > fun) const{
      
      for(uint i=0;i<parts.size();++i){
	fun(parts[i],*p4s[i]);
      }

    }

    //specific corrections
    //momentum corrections
    void  doMomentumCorrection(const clas12::region_particle *p, FourVector& p4) const{
      
      auto [px, py, pz] = _momCorr->Transform(p->getPx(),p->getPy(),p->getPz(),p->getSector(),p->getPid(),C12()->runconfig()->getTorus());

      //set the FourVector X,Y,Z,M
      p4.SetCoordinates(px,py,pz,p4.M());

    }
    
    void doMomentumCorrections(std::vector<const clas12::region_particle*> parts,
			       std::vector<FourVector*> p4s){

      return doCorrections(parts,p4s,
			   [this](const clas12::region_particle *p,FourVector& p4){
			     return doMomentumCorrection(p,p4);
			   });
    }







   //interface to vector transforms
    /*void doCorrections(std::vector<const clas12::region_particle*> parts,
		       std::vector<TLorentzVector*> p4s,
		       std::function< void (const clas12::region_particle *p, TLorentzVector& p4) > fun) const{
      
      for(uint i=0;i<parts.size();++i){
	fun(parts[i],*p4s[i]);
      }

      }*/
     
    /* void  doMomentumCorrection(const clas12::region_particle *p, TLorentzVector& p4) const{
      
      auto [px, py, pz] = _momCorr->Transform(p->getPx(),p->getPy(),p->getPz(),p->getSector(),p->getPid(),C12()->runconfig()->getTorus());
      
      p4.SetXYZM(px,py,pz,p4.M());

    }
    
    void doMomentumCorrections(std::vector<const clas12::region_particle*> parts,
			       std::vector<TLorentzVector*> p4s){

      return doCorrections(parts,p4s,
			   [this](const clas12::region_particle *p,TLorentzVector& p4){
			     return doMomentumCorrection(p,p4);
			   });
    }
    */

  private:
    
    std::unique_ptr<iguana::clas12::MomentumCorrection> _momCorr;
 
    
  };
    
}

