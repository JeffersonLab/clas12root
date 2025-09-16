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
#include "utracker.h"
#include "traj.h"
#include "event.h"
#include "cherenkov.h"
#include "rich.h"
#include "forwardtagger.h"
#include "mcparticle.h"
#include "mcmatch.h"


namespace clas12 {

  class region_particle  {


  public:

    region_particle()=default;
    
    // region_particle(par_ptr pars,covmat_ptr cm);
    // //For region_ft
    // region_particle(par_ptr pars,covmat_ptr cm, ft_ptr ftp);
    // //For region_cdet
    // region_particle(par_ptr pars,covmat_ptr cm,
    // 		    scint_ptr scp, trck_ptr trp, utrck_ptr utrp, traj_ptr trj);
    // //For region_fdet
    // region_particle(par_ptr pars,covmat_ptr cm, cal_ptr calp,
    // 		    scint_ptr scp, trck_ptr trp, utrck_ptr utrp, traj_ptr trj, cher_ptr chp);
    //For all regions
    region_particle(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm, cal_ptr calp,
		    scint_ptr scp, trck_ptr trp, utrck_ptr utrp, traj_ptr trj,
		    cher_ptr chp, ft_ptr ftp,event_ptr event,
		    rich_ptr rich,mcpar_ptr mcp=nullptr);

    virtual ~region_particle() =default;

    ////////////////////////////////////////////////////////////
    /// derived classes must define their own sort
    /// i.e. how the detector banks relate to that region
    virtual bool sort(){
      _pentry=_parts->getEntry();
      _allowed=bankAllowsRow(_pentry, _parts);
      //check for covariance matrix
      if(_covmat)_pcmat=_covmat->getIndex(_pentry);
      if(_mcpart)_pmc=_mcpart->match_to(_pentry);
      return true;
    }

    //solve FTB ambiguity at this stage
    int getPid() const{
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return _parts->getPid();
      return _useFTBPid*_ftbparts->getRows()?_ftbparts->getPid():_parts->getPid();
    }
    float getVt() const{
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return _parts->getVt();
      auto vt =  _useFTBPid*_ftbparts->getRows()?_ftbparts->getVt():_parts->getVt();
      if(vt==-1)return getStartTime();
      else return vt;
    }
    int getStatus() const{
      _parts->setEntry(_pentry);
       if(_ftbparts==nullptr) return _parts->getStatus();
       return _useFTBPid*_ftbparts->getRows()?_ftbparts->getStatus():_parts->getStatus();
    }
    float getChi2Pid() const{
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return  _parts->getChi2Pid();
      return _useFTBPid*_ftbparts->getRows()?_ftbparts->getChi2Pid():_parts->getChi2Pid();
    }
    float getBeta() const{
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return  _parts->getBeta();
      return _useFTBPid*_ftbparts->getRows()?_ftbparts->getBeta():_parts->getBeta();
    }
 
    
    virtual double getTime() const=0;
    virtual double getPath() const=0;
    virtual double getDetEnergy() const=0;
    virtual double getDeltaEnergy() const=0;
    virtual short getSector()  const=0;

    event_ptr event() const{return _event;};
    const par_ptr par() const{_parts->setEntry(_pentry);return _parts;};
    ftbpar_ptr ftbpar() const{_ftbparts->setEntry(_pentry);return _ftbparts;};
    covmat_ptr covmat() const{_covmat->setIndex(_pcmat);return _covmat;};
    virtual  cal_ptr cal(ushort lay) const{_cal->setIndex(-1);return _cal;};
    virtual  scint_ptr sci(ushort lay) const{_scint->setIndex(-1);return _scint;};
    virtual  trck_ptr trk(ushort lay) const{_trck->setIndex(-1);return _trck;};
    virtual  utrck_ptr utrk(ushort lay) const{_utrck->setIndex(-1);return _utrck;};
    virtual  traj_ptr traj(ushort det,ushort layer=0) const{_traj->setIndex(-1);return _traj;};
    virtual  cher_ptr che(ushort lay) const{_cher->setIndex(-1);return _cher;};
    virtual  rich_ptr rich() const{	_rich->setIndex(-1);return _rich;}
    
    virtual  ft_ptr ft(ushort lay) const{_ft->setIndex(-1);return _ft;};

    const CovMatrix* cmat() const{_covmat->setIndex(_pcmat);return _covmat->matrix();};

    mcpar_ptr mc() const{_mcpart->setMatchEntry(_pentry,_pmc);return _mcpart;};

    short getIndex()const {return _pentry;}
      
    short getRegion() const {return _region;}
    double getTheta()  const;
    double getPhi()  const;
    double getP() const{
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return  _parts->getP();
      return _useFTBPid*_ftbparts->getRows()?_parts->getFTBP():_parts->getP();    }
    double getPx() const{
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return  _parts->getPx();
      return _useFTBPid*_ftbparts->getRows()?_parts->getFTBPx():_parts->getPx();    }
    double getPy() const{
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return  _parts->getPy();
      return _useFTBPid*_ftbparts->getRows()?_parts->getFTBPy():_parts->getPy();    }
    double getPz() const{
      _parts->setEntry(_pentry);
      if(_ftbparts==nullptr) return  _parts->getPz();
      return _useFTBPid*_ftbparts->getRows()?_parts->getFTBPz():_parts->getPz();    }
    double getStartTime() const{
      if(_ftbparts==nullptr) return  _event->getStartTime();
      return _useFTBPid*_event->getFTBStartTime()?_event->getFTBStartTime():_event->getStartTime();    }

    double getCalcMass() const;
    double getGamma() const;
    double getDeltaTime() const;
    double getBetaFromP() const;
    double getPdgMass() const;

    void useFTBPid(){if(_ftbparts)_useFTBPid=1;}

    double getMCThetaDiff() const {return getTheta()-mc()->getTheta();}
    double getMCPhiDiff() const {return getPhi()-mc()->getPhi();}
    double getMCPDiff() const {return getP()-mc()->getP();}

    /// @returns true if this particle is "allowed", _e.g._, by a HIPO bank filter,
    /// which can be applied by an iguana algorithm
    bool const& isAllowed() const { return _allowed; }

    //if(_parts->getCharge())
  protected:

    /// @returns `true` if `bank` contains row `row` in its `hipo::bank::rowlist`
    /// @param row the row to look up
    /// @param bank the bank to check
    bool const bankAllowsRow(int const& row, hipo::bank const* bank) const {
      return std::find(bank->getRowList().begin(), bank->getRowList().end(), row) != bank->getRowList().end();
    }

    par_ptr _parts={nullptr};
    ftbpar_ptr _ftbparts={nullptr};
    covmat_ptr _covmat={nullptr};
    ft_ptr  _ft={nullptr};
    cal_ptr  _cal={nullptr};
    scint_ptr _scint={nullptr};
    trck_ptr _trck={nullptr};
    utrck_ptr _utrck={nullptr};
    traj_ptr _traj={nullptr};
    cher_ptr _cher={nullptr};
    event_ptr _event={nullptr};
    rich_ptr _rich={nullptr};
     // mcmatch_ptr _mcmatch={nullptr};
    mcpar_ptr _mcpart={nullptr};
 
    
    //particle index
    short _pentry=-1;
    short _pmc=-1;
    short _pcmat=-1;
    short _region=-1;
    short _useFTBPid=0;

    // filter status
    bool _allowed{true};
  };
  //pointer "typedef"
  using region_part_ptr=clas12::region_particle*;
  using region_part_uptr=std::unique_ptr<clas12::region_particle>;

 
}

#endif /* REGION_PARTICLE_H */
