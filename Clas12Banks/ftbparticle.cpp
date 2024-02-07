/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ftbparticle.h"


namespace clas12 {

  ftbparticle::ftbparticle(hipo::dictionary __factory): hipo::bank(__factory.getSchema("RECFT::particle")) {
 
    auto sch=getSchema();
    _pid_order = sch.getEntryOrder("pid");
    _st_order  = sch.getEntryOrder("status");
    _vt_order  = sch.getEntryOrder("vt");
    _beta_order  = sch.getEntryOrder("beta");
    _chi2pid_order  = sch.getEntryOrder("chi2pid");
    _px_order  = sch.getEntryOrder("px");
    _py_order  = sch.getEntryOrder("py");
    _pz_order  = sch.getEntryOrder("pz");
    
  }

  ftbparticle::ftbparticle(hipo::schema __schema): hipo::bank(__schema) {
 
    auto sch=getSchema();
    _pid_order = sch.getEntryOrder("pid");
    _st_order  = sch.getEntryOrder("status");
    _beta_order  = sch.getEntryOrder("beta");
    _vt_order  = sch.getEntryOrder("vt");
    _chi2pid_order  = sch.getEntryOrder("chi2pid");
    _px_order  = sch.getEntryOrder("px");
    _py_order  = sch.getEntryOrder("py");
    _pz_order  = sch.getEntryOrder("pz");
    
  }

  
}
