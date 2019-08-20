/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ftparticle.h"
#include "dictionary.h"


namespace clas12 {

  ftparticle::ftparticle(hipo::dictionary __factory): hipo::bank(__factory.getSchema("RECFT::particle")) {
 
    auto sch=getSchema();
    pid_order = sch.getEntryOrder("pid");
    ch_order  = sch.getEntryOrder("charge");
    st_order  = sch.getEntryOrder("status");
    beta_order  = sch.getEntryOrder("beta");
    chi2pid_order  = sch.getEntryOrder("chi2pid");
    
  }

  ftparticle::ftparticle(hipo::schema __schema): hipo::bank(__schema) {
 
    auto sch=getSchema();
    pid_order = sch.getEntryOrder("pid");
    st_order  = sch.getEntryOrder("status");
    beta_order  = sch.getEntryOrder("beta");
    chi2pid_order  = sch.getEntryOrder("chi2pid");
     
  }

  
}
