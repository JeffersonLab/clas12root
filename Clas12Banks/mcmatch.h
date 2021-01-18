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
    int  getIsInAcc(int index) const noexcept { return getByte(_inAcc_order,index);}
    int  getNMCclusters(int index) const noexcept { return getShort(_nClust_order,index);}
    float  getFraction(int index) const noexcept { return getFloat(_fraction_order,index);}
    */
    
    int  getMCTindex() const noexcept { return getShort(_mcTindex_order,_index);}
    int  getRecTindex() const noexcept { return getShort(_recTindex_order,_index);}
    int  getPindex() const noexcept { return getShort(_pindex_order,_index);}
    int  getIsInAcc() const noexcept { return getByte(_inAcc_order,_index);}
    int  getNMCclusters() const noexcept { return getShort(_nClust_order,_index);}
    float  getFraction() const noexcept { return getFloat(_fraction_order,_index);}

    int  getPindex(int index) const noexcept { return getShort(_pindex_order,index);}
 
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
    int _recTindex_order{-1};
    int _pindex_order{-1};
    int _inAcc_order{-1};
    int _nClust_order{-1};
    int _fraction_order{-1};
 
    std::vector<int> _rvec{};
 
    short _index={0};

  };
  
  using mcmatch_ptr=clas12::mcmatch*;
  using mcmatch_uptr=std::unique_ptr<clas12::mcmatch>;

}

#endif /* UTILS_H */
