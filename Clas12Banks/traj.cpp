/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "traj.h"


namespace clas12 {

 
  traj::traj(hipo::schema __schema): clas12::particle_detector(__schema) {

    _cx_order   =  __schema.getEntryOrder("cx");
    _cy_order   =  __schema.getEntryOrder("cy");
    _cz_order   =  __schema.getEntryOrder("cz");
    _path_order   =  __schema.getEntryOrder("path");
    _x_order   =  __schema.getEntryOrder("x");
    _y_order   =  __schema.getEntryOrder("y");
    _z_order   =  __schema.getEntryOrder("z");
    _layer_order   = __schema.getEntryOrder("layer");
 
  }


}
