/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "event.h"

namespace clas12 {

   
  event::event(hipo::schema __schema,ftbevent_ptr ftb): hipo::bank(__schema),_ftev(ftb){
    auto sch=getSchema();
    
    _cat_order  = sch.getEntryOrder("category");
    _top_order  = sch.getEntryOrder("topology");
    _bch_order  = sch.getEntryOrder("beamCharge");
    _lt_order  = sch.getEntryOrder("liveTime");
    _st_order  = sch.getEntryOrder("startTime");
    _rf_order  = sch.getEntryOrder("RFTime");
    _hel_order  = sch.getEntryOrder("helicity");
    _helr_order  = sch.getEntryOrder("helicityRaw");
    _pt_order  = sch.getEntryOrder("procTime");
   }
  
  event::event(hipo::schema __schema): hipo::bank(__schema) {
    auto sch=getSchema();
     
    _cat_order  = sch.getEntryOrder("category");
    _top_order  = sch.getEntryOrder("topology");
    _bch_order  = sch.getEntryOrder("beamCharge");
    _lt_order  = sch.getEntryOrder("liveTime");
    _st_order  = sch.getEntryOrder("startTime");
    _rf_order  = sch.getEntryOrder("RFTime");
    _hel_order  = sch.getEntryOrder("helicity");
    _helr_order  = sch.getEntryOrder("helicityRaw");
    _pt_order  = sch.getEntryOrder("procTime");
  }
}
