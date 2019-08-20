/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   region_particle.h
 * Author: dglazier
 *
 */

#ifndef REGION_PARTICLE_H
#define REGION_PARTICLE_H

#include "clas12defs.h"
#include "particle.h"
#include "ftbparticle.h"
#include "covmatrix.h"
#include "calorimeter.h"
#include "scintillator.h"
#include "tracker.h"
#include "traj.h"
#include "event.h"
#include "cherenkov.h"
#include "forwardtagger.h"


namespace clas12 {

  class region_particle  {


  public:

    region_particle(){};
    
    region_particle(par_ptr pars,covmat_ptr cm);
    //For region_ft
    region_particle(par_ptr pars,covmat_ptr cm, ft_ptr ftp);
    //For region_cdet
    region_particle(par_ptr pars,covmat_ptr cm,
		    scint_ptr scp, trck_ptr trp, traj_ptr trj);
    //For region_fdet
    region_particle(par_ptr pars,covmat_ptr cm, cal_ptr calp,
		    scint_ptr scp, trck_ptr trp, traj_ptr trj, cher_ptr chp);
    //For all regions
    region_particle(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm, cal_ptr calp,
		    scint_ptr scp, trck_ptr trp, traj_ptr trj,
		    cher_ptr chp, ft_ptr ftp,event_ptr event);

    virtual ~region_particle()=default;

    ////////////////////////////////////////////////////////////
    /// derived classes must define their own sort
    /// i.e. how the detector banks relate to that region
    virtual bool sort(){
      _pentry=_parts->getEntry();
      //check for covarince matrix
      if(_covmat.get())_pcmat=_covmat->getIndex(_pentry);
       return true;
    }

    int getPid(){_parts->setEntry(_pentry);return _parts->getPid();}
    
    virtual double getTime()=0;
    virtual double getPath()=0;
    virtual double getDetEnergy()=0;
    virtual double getDeltaEnergy()=0;
    virtual short getSector() =0;

    const event_ptr event() const{return _event;};
    const par_ptr par() const{_parts->setEntry(_pentry);return _parts;};
    const ftbpar_ptr ftbpar() const{_ftbparts->setEntry(_pentry);return _ftbparts;};
    const covmat_ptr covmat() const{_covmat->setIndex(_pcmat);return _covmat;};
    virtual const cal_ptr cal(ushort lay) const{_cal->setIndex(-1);return _cal;};
    virtual const scint_ptr sci(ushort lay) const{_scint->setIndex(-1);return _scint;};
    virtual const trck_ptr trk(ushort lay) const{_trck->setIndex(-1);return _trck;};
    virtual const traj_ptr traj(ushort det,ushort layer=0) const{_traj->setIndex(-1);return _traj;};
    virtual const cher_ptr che(ushort lay) const{_cher->setIndex(-1);return _cher;};
    virtual const ft_ptr ft(ushort lay) const{_ft->setIndex(-1);return _ft;};

    const CovMatrix* cmat() const{_covmat->setIndex(_pcmat);return _covmat->matrix();};
 
    
    short getRegion() const {return _region;}
    float getTheta() const;
    float getPhi() const;
    float getP(){_parts->setEntry(_pentry);return _parts->getP();}
    float getCalcMass();
    float getBeta();
    float getGamma();
    
  protected:

    par_ptr _parts;
    ftbpar_ptr _ftbparts;
    covmat_ptr _covmat;
    ft_ptr  _ft;
    cal_ptr  _cal;
    scint_ptr _scint;
    trck_ptr _trck;
    traj_ptr _traj;
    cher_ptr _cher;
    event_ptr _event;
 
    
    //particle index
    short _pentry=-1;
    short _pcmat=-1;
    short _region=-1;
  
  };
  //pointer "typedef"
  using region_part_ptr=std::shared_ptr<clas12::region_particle>;

 
}

#endif /* REGION_PARTICLE_H */
