/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "scintextras.h"


namespace clas12 {

 
  scintextras::scintextras(hipo::schema __schema): hipo::bank(__schema) {
 
    _dedx_order   = __schema.getEntryOrder("dedx");
    _size_order   = __schema.getEntryOrder("size");
    _layermulti_order   = __schema.getEntryOrder("layermult");
  
    
  }


}
