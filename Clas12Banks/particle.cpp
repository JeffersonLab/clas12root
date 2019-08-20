/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "particle.h"
#include "dictionary.h"


namespace clas12 {

  particle::particle(hipo::schema __schema,ftbpar_ptr ftbpar): hipo::bank(__schema),_ftbpar(ftbpar) {
 
    auto sch=getSchema();
    _pid_order = sch.getEntryOrder("pid");
    _px_order  = sch.getEntryOrder("px");
    _py_order  = sch.getEntryOrder("py");
    _pz_order  = sch.getEntryOrder("pz");
    _vx_order  = sch.getEntryOrder("vx");
    _vy_order  = sch.getEntryOrder("vy");
    _vz_order  = sch.getEntryOrder("vz");
    _ch_order  = sch.getEntryOrder("charge");
    _st_order  = sch.getEntryOrder("status");
    _beta_order  = sch.getEntryOrder("beta");
    _chi2pid_order  = sch.getEntryOrder("chi2pid");
    
  }

  particle::particle(hipo::schema __schema): hipo::bank(__schema) {
 
    auto sch=getSchema();
    _pid_order = sch.getEntryOrder("pid");
    _px_order  = sch.getEntryOrder("px");
    _py_order  = sch.getEntryOrder("py");
    _pz_order  = sch.getEntryOrder("pz");
    _vx_order  = sch.getEntryOrder("vx");
    _vy_order  = sch.getEntryOrder("vy");
    _vz_order  = sch.getEntryOrder("vz");
    _ch_order  = sch.getEntryOrder("charge");
    _st_order  = sch.getEntryOrder("status");
    _beta_order  = sch.getEntryOrder("beta");
    _chi2pid_order  = sch.getEntryOrder("chi2pid");
     
  }

  
}
