/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "forwardtagger.h"


namespace clas12 {

 
  forwardtagger::forwardtagger(hipo::schema __schema): clas12::particle_detector(__schema) {

    if(useItem("layer"))_layer_order   = __schema.getEntryOrder("layer");
    if(useItem("energy"))_energy_order  = __schema.getEntryOrder("energy");
    if(useItem("path"))_path_order    = __schema.getEntryOrder("path");
    if(useItem("time"))_time_order    = __schema.getEntryOrder("time");
    if(useItem("status"))_status_order    = __schema.getEntryOrder("status");
    if(useItem("time"))_time_order    = __schema.getEntryOrder("time");
    if(useItem("x"))_x_order  = __schema.getEntryOrder("x");
    if(useItem("y"))_y_order  = __schema.getEntryOrder("y");
    if(useItem("z"))_z_order  = __schema.getEntryOrder("z");
    if(useItem("dx"))_dx_order  = __schema.getEntryOrder("dx");
    if(useItem("dy"))_dy_order  = __schema.getEntryOrder("dy");
    if(useItem("radius"))_radius_order  = __schema.getEntryOrder("radius");
    if(useItem("size"))_size_order  = __schema.getEntryOrder("size");
    if(useItem("chi2"))_chi2_order  = __schema.getEntryOrder("chi2");
   
  }


}
