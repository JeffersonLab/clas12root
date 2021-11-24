/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "mcmatch.h"

#include <algorithm>

namespace clas12 {

  mcmatch::mcmatch(hipo::schema __schema): hipo::bank(__schema) {
    _mcindex_order = __schema.getEntryOrder("mcindex"); 
    _pindex_order = __schema.getEntryOrder("pindex"); 
    _player1_order = __schema.getEntryOrder("player1"); 
    _player2_order = __schema.getEntryOrder("player2"); 
    _mclayer1_order = __schema.getEntryOrder("mclayer1"); 
    _mclayer2_order = __schema.getEntryOrder("mclayer2"); 
    _qual_order = __schema.getEntryOrder("quality"); 
    }


  //return mcmatch index for this particle pindex
  // int mcmatch::getIndex(int pindex){
  //   std::vector<int>::iterator it;
  //   if((it=std::find(_rvec.begin(),_rvec.end(),pindex))!=_rvec.end()){
  //     _index = std::distance(_rvec.begin(), it);
  //     return _index;
  //   }
  //   return _index=-1;
  // }
  //return mcmatch index for this particle pindex
  // int mcmatch::findMCIndex(int mcindex){
  //   std::vector<int>::iterator it;
  //   if((it=std::find(_rmcvec.begin(),_rmcvec.end(),mcindex))!=_rmcvec.end()){
  //     _index = std::distance(_rmcvec.begin(), it);
  //     return _index;
  //   }
  //   return _index=-1;
  // }

  // //find pindex order
  // void  mcmatch::scanIndex(){
  //    _rvec.clear();
  //    const int size = getRows();
  //    _rvec.reserve(size);
     
  //    _rmcvec.clear();
  //    _rmcvec.reserve(size);

  //    for(int i = 0; i < size; i++){
  //      int pindex   = getPindex(i);
  //      _rvec.emplace_back(pindex);
  //      int mcindex   = getMCindex(i);
  //      _rmcvec.emplace_back(mcindex);
  //    }
  //  }
  
 
}
