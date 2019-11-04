/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tracker.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef CLAS12_TRACKER_H
#define CLAS12_TRACKER_H

#include "particle_detector.h"


namespace clas12 {

   class tracker : public particle_detector {

 
  public:


    tracker()=default;

    tracker(hipo::schema __schema);
 
    ~tracker() override=default;
    
    //getter funtions for items in tracker bank
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
     
 private:

    int    _NDF_order=-1;
    int  _sector_order=-1;
    int  _status_order=-1;
    int       _q_order=-1;
    int    _chi2_order=-1;
 

 
   }; //class tracker

   using trck_ptr=std::shared_ptr<clas12::tracker>;

}//namespace clas12

#endif /* UTILS_H */
