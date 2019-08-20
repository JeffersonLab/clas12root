/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "region_particle.h"
#include <cmath>

namespace clas12 {

  region_particle::region_particle(par_ptr pars,covmat_ptr cm):
    _parts(pars),
    _covmat(cm)
  {
  }
  region_particle::region_particle(par_ptr pars,covmat_ptr cm, ft_ptr ftp):
    _parts(pars),
    _covmat(cm),
    _ft(ftp)
  {
    
  }
  region_particle::region_particle(par_ptr pars,covmat_ptr cm, scint_ptr scp, trck_ptr trp, traj_ptr trj):
    _parts(pars),
    _covmat(cm),
    _scint(scp),
    _trck(trp),
    _traj(trj)
  {
    
  }
  region_particle::region_particle(par_ptr pars,covmat_ptr cm, cal_ptr calp, scint_ptr scp, trck_ptr trp, traj_ptr trj, cher_ptr chp):
    _parts(pars),
    _covmat(cm),
    _cal(calp),
    _scint(scp),
    _trck(trp),
    _traj(trj),
    _cher(chp)
  {
    
  }
  region_particle::region_particle(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm, cal_ptr calp, scint_ptr scp, trck_ptr trp, traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event):
    _parts(pars),
    _ftbparts(ftbpars),
    _covmat(cm),
    _cal(calp),
    _scint(scp),
    _trck(trp),
    _traj(trj),
    _cher(chp),
    _ft(ftp),
    _event(event)
  {
    
  }
  
  float region_particle::getTheta() const{
    _parts->setEntry(_pentry);
    float x=_parts->getPx();
    float y=_parts->getPy();
    float z=_parts->getPz();
    return x == 0.0 && y == 0.0 && z == 0.0 ? 0.0
      : atan2(sqrt(x*x+y*y),z);
  }
  float region_particle::getPhi() const{
    _parts->setEntry(_pentry);
    float x=_parts->getPx();
    float y=_parts->getPy();
    return atan2(y,x);
  }
  float region_particle::getCalcMass() {
    float p=getP();
    float t=getTime()-_event->getStartTime();
    float d=getPath()/100;
    if(t==0)
      return 0;
    if(p==0)
      return 0;
    float mass= d/t/p/2.9979246e+08*1E9;
    return mass;
  }
  float region_particle::getBeta() {
    float t=getTime()-_event->getStartTime();
    float d=getPath()/100;
    if(t==0)
      return 0;
    float beta= d/t/2.9979246e+08*1E9;
    return beta;
  }
  float region_particle::getGamma() {
    float beta=getBeta();
    float gamma= sqrt(1/(1-beta*beta));
    return gamma;
  }

}
