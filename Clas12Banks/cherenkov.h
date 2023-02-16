/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   cherenkov.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef CLAS12_CHERENKOV_H
#define CLAS12_CHERENKOV_H

#include "particle_detector.h"


namespace clas12 {

   class cherenkov : public particle_detector {

 
  public:


    cherenkov()=default;

    cherenkov(hipo::schema __schema);
    
    ~cherenkov() override=default;
    
    //getter funtions for items in cherenkov bank
    double getTime() const noexcept{ 
      if(_index>-1)return getFloat(_time_order,_index);
      return 0;
    }
    double getNphe() const noexcept{ 
      if(_index>-1)return getFloat(_nphe_order,_index);
      return 0;
    }
    double getPath() const noexcept{ 
      if(_index>-1)return getFloat(_path_order,_index);
      return 0;
    }
    int getSector() const noexcept{ 
      if(_index>-1)return getByte(_sector_order,_index);
      return 0;
    }
     double getChi2() const noexcept{ 
      if(_index>-1)return getFloat(_chi2_order,_index);
      return 0;
     }
     double getX() const noexcept{ 
      if(_index>-1)return getFloat(_x_order,_index);
      return 0;
    }
     double getY() const noexcept{ 
      if(_index>-1)return getFloat(_y_order,_index);
      return 0;
    }
    double getZ() const noexcept{ 
      if(_index>-1)return getFloat(_z_order,_index);
      return 0;
    }
    double getDtheta() const noexcept{ 
      if(_index>-1)return getFloat(_dtheta_order,_index);
      return 0;
    }
    double getDPhi() const noexcept{ 
      if(_index>-1)return getFloat(_dphi_order,_index);
      return 0;
    }
    int getStatus() const noexcept{ 
      if(_index>-1)return getShort(_status_order,_index);
      return 0;
    }
  
    
 private:

    int  _nphe_order=-1;
    int    _path_order=-1;
    int    _time_order=-1;
    int    _dphi_order=-1;
    int    _dtheta_order=-1;
    int  _sector_order=-1;
    int       _x_order=-1;
    int       _y_order=-1;
    int       _z_order=-1;
    int    _chi2_order=-1;
    int    _status_order=-1;
  

 
   }; //class cherenkov

   using cher_ptr=clas12::cherenkov*;
   using cher_uptr=std::unique_ptr<clas12::cherenkov>;

}//namespace clas12

#endif /* UTILS_H */
