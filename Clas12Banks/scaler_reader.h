#pragma once

#include <hipo4/reader.h>
#include <hipo4/bank.h>
#include "event.h"
#include "scaler.h"
#include "runconfig.h"
#include <string>
#include <float.h>

namespace clas12 {

  using evch_pair = std::pair<int,float>;
  
  class scaler_reader{

  public:
    scaler_reader(std::string filename);

    double getBeamCharge() const noexcept{return _maxCup-_minCup;}
    
    size_t  addLongCounter();
    size_t  addDoubleCounter();
    size_t findPosition(size_t event);
    
    void incrementLong(size_t index,size_t event,int64_t val){_longCounter[index][findPosition(event)]+=val;}
    void incrementDouble(size_t index,size_t event,double val){_doubleCounter[index][findPosition(event)]+=val;}

    std::vector<int64_t>&  getLongCounter(size_t index){return _longCounter[index];}
    std::vector<double>&  getDoubleCounter(size_t index){return _doubleCounter[index];}
    std::vector<float>&  getDeltaCharges(){return _deltaCharges;}
    bool validCharge(size_t ev){ return validChargePos(findPosition(ev));}
    //requres each accumated scaler>0 and delta charge>0
    bool validChargePos(size_t pos){return pos>0 ?_beamCharges[pos].second>0&&_beamCharges[pos-1].second>0&&_deltaCharges[pos]>0 : false;}
    
  private:
 
    scaler_uptr _bscal;
    runconfig_uptr _brun;
    
    double _maxCup{0};
    double _minCup{DBL_MAX};
    std::vector<std::vector<double>> _doubleCounter{0};
    std::vector<evch_pair> _beamCharges{0};
    std::vector<std::vector<int64_t>> _longCounter{0};
    std::vector<float>_deltaCharges{0};
    long _nScalReads{0};
    size_t _lastPosition{0};
    size_t _lastEvent{0};
  };
  using scalerreader_ptr=clas12::scaler_reader*;
  using scalerreader_uptr=std::unique_ptr<clas12::scaler_reader>;

}
