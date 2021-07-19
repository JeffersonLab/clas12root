
#include "vertdoca.h"

namespace clas12 {

   
   
  vertdoca::vertdoca(hipo::schema __schema): hipo::bank(__schema) {
    auto& sch=getSchema();
     
    _index1_order  = sch.getEntryOrder("index1");
    _index2_order  = sch.getEntryOrder("index2");
    _x_order  = sch.getEntryOrder("x");
    _y_order  = sch.getEntryOrder("y");
    _z_order  = sch.getEntryOrder("z");
    _x1_order  = sch.getEntryOrder("x1");
    _y1_order  = sch.getEntryOrder("y1");
    _z1_order  = sch.getEntryOrder("z1");
    _x2_order  = sch.getEntryOrder("x2");
    _y2_order  = sch.getEntryOrder("y2");
    _z2_order  = sch.getEntryOrder("z2");
    _cx1_order  = sch.getEntryOrder("cx1");
    _cy1_order  = sch.getEntryOrder("cy1");
    _cz1_order  = sch.getEntryOrder("cz1");
    _cx2_order  = sch.getEntryOrder("cx2");
    _cy2_order  = sch.getEntryOrder("cy2");
    _cz2_order  = sch.getEntryOrder("cz2");
   
    }
}
