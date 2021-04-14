/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "mcparticle.h"


namespace clas12 {

  mcparticle::mcparticle(hipo::schema aschema,hipo::schema schmatch):
    hipo::bank(aschema),
    _match{new mcmatch{schmatch}}
  {
    _pid_order = aschema.getEntryOrder("pid");
    _px_order  = aschema.getEntryOrder("px");
    _py_order  = aschema.getEntryOrder("py");
    _pz_order  = aschema.getEntryOrder("pz");
    _vx_order  = aschema.getEntryOrder("vx");
    _vy_order  = aschema.getEntryOrder("vy");
    _vz_order  = aschema.getEntryOrder("vz");
    _mass_order  = aschema.getEntryOrder("mass");
    _type_order  = aschema.getEntryOrder("type");
    _life_order  = aschema.getEntryOrder("lifetime");
    _index_order  = aschema.getEntryOrder("index");
    _parent_order  = aschema.getEntryOrder("parent");
    _daughter_order  = aschema.getEntryOrder("daughter");
  
    
  }
 mcparticle::mcparticle(hipo::schema aschema): hipo::bank(aschema) {
    _pid_order = aschema.getEntryOrder("pid");
    _px_order  = aschema.getEntryOrder("px");
    _py_order  = aschema.getEntryOrder("py");
    _pz_order  = aschema.getEntryOrder("pz");
    _vx_order  = aschema.getEntryOrder("vx");
    _vy_order  = aschema.getEntryOrder("vy");
    _vz_order  = aschema.getEntryOrder("vz");
    _mass_order  = aschema.getEntryOrder("mass");
    _type_order  = aschema.getEntryOrder("type");
    _life_order  = aschema.getEntryOrder("lifetime");
    _index_order  = aschema.getEntryOrder("index");
    _parent_order  = aschema.getEntryOrder("parent");
    _daughter_order  = aschema.getEntryOrder("daughter");
  
    
  }


}
