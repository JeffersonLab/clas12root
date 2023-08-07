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

namespace clas12 {

 
  class region_fdet  : public region_particle {


  public:

    // region_fdet()=default;
    region_fdet(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm,
		cal_ptr calp, scint_ptr scp, trck_ptr trp, utrck_ptr utrp,
		traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event,
		rich_ptr rich,mcpar_ptr mcp=nullptr);
    ~region_fdet() final =default;

    
    bool sort() final;

    cal_ptr cal(ushort lay) const final;
    scint_ptr sci(ushort lay) const final;
    traj_ptr traj(ushort det,ushort layer=0) const final;
    trck_ptr trk(ushort lay) const final
    {
      _trck->setIndex(_ptrck);return _trck;
    }
    utrck_ptr utrk(ushort lay) const final
    {
      _utrck->setIndex(_ptrck);return _utrck;
    }
    cher_ptr che(ushort lay) const final;
    rich_ptr rich() const final;
     

    double getTime() final{
      if(_ptof>-1){
	_scint->setIndex(_ptof);
	return _scint->getTime();
      }
      _cal->setIndex(_pcal);
      return _cal->getTime();
    }
    double getPath() final{
      if(_ptof>-1){
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
      if(_ptrck>-1){
	_trck->setIndex(_ptrck);
	return _trck->getSector();
      }
      if(_ptof>-1){
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
    short _prich=-1;
    
   
    
  };
  
  using region_fdet_ptr=clas12::region_fdet*;
  using region_fdet_uptr=std::unique_ptr<clas12::region_fdet>;

}

#endif /* REGION_FDET_H */
