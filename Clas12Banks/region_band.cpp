/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "region_band.h"
#include <iostream>

namespace clas12 {

  ///////////////////////////////////////////////////////
  ///Constructor used for event,
  ///give all detector banks to prevent crashes when looking for FD
  region_band::region_band(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm, cal_ptr calp, scint_ptr scp, trck_ptr trp, utrck_ptr utrp, traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event,rich_ptr rich,mcpar_ptr mcp):
    region_particle(pars,ftbpars,cm,calp,scp,trp,utrp,trj,chp,ftp,event,rich,mcp)
  {
    _region=clas12::BD;
  }
  
  ////////////////////////////////////////////////////////
  ///check if any FT detectors associated with
  ///current particle and store the detector indices
  bool region_band::sort(){
    region_particle::sort();
    _ptof=-1;
    _ptof1=-1;
    _ptof2=-1;
    _ptof3=-1;
    _ptof4=-1;
    _ptof5=-1;
    _pveto=-1;

    // FIXME: should we modify `_allowed` here, since we are looking at additional/other banks?

    if(_scint){
      _ptof1=_scint->getIndex(_pentry,clas12::BAND,clas12::BTOF1-clas12::BANDOFF);
      _ptof2=_scint->getIndex(_pentry,clas12::BAND,clas12::BTOF2-clas12::BANDOFF);
      _ptof3=_scint->getIndex(_pentry,clas12::BAND,clas12::BTOF3-clas12::BANDOFF);
      _ptof4=_scint->getIndex(_pentry,clas12::BAND,clas12::BTOF4-clas12::BANDOFF);
      _ptof5=_scint->getIndex(_pentry,clas12::BAND,clas12::BTOF5-clas12::BANDOFF);
      if(_ptof1!=-1) _ptof=_ptof1;
      else if(_ptof2!=-1) _ptof=_ptof2;
      else if(_ptof3!=-1) _ptof=_ptof3;
      else if(_ptof4!=-1) _ptof=_ptof4;
      else if(_ptof5!=-1) _ptof=_ptof5;

      _pveto=_scint->getIndex(_pentry,clas12::BAND,clas12::BVETO-clas12::BANDOFF);
 
      
    }
    if(_ptof!=-1) return true;
    return false;
    
  }
  ///////////////////////////////////////////////////////
  /// Get pointer to scintillator banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->scint(BTOF)->getEnergy();
  ///      p->scint(BVETO)->getTime();
  scint_ptr region_band::sci(ushort lay) const {
    switch(lay){ 
    case clas12::BVETO :
      _scint->setIndex(_pveto);return _scint;
    default :
      if(_ptof!=-1)_scint->setIndex(_ptof);return _scint;
     
    }
    _scint->setIndex(-1);return _scint;
  }

  ///////////////////////////////////////////////////////
  /// Get pointer to traj banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->traj(BAND,BTOF1)->getCx(); //detector without layer
  ///      p->traj(BAND,BVETO)->getX();
  traj_ptr region_band::traj(ushort det,ushort layer) const {
    _traj->getIndex(_pentry,det,layer);
   return _traj;
  }
  

}
