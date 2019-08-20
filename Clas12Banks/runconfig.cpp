/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "runconfig.h"

namespace clas12 {

   
   
  runconfig::runconfig(hipo::schema __schema): hipo::bank(__schema) {
    auto sch=getSchema();
     
    _run_order  = sch.getEntryOrder("run");
    _ev_order  = sch.getEntryOrder("event");
    _ut_order  = sch.getEntryOrder("unixtime");
    _tr_order  = sch.getEntryOrder("trigger");
    _ts_order  = sch.getEntryOrder("timestamp");
    _ty_order  = sch.getEntryOrder("type");
    _mo_order  = sch.getEntryOrder("mode");
    _to_order  = sch.getEntryOrder("torus");
    _so_order  = sch.getEntryOrder("solenoid");
   }
}
