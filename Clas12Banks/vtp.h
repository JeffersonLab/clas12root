/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   vtp.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef VTP_H
#define VTP_H

#include "bank.h"
#include <bitset>
#include <memory>

namespace clas12 {

  class vtp : public hipo::bank {


  public:
    vtp(hipo::schema __schema);
    
    int    getCrate(int index){ return getInt(cr_order,index); }
    int    getWord(int index){ return getInt(wo_order,index); }
     
    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * particles and or map particles by pid or type (i.e. charge)
    */
    void notify(){
      //printf("particle class is read again\n");
    }
    long  makeVTPTriggers();
    void  decodeVTPTrigger(int word1vtp, int word2vtp);
    void  addVTPTriggerToEvent(long pattern);
  private :

    int cr_order;
    int wo_order;
     
    std::bitset<32> _VTPBitSet;
  };

  using vtp_ptr=std::shared_ptr<clas12::vtp>;

}

#endif /* UTILS_H */
