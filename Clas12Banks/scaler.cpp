/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "scaler.h"

namespace clas12 {


  scaler::scaler(hipo::schema __schema): hipo::bank(__schema) {
 
    _fcupgated_order = __schema.getEntryOrder("fcupgated");
    _fcup_order = __schema.getEntryOrder("fcup");
    _livetime_order = __schema.getEntryOrder("livetime");
    
  }
 
}
