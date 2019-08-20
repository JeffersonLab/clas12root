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
#include <memory>


namespace clas12 {

   class calorimeter : public particle_detector {

 
  public:


    calorimeter()=default;

   calorimeter(hipo::schema __schema);
 
    virtual ~calorimeter()=default;
    
    int getLayer(int index) override{
      if(index>-1)return getInt(_layer_order,index);
      return 0;
    }
   //getter funtions for items in calorimeter bank
    double getTime(){
      if(_index>-1)return getFloat(_time_order,_index);
      return 0;
    }
    double getEnergy(){
      if(_index>-1)return getFloat(_energy_order,_index);
      return 0;
    }
    double getPath(){
      if(_index>-1)return getFloat(_path_order,_index);
      return 0;
    }
    int getLayer() override{
      if(_index>-1)return getByte(_layer_order,_index);
      return 0;
    }
   int getSector(){
      if(_index>-1)return getByte(_sector_order,_index);
      return 0;
    }
    double getX(){
      if(_index>-1)return getFloat(_x_order,_index);
      return 0;
    }
    double getY(){
      if(_index>-1)return getFloat(_y_order,_index);
      return 0;
    }
    double getZ(){
      if(_index>-1)return getFloat(_z_order,_index);
      return 0;
    }
    double getDu(){
      if(_index>-1)return getFloat(_du_order,_index);
      return 0;
    }
    double getDv(){
      if(_index>-1)return getFloat(_dv_order,_index);
      return 0;
    }
    double getDw(){
      if(_index>-1)return getFloat(_dw_order,_index);
      return 0;
    }
    double getHx(){
      if(_index>-1)return getFloat(_hx_order,_index);
      return 0;
    }
    double getHy(){
      if(_index>-1)return getFloat(_hy_order,_index);
      return 0;
    }
    double getHz(){
      if(_index>-1)return getFloat(_hz_order,_index);
      return 0;
    }
    double getLu(){
      if(_index>-1)return getFloat(_lu_order,_index);
      return 0;
    }
    double getLv(){
      if(_index>-1)return getFloat(_lv_order,_index);
      return 0;
    }
    double getLw(){
      if(_index>-1)return getFloat(_lw_order,_index);
      return 0;
    }
    double getM2u(){
      if(_index>-1)return getFloat(_m2u_order,_index);
      return 0;
    }
    double getM2v(){
      if(_index>-1)return getFloat(_m2v_order,_index);
      return 0;
    }
    double getM2w(){
      if(_index>-1)return getFloat(_m2w_order,_index);
      return 0;
    }
    double getM3u(){
      if(_index>-1)return getFloat(_m3u_order,_index);
      return 0;
    }
    double getM3v(){
      if(_index>-1)return getFloat(_m3v_order,_index);
      return 0;
    }
    double getM3w(){
      if(_index>-1)return getFloat(_m3w_order,_index);
      return 0;
    }
    int getStatus(){
      if(_index>-1)return getShort(_status_order,_index);
      return 0;
    }
  
     
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
  

   }; //class calorimeter

   using cal_ptr=std::shared_ptr<clas12::calorimeter>;
 
}//namespace clas12

#endif /* UTILS_H */
