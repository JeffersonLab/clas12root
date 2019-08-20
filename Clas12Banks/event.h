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

#ifndef CLAS12_EVENT_H
#define CLAS12_EVENT_H

#include <iostream>
#include <vector>
#include <memory>
#include "bank.h"
#include "ftbevent.h"

namespace clas12 {

  class event : public hipo::bank {


  public:

 
    event(hipo::schema __schema);
    event(hipo::schema __schema,ftbevent_ptr ftb);
    virtual ~event() = default;
 
    long    getCategory(){ return getLong(_cat_order,0); }
    long    getTopology(){ return getLong(_top_order,0); }
    float  getBeamCharge(){ return getFloat(_bch_order,0); }
    int    getHelicity(){ return getByte(_hel_order,0); }
    int    getHelicityRaw(){ return getByte(_helr_order,0); }
    float  getStartTime(){ return getFloat(_st_order,0); }
    float  getRFTime(){ return getFloat(_rf_order,0); }
    float  getProcTime(){ return getFloat(_pt_order,0); }
    double  getLiveTime(){ return getDouble(_lt_order,0); }

    float  getFTBStartTime(){ if(_ftev) return _ftev->getStartTime();return 0; }

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

    ftbevent_ptr _ftev; //for FTbased start time
    
    int _cat_order;
    int _top_order;
    int _bch_order;
    int _hel_order;
    int _helr_order;
    int _st_order;
    int _rf_order;
    int _pt_order;
    int _lt_order;
 
    
  };

  using event_ptr=std::shared_ptr<clas12::event>;

}

#endif /* UTILS_H */
