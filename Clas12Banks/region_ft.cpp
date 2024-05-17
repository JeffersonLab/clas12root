/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "region_ft.h"
#include <iostream>

namespace clas12 {

  ///////////////////////////////////////////////////////
  ///Constructor used for event,
  ///give all detector banks to prevent crashes when looking for FD
  region_ft::region_ft(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm, cal_ptr calp, scint_ptr scp, trck_ptr trp,  utrck_ptr utrp, traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event,rich_ptr rich,mcpar_ptr mcp):
    region_particle(pars,ftbpars,cm,calp,scp,trp,utrp,trj,chp,ftp,event,rich,mcp)
  {
    _region=clas12::FT;
  }
  
  ///////////////////////////////////////////////////////
  /// Temporary until FT banks updated
  double region_ft::getPath() const {
    if(_pcal>=0){ //currently no FT path reported
      if(_ft)_ft->setIndex(_pcal); 	
      double ftx=_ft->getX();
      double fty=_ft->getY();
      double ftz=_ft->getZ();
      return sqrt(ftx*ftx+fty*fty+ftz*ftz);
    }
    return 0;
  }
  ////////////////////////////////////////////////////////
  ///check if any FT detectors associated with
  ///current particle and store the detector indices
  bool region_ft::sort(){
    region_particle::sort();
    if(_ft){
      _pcal=_ft->getIndex(_pentry,clas12::FTCAL,1); //default layer=1
      if(_pcal==-1)_pcal=_ft->getIndex(_pentry,clas12::FTCAL);//to be deprecated
    }
    if(_ft){
      _phod=_ft->getIndex(_pentry,clas12::FTHODO,1);
      if(_phod==-1) _phod=_ft->getIndex(_pentry,clas12::FTHODO);
    }
    //check if FT assocciated, must be a FTCAL
    if(_pcal!=-1)
      return true;
    return false;
    
  }
  ///////////////////////////////////////////////////////
  /// Get pointer to FT banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->ft(FTCAL)->getEnergy();
  ///      p->ft(FTHODO)->getEnergy();
  ft_ptr region_ft::ft(ushort lay) const {
    switch(lay){ 
    case clas12::FTCAL :
      _ft->setIndex(_pcal);return _ft;
    case clas12::FTHODO :
      _ft->setIndex(_phod);return _ft;
    }
    _ft->setIndex(-1);return _ft;
  }
  
  ///////////////////////////////////////////////////////
  /// Get pointer to traj banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->traj(FT,FTCAL)->getCx(); //detector without layer
  ///      p->traj(FT,FTHODO)->getX();
  traj_ptr region_ft::traj(ushort det,ushort layer) const {
    _traj->getIndex(_pentry,det,layer);
   return _traj;
  }
  

}
