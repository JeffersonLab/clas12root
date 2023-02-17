/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "calextras.h"


namespace clas12 {

 
  calextras::calextras(hipo::schema __schema): hipo::bank(__schema) {
 
    _dbstU_order   = __schema.getEntryOrder("dbstU");
    _dbstV_order   = __schema.getEntryOrder("dbstV");
    _dbstW_order   = __schema.getEntryOrder("dbstW");
    _rawEU_order   = __schema.getEntryOrder("rawEU");
    _rawEV_order   = __schema.getEntryOrder("rawEV");
    _rawEW_order   = __schema.getEntryOrder("rawEW");
    _recEU_order   = __schema.getEntryOrder("recEU");
    _recEV_order   = __schema.getEntryOrder("recEV");
    _recEW_order   = __schema.getEntryOrder("recEW");
    _recDTU_order   = __schema.getEntryOrder("recDTU");
    _recDTV_order   = __schema.getEntryOrder("recDTV");
    _recDTW_order   = __schema.getEntryOrder("recDTW");
    _recFTU_order   = __schema.getEntryOrder("recFTU");
    _recFTV_order   = __schema.getEntryOrder("recFTV");
    _recFTW_order   = __schema.getEntryOrder("recFTW");
   
   }


}
