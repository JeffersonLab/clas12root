/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   runconfig.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef RUNCONFIG_RUNCONFIG_H
#define RUNCONFIG_RUNCONFIG_H

#include <iostream>
#include <vector>
#include <memory>
#include "bank.h"

namespace clas12 {

  class runconfig : public hipo::bank {


  public:

 
    runconfig(hipo::schema __schema);
    virtual ~runconfig() = default;
 
   int    getRun(){ return getInt(_run_order,0); }
   int    getEvent(){ return getInt(_ev_order,0); }
   int    getUnixTime(){ return getInt(_ut_order,0); }
   long   getTrigger(){ return getLong(_tr_order,0); }
   long   getTimeStamp(){ return getLong(_ts_order,0); }
   int    getType(){ return getByte(_ty_order,0); }
   int    getMode(){ return getByte(_mo_order,0); }
   float  getTorus(){ return getFloat(_to_order,0); }
   float  getSolenoid(){ return getFloat(_so_order,0); }


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

    
    int _run_order;
    int _ev_order;
    int _ut_order;
    int _tr_order;
    int _ts_order;
    int _ty_order;
    int _mo_order;
    int _to_order;
    int _so_order;
   
    
  };

  using runconfig_ptr=std::shared_ptr<clas12::runconfig>;

}

#endif /* UTILS_H */
