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
#include "region_band.h"
#include "scaler_reader.h"
#include "rcdb_vals.h"

#include "qadb_reader.h"

#include "dictionary.h"

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

#ifdef RCDB_MYSQL
   #include "rcdb_reader.h"
#endif

namespace clas12 {
  using std::cout;
  using std::endl;

  class clas12reader  {


  public:


    clas12reader()=default;
    clas12reader(std::string filename,std::vector<long> tags=std::vector<long>());
    clas12reader(const clas12reader &other,std::string filename="",std::vector<long> tags=std::vector<long>());
    
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
    std::string getFilename(){return _filename;}
    
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
  void addARegionBAND(){
      //Forward tagger needs particles and forward tagger
       region_band_uptr  reg{new region_band{_bparts.get(),_bftbparts.get(),_bcovmat.get(),_bcal.get(),_bscint.get(),_btrck.get(),_btraj.get(),_bcher.get(),_bft.get(),_bevent.get()}};
       if(_useFTBased)reg->useFTBPid();
      _rbands.push_back(std::move(reg));
     }


    helonline_ptr helonline() const{return _bhelonline.get();};
    helflip_ptr helflip() const{return _bhelflip.get();};
    runconfig_ptr runconfig() const{return _brunconfig.get();};
    event_ptr event() const{return _bevent.get();};
    ftbevent_ptr ftbevent() const{return _bftbevent.get();};
    vtp_ptr vtp() const{return _bvtp.get();};
    //scaler_ptr scaler() const{return _bscal.get();};
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
    bool checkVTPTriggerBit(uint k){
      long pattern = _bvtp->makeVTPTriggers();
      return ( pattern & (1<<k)) != 0;
    }

    scalerreader_ptr scalerReader(){
      if(_scalReader.get()) return _scalReader.get();
      _scalReader.reset(new scaler_reader(_filename));
      _runBeamCharge = _scalReader->getBeamCharge();
      return _scalReader.get();
    }
    double getRunBeamCharge() const noexcept{ return _runBeamCharge;}
    double getCurrApproxCharge(){return _runBeamCharge*_nevent/_reader.getEntries();}

    void summary(){
      std::cout<<"for file "<<_filename<<"\n   read "<<_nevent<<" events from which "<<_nselected<< " passed filtering conditions."<<" The beam charge to this point in the file was "<<getCurrApproxCharge()<<std::endl;
    }
    
    void getStructure(hipo::bank* bank){
      if(_isRead==false)
	hipoRead();
      _event.getStructure(*bank);
    }

    //rcdb
    static int readQuickRunConfig(const std::string& filename);
    void queryRcdb();

    void setEntries(long n){_nToProcess = n;}

    //clasqaDB
    void applyQA(std::string jsonFilePath){
      _applyQA=true;
      _qa = new qadb_reader(jsonFilePath);
    };
    void addQARequirement(std::string req){_reqsQA.push_back(req);};
    void requireOkForAsymmetry(bool ok){_reqOKAsymmetry=ok;};
    
    protected:

    void initReader();
    
    
    private:

    
    void hipoRead(){
      _reader.read(_event);
      _isRead=true;
    }

    std::string _filename;
    
    //reader
    hipo::reader     _reader;//!
    hipo::event      _event;//!
    hipo::dictionary  _factory;//!

    //DST banks
    helonline_uptr  _bhelonline;//!
    helflip_uptr  _bhelflip;//!
    runconfig_uptr  _brunconfig;//!
    event_uptr  _bevent;//!
    ftbevent_uptr  _bftbevent;//!
    par_uptr _bparts;//!
    //    std::unique_ptr<clas12::particle> _ownbparts;
    ftbpar_uptr _bftbparts;//!
    mcpar_uptr _bmcparts;//!
    covmat_uptr _bcovmat;//!
    cal_uptr  _bcal;//!
    scint_uptr _bscint;//!
    trck_uptr _btrck;//!
    traj_uptr _btraj;//!
    cher_uptr _bcher;//!
    ft_uptr _bft;//!
    vtp_uptr _bvtp;//!


    
    std::vector<std::unique_ptr<hipo::bank> > _addBanks; //!owns additional banks
    std::vector<hipo::bank* > _allBanks; //!
    
    //Detector region vectors,
    //each particle in an event will have
    //one associated, these vectors own the ptrs
    std::vector<region_fdet_uptr> _rfdets;//!
    std::vector<region_cdet_uptr> _rcdets;//!
    std::vector<region_ft_uptr> _rfts;//!
    std::vector<region_band_uptr> _rbands;//!

    //this vector links to raw ptrs, does not own
    std::vector<region_part_ptr> _detParticles;//!

     
    double _runBeamCharge{0};
    long _nevent{0};
    long _nselected{0};
    long _nToProcess{-1};
    ushort _nparts{0};
    ushort _n_rfdets{0};
    ushort _n_rcdets{0};
    ushort _n_rfts{0};
    ushort _n_rbands{0};

    std::vector<short> _pids;
    bool _isRead{false};
    bool _rcdbQueried=false;
    
    //members that need copied in constructor
    scalerreader_uptr _scalReader;
    std::vector<short> _givenPids;
    std::map<short,short> _pidSelect;
    std::map<short,short> _pidSelectExact;
    bool _zeroOfRestPid{false};
    bool _useFTBased{false};

       //rcdb
    int _runNo{0};

    //clasqaDB
    bool _applyQA{false};
    vector<std::string> _reqsQA;
    bool _reqOKAsymmetry{false};
    bool passQAReqs();
    qadb_reader * _qa;
    
    ///////////////////////////////RCDB
   private:

    rcdb_vals _rcdbVals;

  public:

    const rcdb_vals& getRcdbVals(){return _rcdbVals;}
    void setRcdbVals(const rcdb_vals& vals){_rcdbVals=vals;}
    
  private:
 ///////////////////////////////

   };
  //helper functions
  
  //filter vectors via a lambda function
  //see for example clas12reader::getByID()
  template <typename Cont, typename Pred>
    Cont container_filter(const Cont &container, Pred predicate){
    Cont result;
    std::copy_if(container.begin(),container.end(),std::back_inserter(result), predicate);
    return result;
  }

}

#endif /* CLAS12READER_H */
