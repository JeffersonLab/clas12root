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
    _recTindex_order = __schema.getEntryOrder("recTindex"); 
    _pindex_order = __schema.getEntryOrder("pindex"); 
    _inAcc_order = __schema.getEntryOrder("isInAcc"); 
    _nClust_order = __schema.getEntryOrder("nMCclusters"); 
    _fraction_order = __schema.getEntryOrder("fraction"); 
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

}
