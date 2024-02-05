/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   runconfig.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef RUNCONFIG_H
#define RUNCONFIG_H

#include <iostream>
#include <vector>
#include <memory>
#include <hipo4/bank.h>

namespace clas12 {

  class runconfig : public hipo::bank {


  public:

 
    runconfig(hipo::schema __schema);
    virtual ~runconfig() = default;
    
    int    getRun() const noexcept{ return getInt(_run_order,0); }
    int    getEvent() const noexcept{ return getInt(_ev_order,0); }
    int    getUnixTime() const noexcept{ return getInt(_ut_order,0); }
    long   getTrigger() const noexcept{ return getLong(_tr_order,0); }
    long   getTimeStamp() const noexcept{ return getLong(_ts_order,0); }
    int    getType() const noexcept{ return getByte(_ty_order,0); }
    int    getMode() const noexcept{ return getByte(_mo_order,0); }
    float  getTorus() const noexcept{ return getFloat(_to_order,0); }
    float  getSolenoid() const noexcept{ return getFloat(_so_order,0); }


    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * particles and or map particles by pid or type (i.e. charge)
    */
    void notify() final{
      bank::notify();
    }
    void setEntry(int i){}
    void setBankEntry(int ind){}

  private :

    
    int _run_order{-1};
    int _ev_order{-1};
    int _ut_order{-1};
    int _tr_order{-1};
    int _ts_order{-1};
    int _ty_order{-1};
    int _mo_order{-1};
    int _to_order{-1};
    int _so_order{-1};
   
    
  };

  using runconfig_ptr=clas12::runconfig*;
  using runconfig_uptr=std::unique_ptr<clas12::runconfig>;

}

#endif /* UTILS_H */
