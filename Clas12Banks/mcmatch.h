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
     "name": "MC::RecMatch",
        "group": 40,
        "item" : 6,
        "info": "Rec::Particle -> MC::Particle matching",
        "entries": [
            {"name":"pindex",        "type":"S", "info":"REC::Particle index"},
            {"name":"mcindex",       "type":"S", "info":"MC::Particle index"},
            {"name":"player1",       "type":"L",  "info":"layers from the 1st set of detectors hit by Recon particle"},
            {"name":"player2",       "type":"L",  "info":"layers from the 1st set of detectors hit by Recon particle"},
            {"name":"mclayer1",      "type":"L",  "info":"layers from the 1st set of detectors hit by MC particle"},
            {"name":"mclayer2",      "type":"L",  "info":"layers from the 2nd set of detectors hit by MC particle"},
            {"name":"quality",       "type":"F",  "info":"matching quality parameter"}
    */
    //OR
    /*
 "name": "MC::GenMatch",
        "group": 40,
        "item" : 5,
        "info": "MC::Particle -> REC::Particle matching",
        "entries": [
            {"name":"mcindex",       "type":"S", "info":"MC::Particle index"},
            {"name":"pindex",        "type":"S", "info":"REC::Particle index"},
            {"name":"mclayer1",      "type":"L",  "info":"layers from the 1st set of detectors hit by MC particle"},
            {"name":"mclayer2",      "type":"L",  "info":"layers from the 2nd set of detectors hit by MC particle"},
            {"name":"player1",       "type":"L",  "info":"layers from the 1st set of detectors hit by Recon particle"},
            {"name":"player2",       "type":"L",  "info":"layers from the 2nd set of detectors hit by Recon particle"},
            {"name":"quality",       "type":"F",  "info":"matching quality parameter"}
        ] 
     */
    short  getMCindex() const noexcept { return getShort(_mcindex_order,_index);}
    short  getPindex() const noexcept { return getShort(_pindex_order,_index);}

    long getPlayer1() const noexcept {return getLong(_player1_order,_index);} 
    long getPlayer2() const noexcept {return getLong(_player2_order,_index);} 
    long getMClayer1() const noexcept {return getLong(_mclayer1_order,_index);} 
    long getMClayer2() const noexcept {return getLong(_mclayer2_order,_index);} 
    float getQuality() const noexcept {return getFloat(_qual_order,_index);}
 
    
    void setEntry(short i){ _index=i;}
    void setBankEntry(short i){ _index=i;} //faster for BankHist
    short getEntry() const  noexcept{return _index;}

    int getIndex()   const noexcept{return _index;}
    int getIndex(int pindex);
    int getMCIndex(int pindex);
    
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
    
    short  getPindex(int ix) const noexcept { return getShort(_pindex_order,ix);}
    short  getMCindex(int ix) const noexcept { return getShort(_mcindex_order,ix);}

  private:
 
    int _mcindex_order{-1};
    int _pindex_order{-1};
    int _player1_order{-1};
    int _player2_order{-1};
    int _mclayer1_order{-1};
    int _mclayer2_order{-1};
    int _qual_order{-1};

    std::vector<int> _rvec{};
    std::vector<int> _rmcvec{};
 
    short _index={0};

  };
  
  using mcmatch_ptr=clas12::mcmatch*;
  using mcmatch_uptr=std::unique_ptr<clas12::mcmatch>;

}

#endif /* UTILS_H */
