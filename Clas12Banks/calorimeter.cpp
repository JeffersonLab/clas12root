/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "calorimeter.h"


namespace clas12 {

 
 
  calorimeter::calorimeter(hipo::schema __schema): clas12::particle_detector(__schema) {
 
    auto sch=getSchema();
 
    if(useItem("layer"))_layer_order   = sch.getEntryOrder("layer");
    if(useItem("energy"))_energy_order  = sch.getEntryOrder("energy");
    if(useItem("path"))_path_order    = sch.getEntryOrder("path");
    if(useItem("time"))_time_order    = sch.getEntryOrder("time");
    if(useItem("x"))_x_order  = sch.getEntryOrder("x");
    if(useItem("y"))_y_order  = sch.getEntryOrder("y");
    if(useItem("z"))_z_order  = sch.getEntryOrder("z");
    if(useItem("status"))_status_order  = sch.getEntryOrder("status");
    if(useItem("sector"))_sector_order  = sch.getEntryOrder("sector");
    if(useItem("chi2"))_chi2_order  = sch.getEntryOrder("chi2");
    if(useItem("du"))_du_order  = sch.getEntryOrder("du");
    if(useItem("dv"))_dv_order  = sch.getEntryOrder("dv");
    if(useItem("dw"))_dw_order  = sch.getEntryOrder("dw");
    if(useItem("hx"))_hx_order  = sch.getEntryOrder("hx");
    if(useItem("hy"))_hy_order  = sch.getEntryOrder("hy");
    if(useItem("hz"))_hz_order  = sch.getEntryOrder("hz");
    if(useItem("lu"))_lu_order  = sch.getEntryOrder("lu");
    if(useItem("lv"))_lv_order  = sch.getEntryOrder("lv");
    if(useItem("m2u"))_m2u_order  = sch.getEntryOrder("m2u");
    if(useItem("m2v"))_m2v_order  = sch.getEntryOrder("m2v");
    if(useItem("m2w"))_m2w_order  = sch.getEntryOrder("m2w");
    if(useItem("m3u"))_m3u_order  = sch.getEntryOrder("m3u");
    if(useItem("m3v"))_m3v_order  = sch.getEntryOrder("m3v");
    if(useItem("m3w"))_m3w_order  = sch.getEntryOrder("m3w");
  }


}
