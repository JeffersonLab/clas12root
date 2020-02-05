/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   clas12reader.h
 * Author: dglazier
 *
 */

#ifndef CLAS12READER_H
#define CLAS12READER_H

#include "clas12defs.h"
#include "reader.h"
#include "particle.h"
#include "ftbparticle.h"
#include "mcparticle.h"
#include "calorimeter.h"
#include "scintillator.h"
#include "tracker.h"
#include "traj.h"
#include "cherenkov.h"
#include "event.h"
#include "runconfig.h"
#include "ftbevent.h"
#include "vtp.h"
#include "scaler.h"
#include "covmatrix.h"
#include "helonline.h"
#include "helflip.h"
#include "forwardtagger.h"
#include "region_particle.h"
#include "region_fdet.h"
#include "region_cdet.h"
#include "region_ft.h"
#include "dictionary.h"

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>


namespace clas12 {
  using std::cout;
  using std::endl;

  class clas12reader  {


  public:


    clas12reader()=default;
    clas12reader(std::string filename,std::vector<long> tags=std::vector<long>());
    virtual ~clas12reader()=default;

    hipo::reader& getReader(){return _reader;}
    
    bool next();
    bool nextInRecord();
    void sort();
    bool readEvent();
    void clearEvent();
    void makeListBanks();
    
    std::vector<hipo::bank* > getAllBanksPtrs(){return _allBanks;}
    hipo::dictionary& getDictionary(){return _factory;}
    
    void addARegionFDet(){
      //Forward detector needs particles, calorimeter, scintillator,
      //track, cherenkov
      region_fdet_uptr  reg{new region_fdet{_bparts.get(),_bftbparts.get(),_bcovmat.get(),_bcal.get(),_bscint.get(),_btrck.get(),_btraj.get(),_bcher.get(),_bft.get(),_bevent.get()}};
      if(_useFTBased)reg->useFTBPid();
     _rfdets.push_back(std::move(reg));
    }
     void addARegionCDet(){
      //Forward detector needs particles, calorimeter, scintillator,
      //track, cherenkov
       region_cdet_uptr  reg{new region_cdet{_bparts.get(),_bftbparts.get(),_bcovmat.get(),_bcal.get(),_bscint.get(),_btrck.get(),_btraj.get(),_bcher.get(),_bft.get(),_bevent.get()}};
      if(_useFTBased)reg->useFTBPid();
      _rcdets.push_back(std::move(reg));
    }
    void addARegionFT(){
      //Forward tagger needs particles and forward tagger
       region_ft_uptr  reg{new region_ft{_bparts.get(),_bftbparts.get(),_bcovmat.get(),_bcal.get(),_bscint.get(),_btrck.get(),_btraj.get(),_bcher.get(),_bft.get(),_bevent.get()}};
       if(_useFTBased)reg->useFTBPid();
      _rfts.push_back(std::move(reg));
     }


    helonline_ptr helonline() const{return _bhelonline.get();};
    helflip_ptr helflip() const{return _bhelflip.get();};
    runconfig_ptr runconfig() const{return _brunconfig.get();};
    event_ptr event() const{return _bevent.get();};
    ftbevent_ptr ftbevent() const{return _bftbevent.get();};
    vtp_ptr vtp() const{return _bvtp.get();};
    scaler_ptr scaler() const{return _bscal.get();};
    mcpar_ptr mcparts() const{return _bmcparts.get();};


    //support for generic non-DST banks
    uint addBank(std::string name){
      std::unique_ptr<hipo::bank> bnk{new hipo::bank{_factory.getSchema(name.data())}};
      _addBanks.push_back(std::move(bnk));
      return _addBanks.size()-1; //return place in vector
    }

    hipo::bank* getBank(uint index) const {return  _addBanks.at(index).get();}
    int getBankOrder(int ibank,std::string itemname ) const{
      return getBank(ibank)->getSchema().getEntryOrder(itemname.data());
    }
    /////////////////////////////////
    
    std::vector<region_part_ptr>& getDetParticles(){return _detParticles;}
    std::vector<region_part_ptr>* getDetParticlesPtr(){return &_detParticles;}
    std::vector<region_part_ptr> getByID(int id);
    std::vector<region_part_ptr> getByRegion(int ir);
    std::vector<region_part_ptr> getByCharge(int ch);
    
    const std::vector<short>& preCheckPids();

    short getNPid(short pid)const noexcept{return std::count(_pids.begin(),_pids.end(), pid);};
    void addAtLeastPid(short pid,short n){
      _pidSelect[pid]=n;
      _givenPids.push_back(pid);
    }
    void addExactPid(short pid,short n){
      _pidSelectExact[pid]=n;
      _givenPids.push_back(pid);
    }
    void addZeroOfRestPid(){_zeroOfRestPid=true;};
    
    bool passPidSelect();

    void useFTBased(){_useFTBased=true;}
    
    int getNParticles() const  noexcept{return _detParticles.size();}
    const std::vector<short> &getPids() const  noexcept{return _pids;}
    
    bool checkTriggerBit(uint k){
      long pattern = _brunconfig->getTrigger();
      return ( pattern & (1<<k)) != 0;
    }

  private:
    
    void hipoRead(){
      _reader.read(_event);
      _isRead=true;
    }
    
    //reader
    hipo::reader     _reader;
    hipo::event      _event;
    hipo::dictionary  _factory;

    //DST banks
    helonline_uptr  _bhelonline;
    helflip_uptr  _bhelflip;
    runconfig_uptr  _brunconfig;
    event_uptr  _bevent;
    ftbevent_uptr  _bftbevent;
    par_uptr _bparts;
    //    std::unique_ptr<clas12::particle> _ownbparts;
    ftbpar_uptr _bftbparts;
    mcpar_uptr _bmcparts;
    covmat_uptr _bcovmat;
    cal_uptr  _bcal;
    scint_uptr _bscint;
    trck_uptr _btrck;
    traj_uptr _btraj;
    cher_uptr _bcher;
    ft_uptr _bft;
    vtp_uptr _bvtp;
    scaler_uptr _bscal;

    std::vector<std::unique_ptr<hipo::bank> > _addBanks; //owns additional banks
    std::vector<hipo::bank* > _allBanks; 
    
    //Detector region vectors,
    //each particle in an event will have
    //one associated, these vectors own the ptrs
    std::vector<region_fdet_uptr> _rfdets;
    std::vector<region_cdet_uptr> _rcdets;
    std::vector<region_ft_uptr> _rfts;

    //this vector links to raw ptrs, does not own
    std::vector<region_part_ptr> _detParticles;

    std::vector<short> _pids;
    std::vector<short> _givenPids;
    std::map<short,short> _pidSelect;
    std::map<short,short> _pidSelectExact;

    long _nevent=0;
    ushort _nparts=0;
    ushort _n_rfdets=0;
    ushort _n_rcdets=0;
    ushort _n_rfts=0;
    bool _zeroOfRestPid=false;
    bool _useFTBased=false;
    bool _isRead=false;
  };
  //helper functions
  
  //filter vectors via a lambda function
  //see for example clas12reader::getByID()
  template <typename Cont, typename Pred>
    Cont container_filter(const Cont &container, Pred predicate){
    Cont result;
    std::copy_if(container.begin(),container.end(),std::back_inserter(result), predicate);
    return std::move(result);
  }

}

#endif /* CLAS12READER_H */
