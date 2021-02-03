/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "scintillator.h"


namespace clas12 {

 
  scintillator::scintillator(hipo::schema aschema,hipo::schema extras):
    clas12::particle_detector(aschema),
    _extras{new scintextras(extras)}
  {
 
    if(useItem("layer"))_layer_order   = aschema.getEntryOrder("layer");
    if(useItem("energy"))_energy_order  = aschema.getEntryOrder("energy");
    if(useItem("path"))_path_order    = aschema.getEntryOrder("path");
    if(useItem("time"))_time_order    = aschema.getEntryOrder("time");
    if(useItem("sector"))_sector_order    = aschema.getEntryOrder("sector");
    if(useItem("status"))_status_order    = aschema.getEntryOrder("status");
    if(useItem("x"))_x_order  = aschema.getEntryOrder("x");
    if(useItem("y"))_y_order  = aschema.getEntryOrder("y");
    if(useItem("z"))_z_order  = aschema.getEntryOrder("z");
    if(useItem("hx"))_hx_order  = aschema.getEntryOrder("hx");
    if(useItem("hy"))_hy_order  = aschema.getEntryOrder("hy");
    if(useItem("hz"))_hz_order  = aschema.getEntryOrder("hz");
    if(useItem("chi2"))_chi2_order  = aschema.getEntryOrder("chi2");
    if(useItem("component"))_component_order  = aschema.getEntryOrder("component");
    
  }
  scintillator::scintillator(hipo::schema aschema):
    clas12::particle_detector(aschema)  {
 
    if(useItem("layer"))_layer_order   = aschema.getEntryOrder("layer");
    if(useItem("energy"))_energy_order  = aschema.getEntryOrder("energy");
    if(useItem("path"))_path_order    = aschema.getEntryOrder("path");
    if(useItem("time"))_time_order    = aschema.getEntryOrder("time");
    if(useItem("sector"))_sector_order    = aschema.getEntryOrder("sector");
    if(useItem("status"))_status_order    = aschema.getEntryOrder("status");
    if(useItem("x"))_x_order  = aschema.getEntryOrder("x");
    if(useItem("y"))_y_order  = aschema.getEntryOrder("y");
    if(useItem("z"))_z_order  = aschema.getEntryOrder("z");
    if(useItem("hx"))_hx_order  = aschema.getEntryOrder("hx");
    if(useItem("hy"))_hy_order  = aschema.getEntryOrder("hy");
    if(useItem("hz"))_hz_order  = aschema.getEntryOrder("hz");
    if(useItem("chi2"))_chi2_order  = aschema.getEntryOrder("chi2");
    if(useItem("component"))_component_order  = aschema.getEntryOrder("component");
    
  }


}
