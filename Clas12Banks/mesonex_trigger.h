/*
 * File:   mesonex_trigger.h
 * Author: A. Thornton
 *
 */

#ifndef CLAS12_MESONEX_TRIGGER_H
#define CLAS12_MESONEX_TRIGGER_H

#include <iostream>
#include <vector>
#include <memory>
#include "clas12defs.h"
#include "clas12reader.h"
#include "bank.h"
#include "utils.h"
#include "dictionary.h"

namespace clas12 {

  class mxtrig {
    
  public:
    
    mxtrig(clas12reader& c12);
    bool fire();
    
  private:
    
    hipo::bank* _TBhits{};
    int superlayer_sector_thres=0;
    int idx_TBHits=0;
  };
  //using mesonex_trigger_ptr=std::shared_ptr<clas12::mesonex_trigger>;  //need this?
  
}//namespace clas12

#endif /* UTILS_H */
