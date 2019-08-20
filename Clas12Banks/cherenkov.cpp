/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "cherenkov.h"


namespace clas12 {

 
  cherenkov::cherenkov(hipo::schema __schema): clas12::particle_detector(__schema) {
    
    if(useItem("nphe"))_nphe_order  = __schema.getEntryOrder("nphe");
    if(useItem("path"))_path_order    = __schema.getEntryOrder("path");
    if(useItem("time"))_time_order    = __schema.getEntryOrder("time");
    if(useItem("x"))_x_order  = __schema.getEntryOrder("x");
    if(useItem("y"))_y_order  = __schema.getEntryOrder("y");
    if(useItem("z"))_z_order  = __schema.getEntryOrder("z");
    if(useItem("status"))_status_order  = __schema.getEntryOrder("status");
    if(useItem("sector"))_sector_order  = __schema.getEntryOrder("sector");
    if(useItem("chi2"))_chi2_order  = __schema.getEntryOrder("chi2");
    if(useItem("dtheta"))_dtheta_order  = __schema.getEntryOrder("dtheta");
    if(useItem("dphi"))_dphi_order  = __schema.getEntryOrder("dphi");
  }


}
