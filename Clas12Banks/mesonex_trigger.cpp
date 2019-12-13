/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "mesonex_trigger.h"

namespace clas12 {

  mxtrig::mxtrig(clas12reader& c12){
    int superlayer_sector_thres = 5;
    auto idx_TBHits = c12.addBank("TimeBasedTrkg::TBHits");
    auto TBHits = c12.getBank(idx_TBHits);	
  }
  
  /**
   * Fire mesonex trigger
   *
   * @return true if all trigger conditions are met
   */
  bool mxtrig::fire(){
    //initialise values
    std::vector< std::set<int> > superlayer_hits(7);
    auto nTBHits = TBHits->getRows();

    //time based hits
    auto id_tbhit_superlayer = c12.getBankOrder(idx_TBHits,"superlayer");
    auto id_tbhit_sector     = c12.getBankOrder(idx_TBHits,"sector");
    
    //loop over TBHits to get super layers for each hit
    for(int i=0;i<nTBHits;i++){   
      int superlayer = TBHits->getByte(id_tbhit_superlayer, i);
      int sector     = TBHits->getByte(id_tbhit_sector, i);
      superlayer_hits[sector].insert(superlayer);
    }
    
    //iterate over the vector of sets, the size of each set == unique # superlayers hit
    int sectors_pass = 0;
    for(int i=1;i<7;i++) {
      int superlayers_hit = superlayer_hits[i].size();
      if(superlayers_hit>=superlayer_sector_thres){
	sectors_pass+=1;
      }
    }
    return sectors_pass>0: //true if any sectors have n superlayers hit above threshold
  }
}
