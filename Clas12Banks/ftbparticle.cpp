/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ftbparticle.h"
#include "dictionary.h"


namespace clas12 {

  ftbparticle::ftbparticle(hipo::dictionary __factory): hipo::bank(__factory.getSchema("RECFT::particle")) {
 
    auto sch=getSchema();
    _pid_order = sch.getEntryOrder("pid");
    _st_order  = sch.getEntryOrder("status");
    _beta_order  = sch.getEntryOrder("beta");
    _chi2pid_order  = sch.getEntryOrder("chi2pid");
    
  }

  ftbparticle::ftbparticle(hipo::schema __schema): hipo::bank(__schema) {
 
    auto sch=getSchema();
    _pid_order = sch.getEntryOrder("pid");
    _st_order  = sch.getEntryOrder("status");
    _beta_order  = sch.getEntryOrder("beta");
    _chi2pid_order  = sch.getEntryOrder("chi2pid");
     
  }

  
}
