/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "scintillator.h"


namespace clas12 {

 
  scintillator::scintillator(hipo::schema __schema): clas12::particle_detector(__schema) {
 
    if(useItem("layer"))_layer_order   = __schema.getEntryOrder("layer");
    if(useItem("energy"))_energy_order  = __schema.getEntryOrder("energy");
    if(useItem("path"))_path_order    = __schema.getEntryOrder("path");
    if(useItem("time"))_time_order    = __schema.getEntryOrder("time");
    if(useItem("sector"))_sector_order    = __schema.getEntryOrder("sector");
    if(useItem("status"))_status_order    = __schema.getEntryOrder("status");
    if(useItem("x"))_x_order  = __schema.getEntryOrder("x");
    if(useItem("y"))_y_order  = __schema.getEntryOrder("y");
    if(useItem("z"))_z_order  = __schema.getEntryOrder("z");
    if(useItem("hx"))_hx_order  = __schema.getEntryOrder("hx");
    if(useItem("hy"))_hy_order  = __schema.getEntryOrder("hy");
    if(useItem("hz"))_hz_order  = __schema.getEntryOrder("hz");
    if(useItem("chi2"))_chi2_order  = __schema.getEntryOrder("chi2");
   
  }


}
