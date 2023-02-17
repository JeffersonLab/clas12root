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
#include "mcparticle.h"
#include "mcmatch.h"


namespace clas12 {

  class region_particle  {


  public:

    region_particle()=default;
    
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
		    cher_ptr chp, ft_ptr ftp,event_ptr event,
		    mcpar_ptr mcp=nullptr);

    virtual ~region_particle() =default;

    ////////////////////////////////////////////////////////////
    /// derived classes must define their own sort
    /// i.e. how the detector banks relate to that region
    virtual bool sort(){
      _pentry=_parts->getEntry();
      //check for covariance matrix
      if(_covmat)_pcmat=_covmat->getIndex(_pentry);
      if(_mcpart)_pmc=_mcpart->match_to(_pentry);
      return true;
    }

    //solve FTB ambiguity at this stage
    int getPid(){
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return _parts->getPid();
      return _useFTBPid*_ftbparts->getRows()?_ftbparts->getPid():_parts->getPid();
    }
    float getVt(){
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return _parts->getVt();
      return _useFTBPid*_ftbparts->getRows()?_ftbparts->getVt():_parts->getVt();
    }
    int getStatus(){
      _parts->setEntry(_pentry);
       if(_ftbparts==nullptr) return _parts->getStatus();
       return _useFTBPid*_ftbparts->getRows()?_ftbparts->getStatus():_parts->getStatus();
    }
    float getChi2Pid(){
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return  _parts->getChi2Pid();
      return _useFTBPid*_ftbparts->getRows()?_ftbparts->getChi2Pid():_parts->getChi2Pid();
    }
    float getBeta(){
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return  _parts->getBeta();
      return _useFTBPid*_ftbparts->getRows()?_ftbparts->getBeta():_parts->getBeta();
    }
 
    virtual double getTime()=0;
    virtual double getPath()=0;
    virtual double getDetEnergy()=0;
    virtual double getDeltaEnergy()=0;
    virtual short getSector() =0;

    event_ptr event() const{return _event;};
    par_ptr par() const{_parts->setEntry(_pentry);return _parts;};
    ftbpar_ptr ftbpar() const{_ftbparts->setEntry(_pentry);return _ftbparts;};
    covmat_ptr covmat() const{_covmat->setIndex(_pcmat);return _covmat;};
    virtual  cal_ptr cal(ushort lay) const{_cal->setIndex(-1);return _cal;};
    virtual  scint_ptr sci(ushort lay) const{_scint->setIndex(-1);return _scint;};
    virtual  trck_ptr trk(ushort lay) const{_trck->setIndex(-1);return _trck;};
    virtual  traj_ptr traj(ushort det,ushort layer=0) const{_traj->setIndex(-1);return _traj;};
    virtual  cher_ptr che(ushort lay) const{_cher->setIndex(-1);return _cher;};
    virtual  ft_ptr ft(ushort lay) const{_ft->setIndex(-1);return _ft;};

    const CovMatrix* cmat() const{_covmat->setIndex(_pcmat);return _covmat->matrix();};

    mcpar_ptr mc() const{_mcpart->setMatchEntry(_pentry,_pmc);return _mcpart;};

    short getIndex()const {return _pentry;}
      
    short getRegion() const {return _region;}
    double getTheta() const;
    double getPhi() const;
    double getP(){
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return  _parts->getP();
      return _useFTBPid*_ftbparts->getRows()?_parts->getFTBP():_parts->getP();    }

    double getCalcMass();
    double getGamma();
    double getDeltaTime();
    double getBetaFromP();
    double getPdgMass();

    void useFTBPid(){if(_ftbparts)_useFTBPid=1;}

    double getMCThetaDiff() {return getTheta()-mc()->getTheta();}
    double getMCPhiDiff() {return getPhi()-mc()->getPhi();}
    double getMCPDiff() {return getP()-mc()->getP();}

    //if(_parts->getCharge())
  protected:

    par_ptr _parts={nullptr};
    ftbpar_ptr _ftbparts={nullptr};
    covmat_ptr _covmat={nullptr};
    ft_ptr  _ft={nullptr};
    cal_ptr  _cal={nullptr};
    scint_ptr _scint={nullptr};
    trck_ptr _trck={nullptr};
    traj_ptr _traj={nullptr};
    cher_ptr _cher={nullptr};
    event_ptr _event={nullptr};
    // mcmatch_ptr _mcmatch={nullptr};
    mcpar_ptr _mcpart={nullptr};
 
    
    //particle index
    short _pentry=-1;
    short _pmc=-1;
    short _pcmat=-1;
    short _region=-1;
    short _useFTBPid=0;
  };
  //pointer "typedef"
  using region_part_ptr=clas12::region_particle*;
  using region_part_uptr=std::unique_ptr<clas12::region_particle>;

 
}

#endif /* REGION_PARTICLE_H */
