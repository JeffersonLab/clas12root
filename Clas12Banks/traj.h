/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   traj.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef CLAS12_TRAJ_H
#define CLAS12_TRAJ_H

#include "particle_detector.h"


namespace clas12 {

   class traj : public particle_detector {

 
  public:


    traj()=default;

    traj(hipo::schema __schema);
 
    ~traj() override =default;
    
    //getter funtions for items in traj bank
   int getLayer(int index)   const noexcept override{
     if(index>-1)return getByte(_layer_order,index);
      return 0;
    }
    int getLayer()  const noexcept override{
      if(_index>-1)return getByte(_layer_order,_index);
      return 0;
    }
    double getCx()  const noexcept{ 
      if(_index>-1)return getFloat(_cx_order,_index);
      return 0;
    }
    double getCy()  const noexcept{ 
      if(_index>-1)return getFloat(_cy_order,_index);
      return 0;
    }
    double getCz()  const noexcept{ 
      if(_index>-1)return getFloat(_cz_order,_index);
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
     double getEdge()  const noexcept{ 
       if(_index>-1)return getFloat(_edge_order,_index);
       return 0;
     }
     
     double getPath()  const noexcept{ 
       if(_index>-1)return getFloat(_path_order,_index);
       return 0;
     }
   
 private:

    int    _cx_order=-1;
    int    _cy_order=-1;
    int    _cz_order=-1;
    int    _layer_order=-1;
    int    _path_order=-1;
    int    _x_order=-1;
    int    _y_order=-1;
    int    _z_order=-1;
     int    _edge_order=-1;
 

 
   }; //class traj

   using traj_ptr=clas12::traj*;
   using traj_uptr=std::unique_ptr<clas12::traj>;

}//namespace clas12

#endif /* UTILS_H */
