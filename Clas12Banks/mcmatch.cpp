/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "mcmatch.h"

#include <algorithm>

namespace clas12 {

  mcmatch::mcmatch(hipo::schema __schema): hipo::bank(__schema) {
    _mcTindex_order = __schema.getEntryOrder("mcTindex"); 
    _pindex_order = __schema.getEntryOrder("pindex"); 
    _mclayerstrk_order= __schema.getEntryOrder("MCLayersTrk"); 
    _mclayersneut_order= __schema.getEntryOrder("MCLayersNeut"); 
    _reclayerstrk_order= __schema.getEntryOrder("RecLayersTrk"); 
    _reclayersneut_order= __schema.getEntryOrder("RecLayersNeut"); 
   }

  
  int mcmatch::getIndex(int pindex){
    std::vector<int>::iterator it;
    if((it=std::find(_rvec.begin(),_rvec.end(),pindex))!=_rvec.end()){
      _index = std::distance(_rvec.begin(), it);
      return _index;
    }
    return _index=-1;
  }


  void  mcmatch::scanIndex(){
     _rvec.clear();
     const int size = getRows();
     _rvec.reserve(size);
     for(int i = 0; i < size; i++){
       int pindex   = getPindex(i);
       _rvec.emplace_back(pindex);
     }
   }
  
   bool mcmatch::checkFDSuperLayers(const short nMinSL, const short nMinLayerPerSL) const noexcept {
      auto pattern=getMCLayersTrk();
      short isSuper = 0;
      for(uint isu=0;isu<6;++isu){//6 superlayers
	short isLay = 0;
	for(uint ilay=0;ilay<6;++ilay){//6 layers
	  isLay+=static_cast<short>(checkBit(pattern,isu*6+ilay));       
	}
	if(isLay>=nMinLayerPerSL) ++isSuper;//at least 4 layers
      }
      if(isSuper>=nMinSL) return true; //at least 5 superlayers
      return false;
    }

}
