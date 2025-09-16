/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "region_cdet.h"

namespace clas12 {

  ///////////////////////////////////////////////////////
  /// constructor used for stand-alone
 // region_cdet::region_cdet(par_ptr pars,covmat_ptr cm, scint_ptr scp, trck_ptr trp, traj_ptr trj):
 //   region_particle(pars,cm,scp,trp,trj)
 //  {
 //    _region=clas12::CD;
 //  }
  ///////////////////////////////////////////////////////
  ///Constructor used for event,
  ///give all detector banks to prevent crashes when looking for FT 
  region_cdet::region_cdet(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm, cal_ptr calp, scint_ptr scp, trck_ptr trp,utrck_ptr utrp, traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event,rich_ptr rich,mcpar_ptr mcp):
    region_particle(pars,ftbpars,cm,calp,scp,trp,utrp,trj,chp,ftp,event,rich,mcp)
  {
    _region=clas12::CD;
  }

  ///////////////////////////////////////////////////////
  /// check if any CD detectors associated with
  /// current particle and store the detector indices
  bool region_cdet::sort(){
    
    region_particle::sort();
    _ptof=-1;
    _pcnd=-1;

    // FIXME: should we modify `_allowed` here, since we are looking at additional/other banks?
    
    //tof 1 layer
    if(_scint){
      _ptof=_scint->getIndex(_pentry,clas12::CTOF,1);//default layer=1
      if(_ptof==-1)_ptof=_scint->getIndex(_pentry,clas12::CTOF);//to be deprecated
    }
    //prefer cnd layer 1,2, 3
    if(_scint)_pcnd1=_scint->getIndex(_pentry,clas12::CND,clas12::CND1-clas12::CNDOFF);
    if(_scint)_pcnd2=_scint->getIndex(_pentry,clas12::CND,clas12::CND2-clas12::CNDOFF);
    if(_scint)_pcnd3=_scint->getIndex(_pentry,clas12::CND,clas12::CND3-clas12::CNDOFF);
    if(_pcnd1!=-1) _pcnd=_pcnd1;
    else if(_pcnd2!=-1) _pcnd=_pcnd2;
    else if(_pcnd3!=-1) _pcnd=_pcnd3;
    
    //should be 1 track per particle
    if(_trck){
      _ptrck=_trck->getIndex(_pentry,clas12::CVT);
      //alternative CD tracker RTPC
      if(_ptrck==-1)_trck->getIndex(_pentry,clas12::RTPC);
    }
    //was cdet involved ?
    if((_ptof+_pcnd+_ptrck) == -3)return false;
    return  true;
  }

  ///////////////////////////////////////////////////////
  /// Get pointer to scintillator banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->scint(CND1)->getEnergy();
  ///      p->scint(CTOF)->getTime();
  scint_ptr region_cdet::sci(ushort lay) const {
    switch(lay){ 
    case clas12::CTOF :
      _scint->setIndex(_ptof);return _scint;
    case clas12::CND1 :
      _scint->setIndex(_pcnd1);return _scint;
    case clas12::CND2 :
      _scint->setIndex(_pcnd2);return _scint;
    case clas12::CND3 :
      _scint->setIndex(_pcnd3);return _scint;
    }
    _scint->setIndex(-1);return _scint;
  }
  
  ///////////////////////////////////////////////////////
  /// Get pointer to traj banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->traj(CND,CD1)->getCx(); //detecor with layer
  ///      p->traj(CTOF)->getX();
  traj_ptr region_cdet::traj(ushort det,ushort layer) const {
    _traj->getIndex(_pentry,det,layer);
    return _traj;
  }

  
}
