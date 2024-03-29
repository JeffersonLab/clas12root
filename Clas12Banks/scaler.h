/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   scaler.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef SCALER_H
#define SCALER_H

#include <hipo4/bank.h>
#include <memory>

namespace clas12 {

  class scaler : public hipo::bank {


  public:

    scaler(hipo::schema __schema);

    float    getFCupGated(){ return getFloat(_fcupgated_order,0); }
    float    getFCup(){ return getFloat(_fcup_order,0); }
    float    getLiveTime(){ return getFloat(_livetime_order,0); }
 
  private :

    int _fcupgated_order{-1};
    int _fcup_order{-1};
    int _livetime_order{-1};
  
    
  };

  using scaler_ptr=clas12::scaler*;
  using scaler_uptr=std::unique_ptr<clas12::scaler>;

}

#endif /* UTILS_H */
