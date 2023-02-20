/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   forwardtagger.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef CLAS12_FORWARDTAGGER_H
#define CLAS12_FORWARDTAGGER_H

#include "particle_detector.h"


namespace clas12 {

  
  class forwardtagger : public particle_detector {

 
  public:


    forwardtagger()=default;

    forwardtagger(hipo::schema __schema);

    virtual ~forwardtagger()=default;
    
    //getter funtions for items in forwardtagger bank
    int getLayer(int index)   const noexcept override{
      if(index>-1)return getByte(_layer_order,index);
      return 0;
    }
    int getLayer()   const noexcept override{
      if(_index>-1)return getByte(_layer_order,_index);
      return 0;
    }
    double getTime()  const noexcept{ 
      if(_index>-1)return getFloat(_time_order,_index);
      return 0;
    }
    double getEnergy()  const noexcept{ 
      if(_index>-1)return getFloat(_energy_order,_index);
      return 0;
    }
    double getPath()  const noexcept{ 
      if(_index>-1)return getFloat(_path_order,_index);
      return 0;
    }
   int getStatus()  const noexcept{ 
      if(_index>-1)return getShort(_status_order,_index);
      return 0;
    }
   double getX()  const noexcept{ 
      if(_index>-1)return getFloat(_x_order,_index);
      return 0;
    }
    double getY()  const noexcept{ 
      if(_index>-1)return getFloat(_y_order,_index);
      return 0;
    }
    double getZ()  const noexcept{ 
      if(_index>-1)return getFloat(_z_order,_index);
      return 0;
    }
    double getDx()  const noexcept{ 
      if(_index>-1)return getFloat(_dx_order,_index);
      return 0;
    }
    double getDy()  const noexcept{ 
      if(_index>-1)return getFloat(_dy_order,_index);
      return 0;
    }
    double getRadius()  const noexcept{ 
      if(_index>-1)return getFloat(_radius_order,_index);
      return 0;
    }
    int getSizeFT()  const noexcept{ //watch-out banks::getSize() !
      if(_index>-1)return getShort(_size_order,_index);
      return 0;
    }
     double getChi2()  const noexcept{ 
      if(_index>-1)return getFloat(_chi2_order,_index);
      return 0;
    }
  
     
 private:

    int  _energy_order=-1;
    int   _layer_order=-1;
    int    _path_order=-1;
    int    _time_order=-1;
    int  _status_order=-1;
    int       _x_order=-1;
    int       _y_order=-1;
    int       _z_order=-1;
    int       _dx_order=-1;
    int       _dy_order=-1;
    int       _radius_order=-1;
    int       _size_order=-1;
    int    _chi2_order=-1;
 

 
   }; //class forwardtagger

  
  using ft_ptr=clas12::forwardtagger*;
  using ft_uptr=std::unique_ptr<clas12::forwardtagger>;

}//namespace clas12

#endif /* UTILS_H */
