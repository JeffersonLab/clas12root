#pragma once

#include "IguanaAlgo.h"
#include <iguana/algorithms/clas12/MomentumCorrection/Algorithm.h>
#include <iguana/algorithms/clas12/FTEnergyCorrection/Algorithm.h>
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
      else if(name=="clas12::FTEnergyCorrection"){
	_ftECorr.reset(new iguana::clas12::FTEnergyCorrection);
	AddAlgo(_ftECorr.get());
      }
      else{
	std::cerr<<"Iguana -> clas12root::Transformers routine "<<name<<" is not implemented, if it exists in iguana you should still be able to use it by creating your won instance. See Example Ex11_Iguana_example_01_bank_rows.C"<<std::endl; exit(0);
      }
      // ...more algorithms
    }
   
   
    //specific corrections
    //momentum corrections
    void  doMomentumCorrection(const clas12::region_particle *p, FourVector& p4) const{
      
      auto [px, py, pz] = _momCorr->Transform(p->getPx(),p->getPy(),p->getPz(),p->getSector(),p->getPid(),C12()->runconfig()->getTorus());

      //set the FourVector X,Y,Z,M
      p4.SetCoordinates(px,py,pz,p4.M());

    }
    
     //FT momentum corrections
    void  doFTEnergyCorrection(const clas12::region_particle *p, FourVector& p4) const{
      
      auto [px, py, pz, en] = _ftECorr->Transform(p->getPx(),p->getPy(),p->getPz(),p->getSector());
      //set the FourVector X,Y,Z,T
      p4.SetXYZT(px,py,pz,en);

    }
    
 

    //Configure to automate the right correction for the particle
    void doAllCorrections(std::vector<const clas12::region_particle*> parts,
		       std::vector<FourVector*> p4s) const{

      if(parts.size()!=p4s.size()){
	std::cerr<<"Iguana-> clas12root::Transformers::doCorrections : Different number of particles, "<<parts.size()<<" to 4 vectors, "<<p4s.size()<<std::endl;exit(0);
      }
      
      //loop over all particles
      for(uint i=0;i<parts.size();++i){

	switch( parts[i]->getRegion() ){
	  //Forward tagger corrections
	case clas12::FT :
	  if(_ftECorr.get()){
	    doFTEnergyCorrection(parts[i],*p4s[i]);
	  }
	  
	  break;
	  
	case clas12::FD :
	  //Forward detector corrections
	  if(_momCorr.get()){
	    doMomentumCorrection(parts[i],*p4s[i]);
	  }
	  
	  break;
	  
	case clas12::CD :

	  break;
	  
	default:
	  break;
	}//end region switch
      }//particle loop

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
    std::unique_ptr<iguana::clas12::FTEnergyCorrection> _ftECorr;
 
    
  };
    
}

