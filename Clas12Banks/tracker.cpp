/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tracker.h"


namespace clas12 {

 
  tracker::tracker(hipo::schema __schema): clas12::particle_detector(__schema) {

    if(useItem("sector"))_sector_order    = __schema.getEntryOrder("sector");
    if(useItem("status"))_status_order    = __schema.getEntryOrder("status");
    if(useItem("NDF"))_NDF_order  = __schema.getEntryOrder("NDF");
    if(useItem("q"))_q_order  = __schema.getEntryOrder("q");
    if(useItem("chi2"))_chi2_order  = __schema.getEntryOrder("chi2");
   
  }


}
