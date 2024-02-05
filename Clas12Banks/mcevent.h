/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   mcevent.h
 * Author: dglazier
 *
 */

#ifndef MCEVENT_H
#define MCEVENT_H

#include <hipo4/bank.h>
#include <cmath>
#include <memory>

namespace clas12 {

  class mcevent : public hipo::bank {

    
  public:

    mcevent() = default;

    mcevent(hipo::schema __schema);

    virtual ~mcevent() = default;


 
    int    getNpart() const noexcept { return getShort(_npart_order,_entry);}
    int    getAtarget() const noexcept { return getShort(_atarget_order,_entry);}
    int    getZtarget() const noexcept { return getShort(_ztarget_order,_entry);}
    float    getPtarget() const noexcept { return getFloat(_ptarget_order,_entry);}
    float    getPbeam() const noexcept { return getFloat(_pbeam_order,_entry);}
    float    getEbeam() const noexcept { return getFloat(_ebeam_order,_entry);}
    int    getBtype() const noexcept { return getShort(_btype_order,_entry);}
    int    getTargetid() const noexcept { return getShort(_targetid_order,_entry);}
    int    getProcessid() const noexcept { return getShort(_processid_order,_entry);}
    float    getWeight() const noexcept { return getFloat(_weight_order,_entry);}

    
    //void setEntry(short i){ _entry=i;}
    // void setBankEntry(short i){ _entry=i;} //faster for BankHist
    short getEntry() const  noexcept{return _entry;}
    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * mcevents and or map mcevents by pid or type (i.e. charge)
    */
    void notify() final {
      bank::notify();
    }

  private:

    int _npart_order{-1};
    int _atarget_order{-1};
    int _ztarget_order{-1};
    int _ptarget_order{-1};
    int _pbeam_order{-1};
    int _btype_order{-1};
    int _ebeam_order{-1};
    int _targetid_order{-1};
    int _processid_order{-1};
    int _weight_order{-1};
 
   
 
    short _entry={0};

  };
  
  using mcevt_ptr=clas12::mcevent*;
  using mcevt_uptr=std::unique_ptr<clas12::mcevent>;

}

#endif /* UTILS_H */
