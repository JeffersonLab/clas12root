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

#ifndef CLAS12_HELONLINE_H
#define CLAS12_HELONLINE_H

#include <iostream>
#include <vector>
#include <memory>
#include <hipo4/bank.h>

namespace clas12 {

  class helonline : public hipo::bank {


  public:

 
    helonline(hipo::schema __schema);
    virtual ~helonline() = default;
 
    int    getHelicity() const noexcept{ return getByte(_hel_order,0); }
    int    getHelicityRaw() const noexcept{ return getByte(_helr_order,0); }
    
    int getCurrHelicity(){ return 1;}
    
    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * particles and or map particles by pid or type (i.e. charge)
    */
    void notify() final{
      bank::notify();
    }
   void setEntry(int ind) const{}
   void setBankEntry(int ind){}

  private :
    
   int _hel_order{-1};
   int _helr_order{-1};
 
    
  };

  using helonline_ptr=clas12::helonline*;
  using helonline_uptr=std::unique_ptr<clas12::helonline>;

}

#endif /* UTILS_H */
