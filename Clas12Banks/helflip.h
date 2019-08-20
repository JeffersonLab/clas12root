/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   event.h
 * Author: dglazier
 *
 */

#ifndef CLAS12_HELFLIP_H
#define CLAS12_HELFLIP_H

#include <iostream>
#include <vector>
#include <memory>
#include "bank.h"
#include "runconfig.h"

namespace clas12 {

  class helflip : public hipo::bank {


  public:

 
    helflip(hipo::schema __schema,runconfig_ptr  runconfig);
    virtual ~helflip() = default;
 
    int    getHelicity(){std::cout<<getByte(_hel_order,0)<<std::endl;  return getByte(_hel_order,0); }
    int    getHelicityRaw(){ return getByte(_helr_order,0); }
    int    getRun(){ return getInt(_run_order,0); }
    int    getEvent(){ return getInt(_event_order,0); }
    long    getTimeStamp(){ return getLong(_timestamp_order,0); }
    int    getPair(){ return getByte(_pair_order,0); }
    int    getPattern(){ return getByte(_pattern_order,0); }
    int    getStatus(){ return getByte(_status_order,0); }

    int getCurrHelicity(){ return _currHelicity;}
    void helicityAnalysis();
    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * particles and or map particles by pid or type (i.e. charge)
    */
    void notify() final{
      bank::notify();
    }
   void setEntry(int ind){}
   void setBankEntry(int ind){}

  private :
   runconfig_ptr  _runconfig;

    int _hel_order;
    int _helr_order;
    int _run_order;
    int _event_order;
    int _timestamp_order;
    int _pair_order;
    int _pattern_order;
    int _status_order;

    int _currHelicity=0;
    
  };

  using helflip_ptr=std::shared_ptr<clas12::helflip>;

}

#endif /* UTILS_H */
