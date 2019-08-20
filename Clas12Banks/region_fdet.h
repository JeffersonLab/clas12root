/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   region_fdet.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef REGION_FDET_H
#define REGION_FDET_H

#include "clas12defs.h"
#include "region_particle.h"
#include "calorimeter.h"
#include "scintillator.h"
#include "tracker.h"
#include "traj.h"
#include "cherenkov.h"


namespace clas12 {

 
  class region_fdet  : public region_particle {


  public:

    // region_fdet()=default;
    region_fdet(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm,
		cal_ptr calp, scint_ptr scp, trck_ptr trp,
		traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event);
    ~region_fdet()=default;

    
    bool sort() final;

    const cal_ptr cal(ushort lay) const final;
    const scint_ptr sci(ushort lay) const final;
    const traj_ptr traj(ushort det,ushort layer=0) const final;
    const trck_ptr trk(ushort lay) const final
    {
      _trck->setIndex(_ptrck);return _trck;
    }
    const cher_ptr che(ushort lay) const final;
    
     

    double getTime() final{
      if(_ptof){
	_scint->setIndex(_ptof);
	return _scint->getTime();
      }
      _cal->setIndex(_pcal);
      return _cal->getTime();
    }
    double getPath() final{
      if(_ptof){
	_scint->setIndex(_ptof);
	return _scint->getPath();
      }
      _cal->setIndex(_pcal);
      return _cal->getPath();
    }

    double getDetEnergy() final{
      _cal->setIndex(_ppre);
      double energy=_cal->getEnergy();
      _cal->setIndex(_pin);
      energy+=_cal->getEnergy();
      _cal->setIndex(_pout);
      energy+=_cal->getEnergy();
      return energy;
  }
 
    double getDeltaEnergy() final{
      _scint->setIndex(_ptof);
      return _scint->getEnergy();
    }

    short getSector() final{
      if(_ptrck){
	_trck->setIndex(_ptrck);
	return _trck->getSector();
      }
      if(_ptof){
	_scint->setIndex(_ptof);
	return _scint->getSector();
      }
      _cal->setIndex(_pcal);
      return _cal->getSector();
    }

    
    private:

    
  
   //calorimeter indices
    short _pcal=-1;
    short _ppre=-1;
    short _pin=-1;
    short _pout=-1;
    //scintillator indices
    short _ptof=-1;
    short _ptof1=-1;
    short _ptof2=-1;
    short _ptof3=-1;
    //track index
    short _ptrck=-1;
    //traj index
    // short _ptraj=-1;
    //cherenkov indices
    short _phtcc=-1;
    short _pltcc=-1;
    
   
    
  };
  
  using region_fdet_ptr=std::shared_ptr<clas12::region_fdet>;

}

#endif /* REGION_FDET_H */
