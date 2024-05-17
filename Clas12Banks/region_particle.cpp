/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "region_particle.h"
#include <cmath>

namespace clas12 {

  // region_particle::region_particle(par_ptr pars,covmat_ptr cm):
  //   _parts(pars),
  //   _covmat(cm)
  // {
  // }
  // region_particle::region_particle(par_ptr pars,covmat_ptr cm, ft_ptr ftp):
  //   _parts(pars),
  //   _covmat(cm),
  //   _ft(ftp)
  // {
    
  // }
  // region_particle::region_particle(par_ptr pars,covmat_ptr cm, scint_ptr scp, trck_ptr trp, traj_ptr trj):
  //   _parts(pars),
  //   _covmat(cm),
  //   _scint(scp),
  //   _trck(trp),
  //   _traj(trj)
  // {
    
  // }
  // region_particle::region_particle(par_ptr pars,covmat_ptr cm, cal_ptr calp, scint_ptr scp, trck_ptr trp, traj_ptr trj, cher_ptr chp):
  //   _parts(pars),
  //   _covmat(cm),
  //   _cal(calp),
  //   _scint(scp),
  //   _trck(trp),
  //   _traj(trj),
  //   _cher(chp)
  // {
    
  // }
  region_particle::region_particle(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm, cal_ptr calp, scint_ptr scp, trck_ptr trp, utrck_ptr utrp, traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event,rich_ptr rich, mcpar_ptr mcp):
    _parts(pars),
    _ftbparts(ftbpars),
    _covmat(cm),
    _cal(calp),
    _scint(scp),
    _trck(trp),
    _utrck(utrp),
    _traj(trj),
    _cher(chp),
    _ft(ftp),
    _event(event),
    _rich(rich),
    _mcpart(mcp)
  {
    
  }
 
  double region_particle::getTheta() const {
    _parts->setEntry(_pentry);
    double x=getPx();
    double y=getPy();
    double z=getPz();
    return  x == 0.0 && y == 0.0 && z == 0.0 ? 0.0
      : atan2(sqrt(x*x+y*y),z);
  }
  double region_particle::getPhi() const {
    _parts->setEntry(_pentry);
    double x=getPx();
    double y=getPy();
    return atan2(y,x);
  }
   double region_particle::getCalcMass() const {
    double p=getP();
    double t=getTime()-getStartTime();
    double d=getPath()/100;
    if(t==0)
      return 0;
    if(p==0)
      return 0;
    double mass= p/getBeta()/getGamma();
    return mass;
  }
  ////clashes with bank item
  // float region_particle::getBeta() {
  //   float t=getTime()-_event->getStartTime();
  //   float d=getPath()/100;
  //   if(t==0)
  //     return 0;
  //   float beta= d/t/2.9979246e+08*1E9;
  //   return beta;
  // }
  double region_particle::getGamma()  const{
    double beta=getBeta();
    double gamma= sqrt(1/(1-beta*beta));
    return gamma;
  }
  double region_particle::getDeltaTime()  const{
    _parts->setEntry(_pentry);
    double t=getTime()-getVt();
    double d=getPath()/100;
    return t-d/getBetaFromP()/2.99792e+08*1E9;
  }
  double region_particle::getBetaFromP()  const{
    double pp=getP();
    auto mass=getPdgMass();
    return pp/sqrt(pp*pp+mass*mass);
  }
  double region_particle::getPdgMass() const {

    switch (getPid()) {

    case 22 :
      return 0.;
    case 11 :
      return 0.00051099891;
    case 211:
      return 0.13957000;
    case -211:
      return 0.13957000;
    case 2212:
      return 0.93827200;
    case 2112:
      return 0.93956500;
    case 321:
      return 0.49367700;
    case -321:
      return 0.49367700;
     case -11 :
      return 0.00051099891;
    case -2212:
      return 0.93827200;
    case 45:
      return 1.875612;
    default :
      return 0;
    }
  }
}
