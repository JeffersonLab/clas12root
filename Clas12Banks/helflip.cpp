/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "helflip.h"

namespace clas12 {

   
  helflip::helflip(hipo::schema __schema,runconfig_ptr  runconfig): hipo::bank(__schema), _runconfig(runconfig){
    auto sch=getSchema();
     
    _hel_order  = sch.getEntryOrder("helicity");
    _helr_order  = sch.getEntryOrder("helicityRaw");
    _run_order  = sch.getEntryOrder("run");
    _event_order  = sch.getEntryOrder("event");
    _timestamp_order  = sch.getEntryOrder("timestamp");
    _pair_order  = sch.getEntryOrder("pair");
    _pattern_order  = sch.getEntryOrder("pattern");
    _status_order  = sch.getEntryOrder("status");
   }
  void helflip::helicityAnalysis(){
    auto runTimeStamp=_runconfig->getTimeStamp();
    //...
    //This function needs to be defined
    _currHelicity=0;
    //  std::cout<<runTimeStamp<<" "<<getTimeStamp()<<std::endl;
  }
}
