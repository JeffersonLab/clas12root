/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "mcevent.h"


namespace clas12 {

  mcevent::mcevent(hipo::schema __schema): hipo::bank(__schema) {
    _npart_order = __schema.getEntryOrder("npart");
    _atarget_order = __schema.getEntryOrder("atarget");
    _ztarget_order = __schema.getEntryOrder("ztarget");
    _ptarget_order = __schema.getEntryOrder("ptarget");
    _pbeam_order = __schema.getEntryOrder("pbeam");
    _btype_order = __schema.getEntryOrder("btype");
    _ebeam_order = __schema.getEntryOrder("ebeam");
    _targetid_order = __schema.getEntryOrder("targetid");
    _processid_order = __schema.getEntryOrder("processid");
    _weight_order = __schema.getEntryOrder("weight");
    
  }


}
