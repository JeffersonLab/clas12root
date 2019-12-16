/*
 * File:   mesonex_trigger.h
 * Author: A. Thornton
 *
 */

#ifndef CLAS12_MESONEX_TRIGGER_H
#define CLAS12_MESONEX_TRIGGER_H

#include <vector>
#include <set>
#include "clas12defs.h"
#include "clas12reader.h"
#include "utils.h"

namespace clas12 {

  class mesonex_trigger {

  public:
    
    mesonex_trigger(clas12reader& c12);
    bool fire();
    bool superlayer_sectors();
    
  private:
    
    hipo::bank* _TBHits{};
    int _id_tbhit_superlayer{};
    int _id_tbhit_sector{};
    
    int _superlayer_sector_thres=0;
    int _idx_TBHits=0;
  };
  
  using mesonex_trigger_ptr=std::shared_ptr<clas12::mesonex_trigger>;
  
}//namespace clas12

#endif /* UTILS_H */
