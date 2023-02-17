/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   detector.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef CLAS12_CALORIMETER_H
#define CLAS12_CALORIMETER_H

#include "particle_detector.h"
#include "calextras.h"
#include <memory>


namespace clas12 {

   class calorimeter : public particle_detector {

 
  public:


     calorimeter()=default;

     calorimeter(hipo::schema __schema);
     calorimeter(hipo::schema __schema,hipo::schema extras);
     
    ~calorimeter() override=default;
    
    int getLayer(int index)  const noexcept override{
      if(index>-1)return getByte(_layer_order,index);
      return 0;
    }
   //getter funtions for items in calorimeter bank
    double getTime() const noexcept{
      if(_index>-1)return getFloat(_time_order,_index);
      return 0;
    }
    double getEnergy() const noexcept{
      if(_index>-1)return getFloat(_energy_order,_index);
      return 0;
    }
    double getPath() const noexcept{
      if(_index>-1)return getFloat(_path_order,_index);
      return 0;
    }
     double getChi2() const noexcept{
      if(_index>-1)return getFloat(_chi2_order,_index);
      return 0;
    }
    int getLayer()  const noexcept override{
      if(_index>-1)return getByte(_layer_order,_index);
      return 0;
    }
   int getSector() const noexcept{
      if(_index>-1)return getByte(_sector_order,_index);
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
    double getDu() const noexcept{
      if(_index>-1)return getFloat(_du_order,_index);
      return 0;
    }
    double getDv() const noexcept{
      if(_index>-1)return getFloat(_dv_order,_index);
      return 0;
    }
    double getDw() const noexcept{
      if(_index>-1)return getFloat(_dw_order,_index);
      return 0;
    }
    double getHx() const noexcept{
      if(_index>-1)return getFloat(_hx_order,_index);
      return 0;
    }
    double getHy() const noexcept{
      if(_index>-1)return getFloat(_hy_order,_index);
      return 0;
    }
    double getHz() const noexcept{
      if(_index>-1)return getFloat(_hz_order,_index);
      return 0;
    }
    double getLu() const noexcept{
      if(_index>-1)return getFloat(_lu_order,_index);
      return 0;
    }
    double getLv() const noexcept{
      if(_index>-1)return getFloat(_lv_order,_index);
      return 0;
    }
    double getLw() const noexcept{
      if(_index>-1)return getFloat(_lw_order,_index);
      return 0;
    }
    double getM2u() const noexcept{
      if(_index>-1)return getFloat(_m2u_order,_index);
      return 0;
    }
    double getM2v() const noexcept{
      if(_index>-1)return getFloat(_m2v_order,_index);
      return 0;
    }
    double getM2w() const noexcept{
      if(_index>-1)return getFloat(_m2w_order,_index);
      return 0;
    }
    double getM3u() const noexcept{
      if(_index>-1)return getFloat(_m3u_order,_index);
      return 0;
    }
    double getM3v() const noexcept{
      if(_index>-1)return getFloat(_m3v_order,_index);
      return 0;
    }
    double getM3w() const noexcept{
      if(_index>-1)return getFloat(_m3w_order,_index);
      return 0;
    }
    int getStatus() const noexcept{
      if(_index>-1)return getShort(_status_order,_index);
      return 0;
    }

     //get extras
     
     int getDbstU() const noexcept{
       return _extras.get()!=nullptr ? _extras->getDbstU(_index):0;
     }
     int getDbstV() const noexcept{
       return _extras.get()!=nullptr ? _extras->getDbstV(_index):0;
     }
     int getDbstW() const noexcept{
       return _extras.get()!=nullptr ? _extras->getDbstW(_index):0;
     }
     double getRawEU() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRawEU(_index):0;
     }
     double getRawEV() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRawEV(_index):0;
     }
     double getRawEW() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRawEW(_index):0;
     }
     double getRecEU() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRecEU(_index):0;
     }
     double getRecEV() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRecEV(_index):0;
     }
     double getRecEW() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRecEW(_index):0;
     }
     double getRecDTU() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRecDTU(_index):0;
     }
     double getRecDTV() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRecDTV(_index):0;
     }
     double getRecDTW() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRecDTW(_index):0;
     }
     double getRecFTU() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRecFTU(_index):0;
     }
     double getRecFTV() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRecFTV(_index):0;
     }
     double getRecFTW() const noexcept{
       return _extras.get()!=nullptr ? _extras->getRecFTW(_index):0;
     }
    calextras* getExtras()const {return _extras.get();}

     
 private:

    int   _layer_order=-1;
    int  _energy_order=-1;
    int    _path_order=-1;
    int    _time_order=-1;
    int  _sector_order=-1;
    int       _x_order=-1;
    int       _y_order=-1;
    int       _z_order=-1;
    int    _chi2_order=-1;
    int    _du_order=-1;
    int    _dv_order=-1;
    int    _dw_order=-1;
    int    _hx_order=-1;
    int    _hy_order=-1;
    int    _hz_order=-1;
    int    _lu_order=-1;
    int    _lv_order=-1;
    int    _lw_order=-1;
    int    _m2u_order=-1;
    int    _m2v_order=-1;
    int    _m2w_order=-1;
    int    _m3u_order=-1;
    int    _m3v_order=-1;
    int    _m3w_order=-1;
    int    _status_order=-1;
  
     calextra_uptr _extras;
 
   }; //class calorimeter

   using cal_ptr=clas12::calorimeter*;
   using cal_uptr=std::unique_ptr<clas12::calorimeter>;
 
}//namespace clas12

#endif /* UTILS_H */
