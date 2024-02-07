/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   scintextras.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef CLAS12_SCINTEXTRAS_H
#define CLAS12_SCINTEXTRAS_H

#include <hipo4/bank.h>
#include <memory>
//#include "scintillator.h"


namespace clas12 {

  class scintillator;
  
   class scintextras : public  hipo::bank {

 
  public:


    scintextras()=default;

    scintextras(hipo::schema __schema);

    ~scintextras() override=default;
    

    double getDedx() const noexcept{ 
      if(_index>-1)return getFloat(_dedx_order,_index);
      return 0.;
    }
    int getSize() const noexcept{ 
      if(_index>-1)return getShort(_size_order,_index);
      return 0;
    }
    int getLayermulti() const noexcept{ 
      if(_index>-1)return getByte(_layermulti_order,_index);
      return 0;
    }
 
    double getDedx(int index) const noexcept{
      if(index>-1)return getFloat(_dedx_order,index);
      return 0.;
    }
    int getSize(int index) const noexcept{ 
      if(index>-1)return getShort(_size_order,index);
      return 0;
    }
    int getLayermulti(int index) const noexcept{ 
      if(index>-1)return getByte(_layermulti_order,index);
      return 0;
    }
 
  
    void setBankEntry(short i){ _index=i;} //faster for BankHist

 private:
    friend scintillator;
    
    void setIndex(short i){ _index=i;}
 
 
      int    _dedx_order=-1;
      int    _size_order=-1;
      int    _layermulti_order=-1;
 
      int _index=-1;
 
   }; //class scintextras

   using scintextra_ptr=clas12::scintextras*;
   using scintextra_uptr=std::unique_ptr<clas12::scintextras>;

}//namespace clas12

#endif /* UTILS_H */
