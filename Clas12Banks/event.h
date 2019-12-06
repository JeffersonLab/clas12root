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
 
    long    getCategory() const noexcept{ return getLong(_cat_order,0); }
    long    getTopology() const noexcept{ return getLong(_top_order,0); }
    float  getBeamCharge() const noexcept{ return getFloat(_bch_order,0); }
    int    getHelicity() const noexcept{ return getByte(_hel_order,0); }
    int    getHelicityRaw() const noexcept{ return getByte(_helr_order,0); }
    float  getStartTime() const noexcept{ return getFloat(_st_order,0); }
    float  getRFTime() const noexcept{ return getFloat(_rf_order,0); }
    float  getProcTime() const noexcept{ return getFloat(_pt_order,0); }
    double  getLiveTime() const noexcept{ return getDouble(_lt_order,0); }

    float  getFTBStartTime() const noexcept{ if(_ftev) return _ftev->getStartTime();return 0; }

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
    
    int _cat_order{-1};
    int _top_order{-1};
    int _bch_order{-1};
    int _hel_order{-1};
    int _helr_order{-1};
    int _st_order{-1};
    int _rf_order{-1};
    int _pt_order{-1};
    int _lt_order{-1};
 
    
  };

  using event_ptr=std::shared_ptr<clas12::event>;

}

#endif /* UTILS_H */
