/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "mesonex_trigger.h"

namespace clas12 {

  mesonex_trigger::mesonex_trigger(clas12reader& c12){
    _superlayer_sector_thres = 5;
    _ftof_pcal_distance = 4;
    _pcal_cluster_energy_threshold = 0.01; //MeV?
    _idx_TBHits = c12.addBank("TimeBasedTrkg::TBHits");
    _TBHits = c12.getBank(_idx_TBHits);	

    //time based hits
    _id_tbhit_superlayer = c12.getBankOrder(_idx_TBHits,"superlayer");
    _id_tbhit_sector     = c12.getBankOrder(_idx_TBHits,"sector");

    //tof and pcal coincidence + clusters
    _idx_FTOFHits = c12.addBank("FTOF::hits");
    _idx_PCALClusters = c12.addBank("ECAL::clusters");
    _id_FTOF = c12.getBankOrder(_idx_FTOFHits,"id"); //FTOF1b? where to check for this?
    _id_PCAL = c12.getBankOrder(_idx_PCALClusters,"idU");
    _id_ECAL_id     = c12.getBankOrder(_idx_PCALClusters,"id");
    _id_ECAL_layer  = c12.getBankOrder(_idx_PCALClusters,"layer");
    _id_ECAL_energy = c12.getBankOrder(_idx_PCALClusters,"energy");
    _FTOF = c12.getBank(_idx_FTOFHits);
    _PCAL = c12.getBank(_idx_PCALClusters);
  }
  
  /**
   * Fire mesonex trigger
   *
   * @return true if all trigger conditions are met
   */
  bool mesonex_trigger::fire(){
    return (superlayer_sectors() && pcal_cluster_energy() && ftof_pcal_coincidence());
  }

  bool mesonex_trigger::superlayer_sectors(){
    //Sector superlayer hits condition
    
    //initialise values
    std::vector< std::set<int> > superlayer_hits(7);
    auto nTBHits = _TBHits->getRows();
    
    //loop over TBHits to get super layers for each hit
    for(int i=0;i<nTBHits;i++){   
      auto superlayer = _TBHits->getByte(_id_tbhit_superlayer, i);
      auto sector     = _TBHits->getByte(_id_tbhit_sector, i);
      superlayer_hits[sector].insert(superlayer);
    }
    
    //iterate over the vector of sets, the size of each set == unique # superlayers hit
    int sectors_pass = 0;
    for(int i=1;i<7;i++) {
      int superlayers_hit = superlayer_hits[i].size();
      if(superlayers_hit >= _superlayer_sector_thres){
	sectors_pass+=1;
      }
    }
    return sectors_pass>0; //true if any sectors have n superlayers hit above threshold
  }

  bool mesonex_trigger::pcal_cluster_energy(){
    //PCAL cluster must have energy above threshold
    //get values of interest
    auto nECALclu = _PCAL->getRows();
    int trigger = 0;
    
    //loop over clusters
    for(int j=0;j<nECALclu;j++){   
      auto id       = _PCAL->getShort(_id_ECAL_id, j);
      auto layer    = _PCAL->getShort(_id_ECAL_layer, j);
      float energy   = _PCAL->getFloat(_id_ECAL_energy, j); 
      if((0<layer) && (layer<=3) && (energy>=_pcal_cluster_energy_threshold)){
      	trigger+=1;
      }
    }
    return trigger>0;
  }

  bool mesonex_trigger::ftof_pcal_coincidence(){
    //one hit in FTOF1B matched to one hit in a PCAL-U bar, with a certain matching geometry
    int mapping[] = {
      0, 1, 1, 2, 2, 3, 4, 4, 5, 6, 7, 7, 8, 9, 9, 10, 
      11, 11, 12, 13, 13, 14, 15, 15, 16, 16, 17, 17, 
      18, 19, 19, 20, 21, 21, 22, 23, 23, 24, 24, 25, 
      26, 26, 27, 28, 28, 29, 30, 30, 31, 32, 32, 33, 
      34, 35, 37, 38, 39, 41, 42, 44, 45, 46, 48, 49, 
      50, 52, 53, 54 };
    
    auto nrowsFTOF = _FTOF->getRows();
    auto nrowsPCAL = _PCAL->getRows();      
    
    for(int i=0;i<nrowsFTOF;++i){
      for(int j=0;j<nrowsPCAL;++j){
	int FTOF_id = _FTOF->getInt(_id_FTOF,i);
	int PCAL_id = _PCAL->getInt(_id_PCAL,j);
	int FTOF_exp_id = mapping[PCAL_id-1] + 1;
	auto diff = abs(FTOF_id-FTOF_exp_id);
	if (diff<=_ftof_pcal_distance){
	  return true;
	}
      }
    }
    return false;
  } 
}
