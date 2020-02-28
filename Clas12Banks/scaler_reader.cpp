#include "scaler_reader.h"
#include <algorithm>

namespace clas12{
  
  scaler_reader::scaler_reader(std::string filename){
    if(filename.empty())return;
    hipo::reader     reader_;
    hipo::event      event_;
    hipo::dictionary  factory_;

    reader_.setTags(1);
    reader_.open(filename.data()); //keep a pointer to the reader
    reader_.readDictionary(factory_);
    
    _bscal.reset(new scaler{factory_.getSchema("RUN::scaler")});
    _brun.reset(new runconfig{factory_.getSchema("RUN::config")});
 
    while(reader_.next()){
      reader_.read(event_);
      event_.getStructure(*_bscal.get());
      if(_bscal->getRows()==0)continue; //only RUN::scaler
      event_.getStructure(*_brun.get());

      _nScalReads++;
      auto cup=_bscal->getFCupGated();
      if(cup>_maxCup) _maxCup=cup;
      if(cup<_minCup) _minCup=cup;
        _beamCharges.push_back(std::make_pair(_brun->getEvent(),cup));
   }
    std::sort(_beamCharges.begin(),_beamCharges.end());

    // int ccc=0;
    _deltaCharges.resize(_nScalReads);
    for(int i=0;i<_nScalReads;i++){
      if(i)_deltaCharges[i]=_beamCharges[i].second-_beamCharges[i-1].second;
      else _deltaCharges[i]=0;
    }
    std::cout<<"Number of scaler reads "<<_nScalReads<<" "<<getBeamCharge()<<std::endl;
  }

  size_t  scaler_reader::addLongCounter(){
    auto index=_longCounter.size();
    std::vector<int64_t> vec(_nScalReads);
    _longCounter.push_back(std::move(vec));
    return index;
  }
  size_t  scaler_reader::addDoubleCounter(){
    auto index=_doubleCounter.size();
    std::vector<double> vec(_nScalReads);
   _doubleCounter.push_back(std::move(vec));
    return index;
  }

  size_t scaler_reader::findPosition(size_t event){

    if(event==_lastEvent) return _lastPosition;
    _lastEvent=event;//new event

    if(_lastPosition>0){
      //beamCharges is sorted in event number
      if(event> _beamCharges[_lastPosition-1].first
	 && event<_beamCharges[_lastPosition].first)
	return _lastPosition; //still in the same scaler read
    }
   
    
    auto itr=std::upper_bound(_beamCharges.begin(),_beamCharges.end(),
			      evch_pair(event,0),
			      [](const evch_pair& entry,const evch_pair& rhs){
				return entry.first<rhs.first;
			      }
			      );
    _lastPosition = std::distance(_beamCharges.begin(),itr);

    //For standard events this should not happen, may happen with tag!=0
    if(_lastPosition>=_nScalReads) _lastPosition=_nScalReads-1;
    return  _lastPosition;
  }
  
}
