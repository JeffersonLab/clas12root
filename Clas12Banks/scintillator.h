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

    ~scintillator() override=default;
    
   int getLayer(int index) const noexcept override{ 
      if(index>-1)return getByte(_layer_order,index);
      return 0;
    }
    //getter funtions for items in scintillator bank
    int getComponent() const noexcept{ 
      if(_index>-1)return getShort(_component_order,_index);
      return 0;
    }
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
    int getSector() const noexcept{ 
      if(_index>-1)return getByte(_sector_order,_index);
      return 0;
    }
    int getStatus() const noexcept{ 
      if(_index>-1)return getShort(_status_order,_index);
      return 0;
    }
    int getLayer()  const noexcept override{ 
      if(_index>-1)return getByte(_layer_order,_index);
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
  double getHX() const noexcept{ 
      if(_index>-1)return getFloat(_hx_order,_index);
      return 0;
    }
    double getHY() const noexcept{ 
      if(_index>-1)return getFloat(_hy_order,_index);
      return 0;
    }
    double getHZ() const noexcept{ 
      if(_index>-1)return getFloat(_hz_order,_index);
      return 0;
    }
      double getChi2() const noexcept{ 
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

   using scint_ptr=clas12::scintillator*;
   using scint_uptr=std::unique_ptr<clas12::scintillator>;

}//namespace clas12

#endif /* UTILS_H */
