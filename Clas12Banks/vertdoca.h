/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   vertdoca.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef VERTDOCA_H
#define VERTDOCA_H

#include <iostream>
#include <vector>
#include <memory>
#include "bank.h"

namespace clas12 {

  class vertdoca : public hipo::bank {


  public:

 
    vertdoca(hipo::schema __schema);
    virtual ~vertdoca() = default;

    short    getIndex1(int entry) const noexcept{ return getShort(_index1_order,entry); }
    short    getIndex2(int entry) const noexcept{ return getShort(_index2_order,entry); }
    float    getX(int entry) const noexcept{ return getFloat(_x_order,entry); }
    float    getY(int entry) const noexcept{ return getFloat(_y_order,entry); }
    float    getZ(int entry) const noexcept{ return getFloat(_z_order,entry); }
    float    getX1(int entry) const noexcept{ return getFloat(_x1_order,entry); }
    float    getY1(int entry) const noexcept{ return getFloat(_y1_order,entry); }
    float    getZ1(int entry) const noexcept{ return getFloat(_z1_order,entry); }
    float    getX2(int entry) const noexcept{ return getFloat(_x2_order,entry); }
    float    getY2(int entry) const noexcept{ return getFloat(_y2_order,entry); }
    float    getZ2(int entry) const noexcept{ return getFloat(_z2_order,entry); }

    float    getCx1(int entry) const noexcept{ return getFloat(_cx1_order,entry); }
    float    getCy1(int entry) const noexcept{ return getFloat(_cy1_order,entry); }
    float    getCz1(int entry) const noexcept{ return getFloat(_cz1_order,entry); }
    float    getCx2(int entry) const noexcept{ return getFloat(_cx2_order,entry); }
    float    getCy2(int entry) const noexcept{ return getFloat(_cy2_order,entry); }
    float    getCz2(int entry) const noexcept{ return getFloat(_cz2_order,entry); }
    float    getR(int entry) const noexcept{ return getFloat(_r_order,entry); }


    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * particles and or map particles by pid or type (i.e. charge)
    */
    void notify() final{
      bank::notify();
    }
  
    int getCombinationEntry(const int index1,const int index2){
      for(int i=0;i<getSize();++i){
	auto i1= getIndex1(i);
	auto i2= getIndex2(i);
	if(i1==index1&&i2==index2)
	  return i;
	else if(i2==index1&&i1==index2)
	  return i;
      }
      return -1;
    }
    bool isTrack1(const int index1,const int combiEntry){
      return index1==getIndex1(combiEntry);
    }
    
  private :

    
    int _index1_order{-1};
    int _index2_order{-1};
    int _x_order{-1};
    int _y_order{-1};
    int _z_order{-1};
    int _x1_order{-1};
    int _y1_order{-1};
    int _z1_order{-1};
    int _x2_order{-1};
    int _y2_order{-1};
    int _z2_order{-1};
    int _cx1_order{-1};
    int _cy1_order{-1};
    int _cz1_order{-1};
    int _cx2_order{-1};
    int _cy2_order{-1};
    int _cz2_order{-1};
    int _r_order{-1};
 
    
  };

  using vertdoca_ptr=clas12::vertdoca*;
  using vertdoca_uptr=std::unique_ptr<clas12::vertdoca>;

}

#endif /* UTILS_H */
