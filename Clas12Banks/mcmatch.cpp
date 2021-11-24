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

 
}
