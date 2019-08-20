/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   region_cdet.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef REGION_CDET_H
#define REGION_CDET_H

#include "clas12defs.h"
#include "region_particle.h"
#include "calorimeter.h"
#include "scintillator.h"
#include "tracker.h"
#include "traj.h"
#include "cherenkov.h"


namespace clas12 {

 
  class region_cdet  : public region_particle {


  public:

    region_cdet(par_ptr pars,covmat_ptr cm, scint_ptr scp,
		trck_ptr trp,traj_ptr trj);
    region_cdet(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm, cal_ptr calp, scint_ptr scp,
		trck_ptr trp, traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event);
    ~region_cdet()=default;

    
    bool sort() final;

    const scint_ptr sci(ushort lay) const final;
    const traj_ptr traj(ushort det,ushort layer=0) const final;
    const trck_ptr trk(ushort lay) const final{_trck->setIndex(_ptrck);return _trck;};
    
     
    
    double getTime() final{
      if(_ptof){
	_scint->setIndex(_ptof);
	return _scint->getTime();
      }
      _scint->setIndex(_pcnd);
      return _scint->getTime();
      
    }
    double getPath() final{
      if(_ptof){
	_scint->setIndex(_ptof);
	return _scint->getPath();
      }
      _scint->setIndex(_pcnd);
      return _scint->getPath();
    }

    double getDetEnergy() final{
      //sum of cnd layers
      _scint->setIndex(_pcnd1);
      double energy=_scint->getEnergy();
      _scint->setIndex(_pcnd2);
      energy+=_scint->getEnergy();
      _scint->setIndex(_pcnd3);
      energy+=_scint->getEnergy();
      return energy;
    }
 
    double getDeltaEnergy() final{
      _scint->setIndex(_ptof);
      return _scint->getEnergy();
    }

    short getSector() final{
      return 0;
    }

    
    private:

    
  
    //scintillator indices
    short _pcnd=-1;
    short _pcnd1=-1;
    short _pcnd2=-1;
    short _pcnd3=-1;
    short _ptof=-1;
    //track index
    short _ptrck=-1;
    
   
    
  };
  
  using region_cdet_ptr=std::shared_ptr<clas12::region_cdet>;

}

#endif /* REGION_CDET_H */
