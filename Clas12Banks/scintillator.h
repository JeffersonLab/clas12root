/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   scintillator.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef CLAS12_SCINTILLATOR_H
#define CLAS12_SCINTILLATOR_H

#include "particle_detector.h"


namespace clas12 {

   class scintillator : public particle_detector {

 
  public:


    scintillator()=default;

    scintillator(hipo::schema __schema);

    virtual ~scintillator()=default;
    
   int getLayer(int index) override{ 
      if(index>-1)return getByte(_layer_order,index);
      return 0;
    }
    //getter funtions for items in scintillator bank
    int getComponent(){ 
      if(_index>-1)return getShort(_component_order,_index);
      return 0;
    }
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
    int getSector(){ 
      if(_index>-1)return getByte(_sector_order,_index);
      return 0;
    }
    int getStatus(){ 
      if(_index>-1)return getShort(_status_order,_index);
      return 0;
    }
    int getLayer() override{ 
      if(_index>-1)return getByte(_layer_order,_index);
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
  double getHX(){ 
      if(_index>-1)return getFloat(_hx_order,_index);
      return 0;
    }
    double getHY(){ 
      if(_index>-1)return getFloat(_hy_order,_index);
      return 0;
    }
    double getHZ(){ 
      if(_index>-1)return getFloat(_hz_order,_index);
      return 0;
    }
      double getChi2(){ 
      if(_index>-1)return getFloat(_chi2_order,_index);
      return 0;
    }
 
    
     
 private:

    int   _layer_order=-1;
    int   _component_order=-1;
    int  _energy_order=-1;
    int    _path_order=-1;
    int    _time_order=-1;
    int  _sector_order=-1;
    int  _status_order=-1;
    int       _x_order=-1;
    int       _y_order=-1;
    int       _z_order=-1;
    int       _hx_order=-1;
    int       _hy_order=-1;
    int       _hz_order=-1;
    int    _chi2_order=-1;
 

 
   }; //class scintillator

   using scint_ptr=std::shared_ptr<clas12::scintillator>;

}//namespace clas12

#endif /* UTILS_H */
