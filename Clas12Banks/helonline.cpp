/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "helonline.h"

namespace clas12 {

   
  helonline::helonline(hipo::schema __schema): hipo::bank(__schema) {
    auto sch=getSchema();
     
    _hel_order  = sch.getEntryOrder("helicity");
    _helr_order  = sch.getEntryOrder("helicityRaw");
   }
}
