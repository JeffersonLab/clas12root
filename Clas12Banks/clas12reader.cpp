/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "clas12reader.h"
#include "vtp.h"

namespace clas12 {

  clas12reader::clas12reader(std::string filename,std::vector<long> tags):
    _filename(filename){

    if(_verbose) cout<<" clas12reader::clas12reader reading "<<filename.data()<<endl;
    _reader.setTags(tags);
    
     
    if(_filename.empty()==false)initReader();
  }
  ///////////////////////////////////////////////////
  ///copy constructor
  ///opens a new reader
  ///Can give alternative filename
  clas12reader::clas12reader(const clas12reader &other,std::string filename,
			     std::vector<long> tags):
    _filename(filename)
    //_db{other._db}
  {
    
  
    //if default filename take same file as original
    if(_filename.empty())_filename=other._filename;
    if(other._scalReader.get()) scalerReader();
 
    _reader.setTags(tags);
    initReader();

    for(auto& bnk:other._bankInUse){//copy list of in use banks
      //give the value in other to this entry
      if(bnk.second!=nullptr)
	(*(_bankInUse.at(bnk.first)))=*(bnk.second);
    }
    
    _justParticleAna=other._justParticleAna;
    _givenPids=other._givenPids;
    _pidSelect=other._pidSelect;
    _pidSelectExact=other._pidSelectExact;
    _zeroOfRestPid=other._zeroOfRestPid;
    _useFTBased=other._useFTBased;
    _nToProcess=other._nToProcess;

    setVerbose(other._verbose);

    for(const auto& additionalBank : other._addBankNames)
      addBank(additionalBank);
	
    _applyQA=other._applyQA;
    _readEventUserAction=other._readEventUserAction;
  }

  void clas12reader::initReader(){
  
    _reader.open(_filename.data()); //keep a pointer to the reader
    _isOpen=true;
      // hipo::dictionary  factory;
    _reader.readDictionary(_factory);

    //initialise banks pointers
    if(_factory.hasSchema("RECFT::Particle"))
      _bftbparts.reset(new ftbparticle{_factory.getSchema("RECFT::Particle")});

    if(_factory.hasSchema("REC::Particle"))
      _bparts.reset(new particle{_factory.getSchema("REC::Particle"),_bftbparts.get()});

    if(_factory.hasSchema("REC::CovMat")){
      _bcovmat.reset(new covmatrix{_factory.getSchema("REC::CovMat")});
      _bankInUse["REC::CovMat"]=&_useCovmat;
      _useCovmat=true;
    }
    
    if(_factory.hasSchema("RECFT::Event")){
      _bftbevent.reset(new clas12::ftbevent{_factory.getSchema("RECFT::Event")});
      _bankInUse["RECFT::Event"]=&_useFTBEvent;
      _useFTBEvent=true;
    }
    
    if(_factory.hasSchema("RUN::config")){
      _brunconfig.reset(new clas12::runconfig{_factory.getSchema("RUN::config")});
      _bankInUse["RUN::config"]=&_useRunconfig;
      _useRunconfig=true;
    }
    
    if(_factory.hasSchema("REC::Event")){
      _bevent.reset(new clas12::event{_factory.getSchema("REC::Event"),_bftbevent.get()});
      _bankInUse["REC::Event"]=&_useEvent;
      _useEvent=true;
    }  

    if(_factory.hasSchema("REC::Calorimeter")){
      if(_factory.hasSchema("REC::CaloExtras")){
	_bcal.reset(new calorimeter{_factory.getSchema("REC::Calorimeter"),_factory.getSchema("REC::CaloExtras")});
	_bankInUse["REC::Calorimeter"]=&_useCal;
	_useCal=true;
	_bankInUse["REC::CaloExtras"]=&_useCalExtras;
	_useCalExtras=true;
 
      }
      else{
	_bcal.reset(new calorimeter{_factory.getSchema("REC::Calorimeter")});
	_bankInUse["REC::Calorimeter"]=&_useCal;
	_useCal=true;
      }
    }
    
    if(_factory.hasSchema("REC::Scintillator")){
      if(_factory.hasSchema("REC::ScintExtras")){
	_bscint.reset(new scintillator{_factory.getSchema("REC::Scintillator"),_factory.getSchema("REC::ScintExtras")});
	_bankInUse["REC::Scintillator"]=&_useScint;
	_useScint=true;
	_bankInUse["REC::ScintExtras"]=&_useScintExtras;
	_useScintExtras=true;
	
      }
      else{
	_bscint.reset(new scintillator{_factory.getSchema("REC::Scintillator")});
	_bankInUse["REC::Scintillator"]=&_useScint;
	_useScint=true;
	
      }
    }
    
    if(_factory.hasSchema("REC::Track")){
      _btrck.reset(new tracker{_factory.getSchema("REC::Track")});
      _bankInUse["REC::Track"]=&_useTrck;
      _useTrck=true;
    }
    
    if(_factory.hasSchema("REC::UTrack")){
      _butrck.reset(new utracker{_factory.getSchema("REC::UTrack")});
      _bankInUse["REC::UTrack"]=&_useUTrck;
      _useUTrck=true;
    }
    
    if(_factory.hasSchema("REC::Traj")){
      _btraj.reset(new traj{_factory.getSchema("REC::Traj")});
      _bankInUse["REC::Traj"]=&_useTraj;
      _useTraj=true;
    }
    
    if(_factory.hasSchema("REC::Cherenkov")){
      _bcher.reset(new cherenkov{_factory.getSchema("REC::Cherenkov")});
      _bankInUse["REC::Cherenkov"]=&_useCher;
      _useCher=true;
    }
    
    if(_factory.hasSchema("RICH::Particle")){
      _brich.reset(new rich{_factory.getSchema("RICH::Particle"),_factory.getSchema("RICH::Ring")});
      _bankInUse["RICH::Particle"]=&_useRich;
      _useRich=true;
    }
    
    if(_factory.hasSchema("REC::ForwardTagger")){
      _bft.reset(new forwardtagger{_factory.getSchema("REC::ForwardTagger")});
      _bankInUse["REC::ForwardTagger"]=&_useFT;
      _useFT=true;
    }
    
    if(_factory.hasSchema("REC::VertDoca")){
      _bvertdoca.reset(new clas12::vertdoca{_factory.getSchema("REC::VertDoca")});
      _bankInUse["REC::VertDoca"]=&_useVertdoca;
      _useVertdoca=true;
    }
  
    if(_factory.hasSchema("MC::Lund")){
      if(_factory.hasSchema("MC::RecMatch")){
	_bmcparts.reset( new mcparticle{_factory.getSchema("MC::Lund"),_factory.getSchema("MC::RecMatch")});
      }
      else{
	_bmcparts.reset( new mcparticle{_factory.getSchema("MC::Lund")});
      }
      _bankInUse["MC::Lund"]=&_useMCparts;
      _useMCparts=true;

    }

    if(_factory.hasSchema("MC::Event")){
      _bmcevent.reset( new clas12::mcevent{_factory.getSchema("MC::Event")});
      _bankInUse["MC::Event"]=&_useMCevent;
      _useMCevent=true;
    }

    //could remove below
    if(_factory.hasSchema("RAW::vtp")){
      _bvtp.reset(new clas12::vtp{_factory.getSchema("RAW::vtp")});
      _bankInUse["RAW::vtp"]=&_useVTP;
      _useVTP=true;

    } 
    
    if(_factory.hasSchema("HEL::online")){
      _bhelonline.reset(new clas12::helonline{_factory.getSchema("HEL::online")});
      _bankInUse["HEL::online"]=&_useHelonline;
      _useHelonline=true;

    }

    makeListBanks();

  }

  ///////////////////////////////////////////////////////////////////////
  ///Basically get the run number!
  ///will open and close a hipo file
  int  clas12reader::readQuickRunConfig(const std::string& filename) {


    //There are some inconsistencies on which the RunConfig tag is...
    //So just in case...
    int rnb=tryTaggRunConfig( filename,1);
    if(rnb==0) return tryTaggRunConfig( filename,0);
    else return rnb;

  }
     
  int  clas12reader::tryTaggRunConfig(const std::string& filename, int tag) {
   if(filename.empty()==true) return 0;
    
    hipo::reader     areader;
    hipo::event      anevent;
    hipo::dictionary  afactory;
    
    areader.setTags(tag);
    areader.open(filename.data()); //keep a pointer to the reader
    areader.readDictionary(afactory);
    
    clas12::runconfig  arunconf(afactory.getSchema("RUN::config"));

    int runNo=0;
    while(runNo==0){
      if(areader.next()==false) break;
      areader.read(anevent);
      anevent.getStructure(arunconf);

      runNo=arunconf.getRun();
    }
    //std::cout<<"Found run number : "<<runNo<<"  in tag "<<tag<<std::endl;
    return runNo;
  }
  ///////////////////////////////////////////////////////////////////////
  ///Function to query RCDB and record most relevant run conditions.
  ///This is only called once to avoid overloading the database.
  /*void clas12reader::queryRcdb(){
    if(_rcdbQueried==true) return; //only allowed to call once
    _rcdbQueried=true;

#ifdef RCDB_MYSQL
    if(_runNo==0){
      _runNo=readQuickRunConfig(_filename);
    }
    
    rcdb_reader rc("mysql://rcdb@clasdb.jlab.org/rcdb"); //initialise rcdb_reader
    
    //For full list see https://clasweb.jlab.org/rcdb/conditions/
    _rcdbVals = rc.readAll(_runNo,getFilename());
 #endif
    //rcdb connection closed when rc goes out of scope here 
  }
  */
  
  ///////////////////////////////////////////////////////
  ///read the data
  void clas12reader::clearEvent(){
    //clear previous event
    _n_rfdets=0;
    _n_rcdets=0;
    _n_rfts=0;
    _n_rbands=0;
    
    _detParticles.clear();
    _pids.clear();
 
    _isRead=false;
  }

  ///////////////////////////////////////////////////////////////
  ///This function may be called before readEvent to allow checking
  ///of Pids by external routines
   const std::vector<short>& clas12reader::preCheckPids(){
    //regular particle bank
     if(_isRead) return _pids; //already read return current pids
     hipoRead();
  
     _event.getStructure(*_bparts.get());
    if(_bftbparts.get())_event.getStructure(*_bftbparts.get());//FT based PID particle bank
   
    //First check if event passes criteria
    _nparts=_bparts->getRows();
    _pids.clear();
    _pids.reserve(_nparts);
 
    //Loop over particles and find their Pid
    for(ushort i=0;i<_nparts;i++){
      if(!_useFTBased){
	_bparts->setEntry(i);
	_pids.emplace_back(_bparts->getPid());
      }
      else{
	if(_bftbparts->getRows()){
	  _bftbparts->setEntry(i);
	  _pids.emplace_back(_bftbparts->getPid());
	}
	else{//if not ftbased use FD based
	  _bparts->setEntry(i);
	  _pids.emplace_back(_bparts->getPid());
	}
      }
	
    }
    return _pids;
  }
  const std::vector<short>& clas12reader::preCheckPidsOrCharge(){
    //Required for EventBuilder Pid==0, which may have any charge
    //give these particles Pid = UndefPDG*charge
 
    //regular particle bank
     if(_isRead) return _pids; //already read return current pids
     hipoRead();
  
     _event.getStructure(*_bparts.get());
    if(_bftbparts.get())_event.getStructure(*_bftbparts.get());//FT based PID particle bank
   
    //First check if event passes criteria
    _nparts=_bparts->getRows();
    _pids.clear();
    _pids.reserve(_nparts);
 
    //Loop over particles and find their Pid
    for(ushort i=0;i<_nparts;i++){
      if(!_useFTBased){
	_bparts->setEntry(i);
	
	auto pidch=_bparts->getPid();
	if(pidch==0) //no PID use charge instead
	  pidch=_bparts->getCharge()*UndefPDG;
	_pids.emplace_back(pidch);
      }
      else{
	if(_bftbparts->getRows()!=0){
	  _bparts->setEntry(i);
	  _bftbparts->setEntry(i);
	  
	  auto pidch=_bftbparts->getPid();
	  if(pidch==0) //no PID use charge instead
	    pidch=_bparts->getCharge()*UndefPDG;
	 
	  _pids.emplace_back(pidch);
 	}
	else{//if not ftbased use FD based
	  _bparts->setEntry(i);
	  
	  auto pidch=_bparts->getPid();
	  if(pidch==0) //no PID use charge instead
	    pidch=_bparts->getCharge()*UndefPDG;
	  _pids.emplace_back(pidch);
	}
      }
	
    }
    return _pids;
  }

  bool clas12reader::checkQA(){

    //check if event has QA requirements and those were met
    if(_db!=nullptr){
      if(_applyQA&&_db->qa()!=nullptr){
	if(!_db->qa()->passQAReqs(_brunconfig->getEvent())){
	  return false;
	}
      }
      else   if(_db->qa()!=nullptr){
      	//accumulate charge even if no conditions given
      	_db->qa()->accumulateCharge(_brunconfig->getEvent());
      }
    }
    return true;
  }
  bool clas12reader::justCheckQA(){
    if(_brunconfig.get())_event.getStructure(*_brunconfig.get());
    if(checkQA()==false) return false;
    return true;
  }
  // double clas12reader::sumChargeFromQA(){
  //   while(_reader.next()){
  //     if(_nevent==_nToProcess){
  // 	summary();
  // 	return false; //reached supplied event limit
  //     }
  //     hipoRead();
  //     ++_nevent;
  //     if(justCheckQA())
  // 	++_nselected;
  //   }
  //   std::cout<<" clas12reader::sumChargeFromQA() "<<db()->qa()->getAccCharge()<<" from "<<_nselected <<" events out of "<<_nevent<<endl;
  //   return db()->qa()->getAccCharge();
  // }
  
  bool clas12reader::readEvent(){
  
    //First get pid of tracks and save in _pids
    //also responsible for calling hiporead()
    preCheckPids();

    //Second check qa
    //Special run banks
    if(_useRunconfig==true)_event.getStructure(*_brunconfig.get());
    //Keep track of run which have been processed
    auto currRun=_brunconfig->getRun();
    if(currRun!=_runNo){
      if(currRun!=0 )_runNumbers.insert(currRun);
      _runNo=currRun;
    }
    
    if(checkQA()==false) return false;
   
     //Third check if event is of the right type
    if(!passPidSelect()){
      _pids.clear(); //reset so read next event in preChekPids
      return false;
    }

    //Now event OK, read all the data
    //now getthe data for the rest of the banks
    if(_useCovmat==true)_event.getStructure(*_bcovmat.get());
    if(_useEvent==true)_event.getStructure(*_bevent.get());
    if(_useFTBEvent==true)_event.getStructure(*_bftbevent.get());
    if(_useCal==true)_event.getStructure(*_bcal.get());
    if(_useCalExtras==true)_event.getStructure(*_bcal->getExtras());
    if(_useScint==true)_event.getStructure(*_bscint.get());
    if(_useScintExtras==true)_event.getStructure(*_bscint->getExtras());
    if(_useTrck==true)_event.getStructure(*_btrck.get());
    if(_useUTrck==true)_event.getStructure(*_butrck.get());
    if(_useTraj==true)_event.getStructure(*_btraj.get());
    if(_useCher==true)_event.getStructure(*_bcher.get());
    if(_useRich==true){
      _event.getStructure(*_brich.get());
      _event.getStructure(*_brich->ring());
    }
    if(_useFT==true)_event.getStructure(*_bft.get());
    if(_useVTP==true)_event.getStructure(*_bvtp.get());
    if(_useHelonline==true)_event.getStructure(*_bhelonline.get());
    if(_useVertdoca==true)_event.getStructure(*_bvertdoca.get());

    if(_useMCparts==true){
      _event.getStructure(*_bmcparts.get());
      if(_bmcevent.get())_event.getStructure(*_bmcevent.get());
      if(_bmcparts->getMatch())_event.getStructure(*_bmcparts->getMatch());
    }
    
    for(auto& ibank:_addBanks){//if any additional banks requested get those
      _event.getStructure(*ibank.get());
    }
   
    // now that we have read all the banks, call user's custom read action;
    // return its return value, since it's the last thing `readEvent()` does
    return _readEventUserAction(this);
  }
  ////////////////////////////////////////////////////////
  ///initialise next event from the reader
  bool clas12reader::next(){

    //keep going until we get an event that passes
    bool validEvent=false;
    while(_reader.next()){
      if(_nevent==_nToProcess){
	summary();
	return false; //reached supplied event limit
      }
      
      clearEvent();
      _nevent++;
      if(readEvent()){ //got one
	validEvent=true;
	_nselected++;
	break;
      }
    }
    if(!validEvent){
      summary();
      return false;//no more events in reader
    }
    //can proceed with valid event
    sort();
  
    return true;
  }
  ////////////////////////////////////////////////////////
  ///initialise next event from the reader
  bool clas12reader::nextInRecord(){
     
    //keep going until we get an event that passes
    bool validEvent=false;
    while(_reader.nextInRecord()){
      clearEvent();
      _nevent++;
      if(readEvent()){ //got one
	validEvent=true;
	break;
      }
    }
    if(!validEvent) return false;//no more events in record

    //can proceed with valid event
    sort();
 
    return true;
  }
  ////////////////////////////////////////////////////////
  /// Loop over particles and find their region
  /// Add appropriate region_partcle to event particle vector
  void clas12reader::sort(){

    if(_nparts==0) return;
   
    _n_rfdets=0;
    _n_rcdets=0;
    _n_rfts=0;
    _n_rbands=0;

    _detParticles.clear();
    _detParticles.reserve(_nparts);

    //Loop over particles and find their region
    for(ushort i=0;i<_nparts;i++){ 
      _bparts->setEntry(i);
     
      if(_rfdets.empty()) addARegionFDet();
      if(_justParticleAna){
	_rfdets[_n_rfdets]->region_particle::sort();
	// Just analysing REC::Particle, Use FD for convenience
	_detParticles.emplace_back(_rfdets[_n_rfdets].get());
	_n_rfdets++;
	//check if need more vector entries
	//only required of previous events have
	//less particles than this
	if(_n_rfdets==_rfdets.size())
	  addARegionFDet();
	
	continue;
      }
      //Check if FDet particle
       if(_rfdets[_n_rfdets]->sort()){
	//	add a FDet particle to the event list
	_detParticles.emplace_back(_rfdets[_n_rfdets].get());
	_n_rfdets++;
	//check if need more vector entries
	//only required of previous events have
	//less particles than this
	if(_n_rfdets==_rfdets.size())
	  addARegionFDet();

	continue;
      }
      
      //Check if CDet particle
      if(_rcdets.empty()) addARegionCDet();
      if(_rcdets[_n_rcdets]->sort()){
	//	add a FDet particle to the event list
	_detParticles.emplace_back(_rcdets[_n_rcdets].get());
	_n_rcdets++;
	//check if need more vector entries
	//only required of previous events have
	//less particles than this
	if(_n_rcdets==_rcdets.size())
	  addARegionCDet();

	continue;
      }
	 
       //Check if FT particle
      if(_rfts.empty())addARegionFT();
      if(_rfts[_n_rfts]->sort()){
	//add a FDet particle to the event list
	_detParticles.emplace_back(_rfts[_n_rfts].get());
	_n_rfts++;
	//check if need more vector entries
	//only required of previous events have
	//less particles than this
	if(_n_rfts==_rfts.size())
	  addARegionFT();

	continue;
      }
      //Check if BAND particle
      if(_rbands.empty())addARegionBAND();
      if(_rbands[_n_rbands]->sort()){
	//add a FDet particle to the event list
	_detParticles.emplace_back(_rbands[_n_rbands].get());
	_n_rbands++;
	//check if need more vector entries
	//only required of previous events have
	//less particles than this
	if(_n_rbands==_rbands.size())
	  addARegionBAND();
	continue;
      }
    }
 
 
  }
  bool clas12reader::passPidSelect(){
    //if no selections take event
    if(_pidSelect.empty()&&_pidSelectExact.empty()) return true;

    //check is there is at least enough particles
    if(_pidSelectExact.size()+_pidSelect.size()>_nparts)
      return false;

    //check if any unwanted particles
    if(_zeroOfRestPid){
      auto uniquePids=_pids;//make a copy
      std::sort(uniquePids.begin(), uniquePids.end());
      auto ip = std::unique(uniquePids.begin(), uniquePids.begin() + _nparts); 
      uniquePids.resize(std::distance(uniquePids.begin(), ip));
      //now just loop over the unique particle types
      for(auto const& pid : uniquePids){
	//check if we have a PID not given in a selection
	if(!(std::count(_givenPids.begin(),_givenPids.end(), pid)))
	  return false;
      }
    }
  
    //check for requested exact matches
    for(auto const& select : _pidSelectExact){
      if(!(select.second==getNPid(select.first)))
	return false;
    }
    
    //check for requeseted at least  matches
    for(auto const& select : _pidSelect){
      if((select.second>getNPid(select.first)))
	return false;
    }
    return true;
  }
  
   ////////////////////////////////////////////////////////
  ///Filter and return detParticles

  std::vector<region_part_ptr> clas12reader::getDetParticles(bool const& applyBankFilter) const
  {
    return applyBankFilter ?
      container_filter(_detParticles, [](region_part_ptr dr) { return dr->isAllowed(); }) :
      _detParticles;
  }

  std::vector<region_part_ptr> clas12reader::getByID(int id, bool const& applyBankFilter) const
  {
    return applyBankFilter ?
      container_filter(_detParticles, [&id](region_part_ptr dr) { return dr->getPid()==id && dr->isAllowed(); }) :
      container_filter(_detParticles, [&id](region_part_ptr dr) { return dr->getPid()==id; });
  }

  std::vector<region_part_ptr> clas12reader::getByRegion(int ir, bool const& applyBankFilter) const
  {
    return applyBankFilter ?
      container_filter(_detParticles, [&ir](region_part_ptr dr) { return dr->getRegion()==ir && dr->isAllowed(); }) :
      container_filter(_detParticles, [&ir](region_part_ptr dr) { return dr->getRegion()==ir; });
  }

  std::vector<region_part_ptr> clas12reader::getByCharge(int ch, bool const& applyBankFilter) const
  {
    return applyBankFilter ?
      container_filter(_detParticles, [&ch](region_part_ptr dr) { return dr->par()->getCharge()==ch && dr->isAllowed(); }) :
      container_filter(_detParticles, [&ch](region_part_ptr dr) { return dr->par()->getCharge()==ch; });
  }

  ////////////////////////////////////////////////////////////////
  ///Enable QA skimming.
  /*void clas12reader::applyQA(std::string jsonFilePath){
#ifdef CLAS_QADB
    //_runNo may already have been found
    if(_runNo==0){
      _runNo=readQuickRunConfig(_filename);
    }
    _qa.reset(new qadb_reader(jsonFilePath, _runNo));
#endif
}*/

  //////////////////////////////////////////////////////////////
  ///Returns qadb_reader once declared
  /*#ifdef CLAS_QADB
  qadb_reader * clas12reader::getQAReader(){
    return _qa.get();
  }
#endif
  */
  ////////////////////////////////////////////////////////////
  ///connect to the data bases
  void clas12reader::connectDataBases(clas12databases* db){
    _db=db;

    _connectDB=true;

    auto runNumber =readQuickRunConfig(_filename);
    _runNo = runNumber;
    if(_verbose )std::cout<<"Connecting databases to run "<<runNumber<<std::endl;
    //Do not need to notify if run number has not changed
    if(_runNo!=0)_db->notifyRun(runNumber);
  }
  /////////////////////////////////////////////////////////
  ///make a list of banks, required for writer
  void clas12reader::makeListBanks(){
    _allBanks.clear();
    //  cout<<"clas12reader::makeListBanks() "<<_addBanks.size()<<endl;
    //If any non standard DST banks added include them
    //for(auto& bnk:_addBanks){
    // cout<<" add bank "<<bnk->getSchema().getName()<<endl;
    //  _allBanks.push_back(bnk.get());
    //}
    //Now standard DST banks
    if(_brunconfig.get())_allBanks.push_back(_brunconfig.get());
    if(_bparts.get())_allBanks.push_back(_bparts.get());
    if(_bftbparts.get())_allBanks.push_back(_bftbparts.get());
    if(_bmcparts.get()){
      _allBanks.push_back(_bmcparts.get());
      if(_bmcparts->getMatch())_allBanks.push_back(_bmcparts->getMatch());     
    }
    if(_bmcevent.get())_allBanks.push_back(_bmcevent.get());
    if(_bcovmat.get())_allBanks.push_back(_bcovmat.get());
    if(_bevent.get())_allBanks.push_back(_bevent.get());
    if(_bftbevent.get())_allBanks.push_back(_bftbevent.get());
    if(_bcal.get())_allBanks.push_back(_bcal.get());
    if(_bscint.get()){
      _allBanks.push_back(_bscint.get());
      if(_bscint->getExtras())_allBanks.push_back(_bscint->getExtras());     
    }
    if(_btrck.get())_allBanks.push_back(_btrck.get());
    if(_butrck.get())_allBanks.push_back(_butrck.get());
    if(_btraj.get())_allBanks.push_back(_btraj.get());
    if(_bcher.get())_allBanks.push_back(_bcher.get());
    if(_brich.get()){
      _allBanks.push_back(_brich.get());
      _allBanks.push_back(_brich->ring());
   }
    if(_bft.get())_allBanks.push_back(_bft.get());
    if(_bvtp.get())_allBanks.push_back(_bvtp.get());
    if(_bvertdoca.get())_allBanks.push_back(_bvertdoca.get());
  

  }
  
  bool clas12reader::grabEvent(Long64_t Nev){
    if(_isOpen==false){
      cout<<"clas12reader::grabEvent "<<" reader not open"<<endl;
      return false;
    }
    //clear event, so can read next
    clearEvent();
    //move to Nev via hipo::reader
    if(getReader().gotoEvent(Nev)==false){
      return false; //outwith event range
    }
    //read full event
    readEvent();
    sort();
    return true;
  }

}
