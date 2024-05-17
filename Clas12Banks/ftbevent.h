/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ftbevent.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef FTBEVENT_H
#define FTBEVENT_H

#include <iostream>
#include <vector>
#include <memory>
#include <hipo4/bank.h>

namespace clas12 {

  class ftbevent : public hipo::bank {


  public:

 
    ftbevent(hipo::schema __schema);
    virtual ~ftbevent() = default;
 
    long    getCategory() const  noexcept{ return getLong(_cat_order,0); }
    float  getStartTime() const  noexcept{ return getFloat(_st_order,0); }
    void setEntry(int ind) const {}
    void setBankEntry(int ind){}

    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * particles and or map particles by pid or type (i.e. charge)
    */
    void notify() final{
      bank::notify();
    }

  private :

    int _st_order{-1};
    int _cat_order{-1};
 
    
  };

  using ftbevent_ptr=clas12::ftbevent*;
  using ftbevent_uptr=std::unique_ptr<clas12::ftbevent>;

}

#endif /* UTILS_H */
