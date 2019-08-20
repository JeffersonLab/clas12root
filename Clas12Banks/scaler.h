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

#include "bank.h"
#include <memory>

namespace clas12 {

  class scaler : public hipo::bank {


  public:

    scaler(hipo::schema __schema);

    int    getChannel(){ return getInt(ch_order,0); }
    int    getSlot(){ return getInt(sl_order,0); }
    int    getValue(){ return getInt(val_order,0); }
    int    getHelicity(){ return getInt(hel_order,0); }
    
    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * particles and or map particles by pid or type (i.e. charge)
    */
    void notify(){
      //printf("particle class is read again\n");
    }

  private :

    int ch_order;
    int sl_order;
    int val_order;
    int hel_order;
  
    
  };

  using scaler_ptr=std::shared_ptr<clas12::scaler>;

}

#endif /* UTILS_H */
