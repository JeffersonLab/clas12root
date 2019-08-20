/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "scaler.h"

namespace clas12 {


  scaler::scaler(hipo::schema __schema): hipo::bank(__schema) {
 
    auto sch=getSchema();
    ch_order = sch.getEntryOrder("channel");
    sl_order = sch.getEntryOrder("slot");
    val_order = sch.getEntryOrder("value");
    hel_order = sch.getEntryOrder("helicity");
 
  }
 
}
