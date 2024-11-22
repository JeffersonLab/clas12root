/*
 * File:   clas12reader.h
 * Author: dglazier
 *
 */

#ifndef CLAS12READER_H
#define CLAS12READER_H

#include "clas12defs.h"
#include "clas12databases.h"
#include <hipo4/reader.h>
#include "particle.h"
#include "ftbparticle.h"
#include "mcparticle.h"
#include "mcevent.h"
#include "calorimeter.h"
#include "scintillator.h"
#include "tracker.h"
#include "utracker.h"
#include "traj.h"
#include "cherenkov.h"
#include "event.h"
#include "runconfig.h"
#include "ftbevent.h"
#include "vtp.h"
#include "vertdoca.h"
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
#include <hipo4/dictionary.h>

#include <algorithm>
#include <set>
#include <vector>
#include <string>
#include <iostream>

#ifdef RCDB_MYSQL
   #include "rcdb_reader.h"
#endif

//#ifdef CLAS_QADB
   #include "qadb_reader.h"
//#endif

namespace clas12 {
  using std::cout;
  using std::endl;
  using std::cerr;

  class clas12reader  {


  public:


    clas12reader()=default;
    clas12reader(std::string filename,std::vector<long> tags=std::vector<long>());
    clas12reader(const clas12reader &other,std::string filename="",std::vector<long> tags=std::vector<long>());
    
    virtual ~clas12reader()=default;

    hipo::reader& getReader(){
      if(_isOpen==false){
	cerr<<"hipo::reader& getReader() clas12reader must be created with valid filename, currently we have  "<<getFilename()<<endl;
      }
      return _reader;
    }
    
    bool next();
    bool nextInRecord();
    void sort();
    bool readEvent();
    void clearEvent();
    
    std::vector<hipo::bank* > getAllBanksPtrs(){return _allBanks;}
    hipo::dictionary& getDictionary(){return _factory;}
    std::string getFilename(){return _filename;}
    
    void addARegionFDet(){
      //Forward detector needs particles, calorimeter, scintillator,
      //track, cherenkov
      region_fdet_uptr  reg{new region_fdet{_bparts.get(),_bftbparts.get(),_bcovmat.get(),_bcal.get(),_bscint.get(),_btrck.get(),_butrck.get(),_btraj.get(),_bcher.get(),_bft.get(),_bevent.get(),_brich.get(),_bmcparts.get()}};
      if(_useFTBased)reg->useFTBPid();
     _rfdets.push_back(std::move(reg));
    }
     void addARegionCDet(){
      //Forward detector needs particles, calorimeter, scintillator,
      //track, cherenkov
       region_cdet_uptr  reg{new region_cdet{_bparts.get(),_bftbparts.get(),_bcovmat.get(),_bcal.get(),_bscint.get(),_btrck.get(),_butrck.get(),_btraj.get(),_bcher.get(),_bft.get(),_bevent.get(),_brich.get(),_bmcparts.get()}};
      if(_useFTBased)reg->useFTBPid();
      _rcdets.push_back(std::move(reg));
    }
    void addARegionFT(){
      //Forward tagger needs particles and forward tagger
       region_ft_uptr  reg{new region_ft{_bparts.get(),_bftbparts.get(),_bcovmat.get(),_bcal.get(),_bscint.get(),_btrck.get(),_butrck.get(),_btraj.get(),_bcher.get(),_bft.get(),_bevent.get(),_brich.get(),_bmcparts.get()}};
       if(_useFTBased)reg->useFTBPid();
      _rfts.push_back(std::move(reg));
     }
  void addARegionBAND(){
      //Forward tagger needs particles and forward tagger
       region_band_uptr  reg{new region_band{_bparts.get(),_bftbparts.get(),_bcovmat.get(),_bcal.get(),_bscint.get(),_btrck.get(),_butrck.get(),_btraj.get(),_bcher.get(),_bft.get(),_bevent.get(),_brich.get(),_bmcparts.get()}};
       if(_useFTBased)reg->useFTBPid();
      _rbands.push_back(std::move(reg));
     }


    helonline_ptr helonline() const{return _bhelonline.get();};
    helflip_ptr helflip() const{return _bhelflip.get();};
    runconfig_ptr runconfig() const{return _brunconfig.get();};
    event_ptr event() const{return _bevent.get();};
    ftbevent_ptr ftbevent() const{return _bftbevent.get();};
    vtp_ptr vtp() const{return _bvtp.get();};
    vertdoca_ptr vertdoca() const{return _bvertdoca.get();};
 
    
    mcpar_ptr mcparts() const{return _bmcparts.get();};
    mcevt_ptr mcevent() const{return _bmcevent.get();};


    //support for generic non-DST banks
    uint addBank(const std::string& name){
      if(isOpen()==false){
	cerr<<"clas12reader::addBank reader not opened, exiting..."<<endl;
	cerr<<"  in case using HipoChain, call NextFile() first"<<endl;
	exit(0);
      }
      std::unique_ptr<hipo::bank> bnk{new hipo::bank{_factory.getSchema(name.data())}};
      _addBanks.push_back(std::move(bnk));
      _allBanks.push_back(_addBanks.back().get());
      _addBankNames.push_back(name);
      return _addBanks.size()-1; //return place in vector
    }

    hipo::bank* getBank(uint index) const {return  _addBanks.at(index).get();}
    int getBankOrder(int ibank,std::string itemname ) const{
      return getBank(ibank)->getSchema().getEntryOrder(itemname.data());
    }

    const std::vector<hipo::bank*> getBanksPtrs(std::vector<std::string> names) const{
      std::vector<hipo::bank*> blist;
      for(const auto& name:names){
	//Find this bank in all banks
	auto it = std::find_if(
			       _allBanks.begin(),
			       _allBanks.end(),
			       [&name](auto* bank)
			       { return bank->getSchema().getName() == name; });
	if(it == _allBanks.end()) {
	  cerr<<"clas12reader::getBanks : bank "<<name<<" does not exist"<<endl;
	  exit(0);
	}
	//make a copy and add to list
	auto cpBank = dynamic_cast<hipo::bank*>(*it);
	blist.push_back(cpBank);
	
      }
      cout<<"clas12reader::getBanks  got "<<blist.size()<<endl;
      return blist;
    }
 
    /////////////////////////////////
    
    std::vector<region_part_ptr>& getDetParticles(){return _detParticles;}
    std::vector<region_part_ptr>* getDetParticlesPtr(){return &_detParticles;}
    std::vector<region_part_ptr> getByID(int id);
    std::vector<region_part_ptr> getByRegion(int ir);
    std::vector<region_part_ptr> getByCharge(int ch);

    const std::vector<short>& preCheckPids();
    const std::vector<short>& preCheckPidsOrCharge();

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
    double getRunBeamCharge() {
      if(_db!=nullptr)
	if(db()->qa()!=nullptr) return db()->qa()->getAccCharge();
      return _runBeamCharge;
    }

    int getRunNumber()const {return _runNo;}//works if connectDatabases called
    std::set<int> getRunNumbers() const {return _runNumbers;}
    
    double getCurrApproxCharge(){return _runBeamCharge*_nevent/_reader.getEntries();}

    void summary(){
      std::cout<<"for file "<<_filename<<"\n\t read "<<_nevent<<" events from which "<<_nselected<< " passed filtering conditions."<<std::endl;
      if(_db!=nullptr)
	if(db()->qa()){
	  auto prev = db()->qa()->getPreviousCharge();
	  //Just charge for this file
	  //  double charge=db()->qa()->getChargeForRunlist(getRunNumbers()) - prev;
	  double charge=db()->qa()->getAccCharge();
	  db()->qa()->setPreviousCharge(charge);
	  charge-= prev;
	  cout<<"\t accumulated charge past QA: "<<charge<<" nC"<<endl;
	}
    }
    
    void getStructure(hipo::bank* bank){
      if(_isRead==false)
	hipoRead();
      _event.getStructure(*bank);
    }
    bool grabEvent(Long64_t Nev);

    //rcdb
    static int readQuickRunConfig(const std::string& filename);
    static int tryTaggRunConfig(const std::string& filename, int tag);
    

    bool isOpen(){return _isOpen;}
    
    void setEntries(long n){_nToProcess = n;}
    void setVerbose(short level=1){
      _verbose=level;
      _reader.setVerbose(level);
    }
    
  protected:

    void initReader();
    
    
    private:

    void makeListBanks(); //calls clear, so only do this in here
  
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
    covmat_uptr _bcovmat;//!
    cal_uptr  _bcal;//!
    scint_uptr _bscint;//!
    trck_uptr _btrck;//!
    utrck_uptr _butrck;//!
    traj_uptr _btraj;//!
    cher_uptr _bcher;//!
    rich_uptr _brich;//!
    ft_uptr _bft;//!
    vtp_uptr _bvtp;//!
    vertdoca_uptr _bvertdoca;//!

    
    mcpar_uptr _bmcparts;//!
    mcevt_uptr _bmcevent;//!

    
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

    ushort _verbose{0};
    
    std::vector<short> _pids;
    bool _isRead{false};
    //bool _rcdbQueried=false;
    
    //members that need copied in constructor
    scalerreader_uptr _scalReader;
    std::vector<short> _givenPids;
    std::map<short,short> _pidSelect;
    std::map<short,short> _pidSelectExact;
    bool _zeroOfRestPid{false};
    bool _useFTBased{false};
    bool _isOpen{false};
    std::vector<std::string> _addBankNames;
     
    ///////////////////////////////DB
  private:
    int _runNo{0};
    std::set<int> _runNumbers;
    clas12databases *_db={nullptr}; //

    bool _applyQA=false;
    bool _connectDB=false;
    bool checkQA();
    bool justCheckQA();

  public:

    //Database stuff
    void connectDataBases(clas12databases *db);
    //void connectDataBases();

    ccdb_reader* ccdb()const {return _db->cc();}
    rcdb_reader* rcdb()const {return _db->rc();}
    qadb_reader* qadb()const {return _db->qa();}

    clas12databases* db(){return _db;};
    
    //clas12-qadb   
    void applyQA(const string& pass) {
      _applyQA=true;
      _db->setPass(pass);
    }
 
    //double sumChargeFromQA();
    void ignoreBank(const string& bnk ){
      if(_bankInUse.at(bnk))
	(*_bankInUse[bnk])=false;
    }

    void SetRecParticleOnly(){
      _justParticleAna=true;
       ignoreBank("REC::CovMat");
       ignoreBank("REC::Traj");
       ignoreBank("REC::Calorimeter");
       ignoreBank("REC::Scintillator");
       ignoreBank("REC::CaloExtras");
       ignoreBank("REC::ScintExtras");
       ignoreBank("REC::Track");
       ignoreBank("REC::UTrack");
       ignoreBank("REC::ForwardTagger");
       ignoreBank("REC::VertDoca");
       ignoreBank("REC::Cherenkov");
       ignoreBank("RICH::Particle");
       ignoreBank("HEL::online");
       ignoreBank("RAW::vtp");
  
    }
  private:

    std::map<string,bool*> _bankInUse;
    bool _useCovmat={false};
    bool _useEvent={false};
    bool _useRunconfig={false};
    bool _useFTBEvent={false};
    bool _useCal={false};
    bool _useCalExtras={false};
    bool _useScint={false};
    bool _useScintExtras={false};
    bool _useTrck={false};
    bool _useUTrck={false};
    bool _useTraj={false};
    bool _useCher={false};
    bool _useRich={false};
    bool _useFT={false};
    bool _useVTP={false};
    bool _useHelonline={false};
    bool _useVertdoca={false};
    bool _useMCparts={false};
    bool _useMCevent={false};
    bool _justParticleAna={false};
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
