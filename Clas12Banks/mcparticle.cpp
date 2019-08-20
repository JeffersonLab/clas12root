/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "mcparticle.h"


namespace clas12 {

  mcparticle::mcparticle(hipo::schema __schema): hipo::bank(__schema) {
    pid_order = __schema.getEntryOrder("pid");
    px_order  = __schema.getEntryOrder("px");
    py_order  = __schema.getEntryOrder("py");
    pz_order  = __schema.getEntryOrder("pz");
    vx_order  = __schema.getEntryOrder("vx");
    vy_order  = __schema.getEntryOrder("vy");
    vz_order  = __schema.getEntryOrder("vz");
    mass_order  = __schema.getEntryOrder("mass");
    
  }


}
