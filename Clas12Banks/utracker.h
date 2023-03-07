/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   utracker.h
 * Author: dglazier
 *
 * Created on February 20, 2023
 */

#ifndef CLAS12_UTRACKER_H
#define CLAS12_UTRACKER_H

#include "particle_detector.h"


namespace clas12 {

   class utracker : public particle_detector {

 
  public:


    utracker()=default;

    utracker(hipo::schema __schema);
 
    ~utracker() override=default;
    
    //getter funtions for items in utracker bank
    int getNDF() const noexcept { 
      if(_index>-1)return getShort(_NDF_order,_index);
      return 0;
    }
     int getSector() const noexcept { 
      if(_index>-1)return getByte(_sector_order,_index);
      return 0;
    }
    int getStatus() const noexcept { 
      if(_index>-1)return getShort(_status_order,_index);
      return 0;
    }
    int getCharge() const noexcept { 
      if(_index>-1)return getByte(_q_order,_index);
      return 0;
    }
   double getChi2() const noexcept { 
     if(_index>-1)return getFloat(_chi2_order,_index);
     return 0;
   }
   double getChi2N() const noexcept {
     auto N=getNDF();
     if(N)return getChi2()/N;
     return 0;
   }
   double getPx() const noexcept { 
     if(_index>-1)return getFloat(_px_order,_index);
     return 0;
   }
   double getPy() const noexcept { 
     if(_index>-1)return getFloat(_py_order,_index);
     return 0;
   }
   double getPz() const noexcept { 
     if(_index>-1)return getFloat(_pz_order,_index);
     return 0;
   }
   double getVx() const noexcept { 
     if(_index>-1)return getFloat(_vx_order,_index);
     return 0;
   }
   double getVy() const noexcept { 
     if(_index>-1)return getFloat(_vy_order,_index);
     return 0;
   }
   double getVz() const noexcept { 
     if(_index>-1)return getFloat(_vz_order,_index);
     return 0;
   }

 private:

    int    _NDF_order=-1;
    int  _sector_order=-1;
    int  _status_order=-1;
    int       _q_order=-1;
    int    _chi2_order=-1;
    int    _px_order=-1;
    int    _py_order=-1;
    int    _pz_order=-1;
    int    _vx_order=-1;
    int    _vy_order=-1;
    int    _vz_order=-1;
 

 
   }; //class utracker

   using utrck_ptr=clas12::utracker*;
   using utrck_uptr=std::unique_ptr<clas12::utracker>;

}//namespace clas12

#endif /* UTILS_H */
