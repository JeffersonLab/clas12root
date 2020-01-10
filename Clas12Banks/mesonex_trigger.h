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
    bool pcal_cluster_energy();
    bool ftof_pcal_coincidence();
    
  private:
    
    int _superlayer_sector_thres=0;
    int _ftof_pcal_distance=0;
    int _pcal_cluster_energy_threshold=0;

    hipo::bank* _TBHits{};
    hipo::bank* _FTOF{};
    hipo::bank* _PCAL{};

    int _id_tbhit_superlayer{};
    int _id_tbhit_sector{};
    int _id_ECAL_id{};
    int _id_ECAL_layer{};
    int _id_ECAL_energy{};

    int _idx_TBHits{};
    int _idx_FTOFHits{};
    int _idx_PCALClusters{};
    int _id_FTOF{};
    int _id_PCAL{};
  };
  
  using mesonex_trigger_ptr=clas12::mesonex_trigger*;
  using mesonex_trigger_uptr=std::unique_ptr<clas12::mesonex_trigger>;
  
}//namespace clas12

#endif /* UTILS_H */
