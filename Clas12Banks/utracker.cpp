#include "utracker.h"


namespace clas12 {

 
  utracker::utracker(hipo::schema __schema): clas12::particle_detector(__schema) {

    if(useItem("sector"))_sector_order    = __schema.getEntryOrder("sector");
    if(useItem("status"))_status_order    = __schema.getEntryOrder("status");
    if(useItem("NDF"))_NDF_order  = __schema.getEntryOrder("NDF");
    if(useItem("q"))_q_order  = __schema.getEntryOrder("q");
    if(useItem("chi2"))_chi2_order  = __schema.getEntryOrder("chi2");
    if(useItem("px"))_px_order  = __schema.getEntryOrder("px");
    if(useItem("py"))_py_order  = __schema.getEntryOrder("py");
    if(useItem("pz"))_pz_order  = __schema.getEntryOrder("pz");
    if(useItem("vx"))_vx_order  = __schema.getEntryOrder("vx");
    if(useItem("vy"))_vy_order  = __schema.getEntryOrder("vy");
    if(useItem("vz"))_vz_order  = __schema.getEntryOrder("vz");
  
  }


}
