/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "region_fdet.h"

namespace clas12 {
   ///////////////////////////////////////////////////////
  ///Constructor used for event,
  ///give all detector banks to prevent crashes when looking for FT 

  region_fdet::region_fdet(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm, cal_ptr calp, scint_ptr scp, trck_ptr trp, utrck_ptr utrp, traj_ptr trj,cher_ptr chp, ft_ptr ftp,event_ptr event,rich_ptr rich,mcpar_ptr mcp):
    region_particle(pars,ftbpars,cm,calp,scp,trp,utrp,trj,chp,ftp,event,rich,mcp)
  {
    _region=clas12::FD;  
  }

  ///////////////////////////////////////////////////////
  /// check if any FD detectors associated with
  /// current particle and store the detector indices
  bool region_fdet::sort(){
    region_particle::sort();
    _ptof=-1;
    _pcal=-1;
    _ptrck=-1;
    _prich=-1;
    _phtcc=-1;
    _pltcc=-1;
    //_scint->print();
    
    //prefer tof layer 1,0,2
    if(_scint)_ptof3=_scint->getIndex(_pentry,clas12::FTOF,clas12::FTOF2);
    if(_scint)_ptof1=_scint->getIndex(_pentry,clas12::FTOF,clas12::FTOF1A);
    if(_scint)_ptof2=_scint->getIndex(_pentry,clas12::FTOF,clas12::FTOF1B);
    
    if(_ptof2!=-1) _ptof=_ptof2;
    else if(_ptof1!=-1) _ptof=_ptof1;
    else if(_ptof3!=-1) _ptof=_ptof3;

   //prefer pcal,inner, outer
    if(_cal)_pcal=_pin=_cal->getIndex( _pentry,clas12::EC, clas12::ECIN);
    if(_cal)_pcal=_pout=_cal->getIndex( _pentry,clas12::EC, clas12::ECOUT);
    if(_cal)_pcal=_ppre=_cal->getIndex( _pentry,clas12::EC, clas12::PCAL);
    if(_ppre!=-1) _pcal=_ppre;
    else if(_pout!=-1) _pcal=_pout;
    else if(_pin!=-1) _pcal=_pin;
      
    //should be 1 track per particle
    if(_trck)_ptrck=_trck->getIndex(_pentry,clas12::DC);
   
    if(_cher)_phtcc=_cher->getIndex( _pentry,clas12::HTCC);
    if(_cher)_pltcc=_cher->getIndex( _pentry,clas12::LTCC);

    if(_rich)_prich=_rich->getIndex( _pentry,clas12::RICH);

    //was fdet involved ?
    if((_ptof+_pcal+_ptrck) == -3)return false;
    return  true;
  }
  ///////////////////////////////////////////////////////
  /// Get pointer to calorimeter banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->cal(ECIN)->getEnergy();
  ///      p->cal(PCAL)->getTime();
  cal_ptr region_fdet::cal(ushort lay) const {
    switch(lay){ 
    case clas12::PCAL :
      _cal->setIndex(_ppre);return _cal;
    case clas12::ECIN :
      _cal->setIndex(_pin);return _cal;
    case clas12::ECOUT :
      _cal->setIndex(_pout);return _cal;
    }
    _cal->setIndex(-1);return _cal;
  }
 
  ///////////////////////////////////////////////////////
  /// Get pointer to scintillator banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->scint(FTOF1A)->getEnergy();
  ///      p->scint(FTOF2)->getTime();
  scint_ptr region_fdet::sci(ushort lay) const {
    switch(lay){ 
    case clas12::FTOF1A :
      _scint->setIndex(_ptof1);return _scint;
    case clas12::FTOF1B :
      _scint->setIndex(_ptof2);return _scint;
    case clas12::FTOF2 :
      _scint->setIndex(_ptof3);return _scint;
    }
    _scint->setIndex(-1);return _scint;
 }
 
  ///////////////////////////////////////////////////////
  /// Get pointer to chrenkov banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->che(HTCC)->getNphe();
  ///      p->che(LTCC)->getNphe();
  cher_ptr region_fdet::che(ushort lay) const {
    switch(lay){ 
    case clas12::HTCC :
      _cher->setIndex(_phtcc);return _cher;
    case clas12::LTCC :
      _cher->setIndex(_pltcc);return _cher;
    }
    _cher->setIndex(-1);return _cher;
  }
  
 ///////////////////////////////////////////////////////
  /// Get pointer to rich banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->rich()->getId();
  ///      p->rich()->ring()->getSector();
  rich_ptr region_fdet::rich() const {
    if(_rich)
      _rich->setIndex(_prich);

    return _rich;
  }
  
  ///////////////////////////////////////////////////////
  /// Get pointer to traj banks for this particle
  /// This should be used directly to acess data
  /// e.g. p->traj(HTCC)->getCx(); //detector without layer
  ///      p->traj(FTOF,FTOF1A)->getX(); //detector with layer
  traj_ptr region_fdet::traj(ushort det,ushort layer) const {
    _traj->getIndex(_pentry,det,layer);
    return _traj;
  }
  

  
}
