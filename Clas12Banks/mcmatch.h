/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   mcmatch.h
 * Author: dglazier
 *
 */

#ifndef MCMATCH_H
#define MCMATCH_H

#include "bank.h"
#include <cmath>
#include <memory>

namespace clas12 {

  class mcmatch : public hipo::bank {

    
  public:

    mcmatch() = default;

    mcmatch(hipo::schema __schema);

    ~mcmatch() override = default;


    /*
    int  getMCTindex(int index) const noexcept { return getShort(_mcTindex_order,index);}
    int  getRecTindex(int index) const noexcept { return getShort(_recTindex_order,index);}
    int  getPindex(int index) const noexcept { return getShort(_pindex_order,index);}
    */
    
    short  getMCTindex() const noexcept { return getShort(_mcTindex_order,_index);}
    short  getPindex() const noexcept { return getShort(_pindex_order,_index);}

    //bit patterns
    long  getMCLayersTrk() const noexcept { return getLong(_mclayerstrk_order,_index);}
    long  getMCLayersNeut() const noexcept { return getLong(_mclayersneut_order,_index);}
    long  getRecLayersTrk() const noexcept { return getLong(_reclayerstrk_order,_index);}
    long  getRecLayersNeut() const noexcept { return getLong(_reclayersneut_order,_index);}
 

    int  getPindex(int index) const noexcept { return getShort(_pindex_order,index);}

    bool checkBitInPattern(uint k) const noexcept {
      return  checkBit(getMCLayersTrk(),k);
    }
    bool checkBit(long word, uint k) const noexcept {
      return  ( (word >> k) & 1 )!=0;
    }

    bool checkFDSuperLayers(const short nMinSL, const short nMinLayerPerSL) const noexcept ;
    
    
    void setEntry(short i){ _index=i;}
    void setBankEntry(short i){ _index=i;} //faster for BankHist
    short getEntry() const  noexcept{return _index;}

    int getIndex()   const noexcept{return _index;}
    int getIndex(int pindex);
    
   /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * mcmatchs and or map mcmatchs by pid or type (i.e. charge)
    */
    void notify() final {
      bank::notify();
      scanIndex();
    }
    
    void   scanIndex();

  private:

    int _mcTindex_order{-1};
    int _pindex_order{-1};
    int _mclayerstrk_order{-1};
    int _mclayersneut_order{-1};
    int _reclayerstrk_order{-1};
    int _reclayersneut_order{-1};
 
    std::vector<int> _rvec{};
 
    short _index={0};

  };
  
  using mcmatch_ptr=clas12::mcmatch*;
  using mcmatch_uptr=std::unique_ptr<clas12::mcmatch>;

}

#endif /* UTILS_H */
