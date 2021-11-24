/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   mcparticle.h
 * Author: dglazier
 *
 */

#ifndef MCPARTICLE_H
#define MCPARTICLE_H

#include "bank.h"
#include "mcmatch.h"
#include <cmath>
#include <memory>

namespace clas12 {

  class mcparticle : public hipo::bank {

    
  public:

    mcparticle() = default;

    mcparticle(hipo::schema aschema);
    mcparticle(hipo::schema aschema,hipo::schema schmatch);

    virtual ~mcparticle() = default;


    int    getPid(int index) const noexcept { return getInt(_pid_order,index);}
    float  getPx(int index) const noexcept  { return getFloat(_px_order,index);}
    float  getPy(int index) const noexcept  { return getFloat(_py_order,index);}
    float  getPz(int index) const noexcept  { return getFloat(_pz_order,index);}
    float  getVx(int index) const noexcept  { return getFloat(_vx_order,index);}
    float  getVy(int index) const noexcept  { return getFloat(_vy_order,index);}
    float  getVz(int index) const noexcept  { return getFloat(_vz_order,index);}
    float  getMass(int index) const noexcept  { return getFloat(_mass_order,index);}
    int    getIndex(int index) const noexcept { return getByte(_index_order,index);}
    int    getType(int index) const noexcept { return getByte(_type_order,index);}
    int    getParent(int index) const noexcept { return getByte(_parent_order,index);}
    int    getDaughter(int index) const noexcept { return getByte(_daughter_order,index);}
    float  getLifetime(int index)  const noexcept { return getFloat(_life_order,index);}
 
    int    getPid() const noexcept { return _entry==-1 ? 0: getInt(_pid_order,_entry);}
    float  getPx()  const noexcept { return _entry==-1 ? 0: getFloat(_px_order,_entry);}
    float  getPy() const noexcept  { return _entry==-1 ? 0: getFloat(_py_order,_entry);}
    float  getPz() const noexcept  { return _entry==-1 ? 0: getFloat(_pz_order,_entry);}
    float  getVx() const noexcept  { return _entry==-1 ? 0: getFloat(_vx_order,_entry);}
    float  getVy() const noexcept  { return _entry==-1 ? 0: getFloat(_vy_order,_entry);}
    float  getVz() const noexcept  { return _entry==-1 ? 0: getFloat(_vz_order,_entry);}
    float  getMass() const noexcept  { return _entry==-1 ? 0: getFloat(_mass_order,_entry);}
    int    getIndex() const noexcept { return _entry==-1 ? 0: getByte(_index_order,_entry);}
    int    getType() const noexcept { return  _entry==-1 ? 0: getByte(_type_order,_entry);}
    int    getParent() const noexcept { return _entry==-1 ? 0: getByte(_parent_order,_entry);}
    int    getDaughter() const noexcept { return _entry==-1 ? 0: getByte(_daughter_order,_entry);}
    float  getLifetime()  const noexcept { return _entry==-1 ? 0: getFloat(_life_order,_entry);}
 
 
    float getP() const noexcept{
      auto x= getFloat(_px_order,_entry);
      auto y= getFloat(_py_order,_entry);
      auto z= getFloat(_pz_order,_entry);
      return sqrt(x*x+y*y+z*z);
    }
    float getTheta() const noexcept{
      auto x= getFloat(_px_order,_entry);
      auto y= getFloat(_py_order,_entry);
      auto z= getFloat(_pz_order,_entry);
      return x == 0.0 && y == 0.0 && z == 0.0 ? 0.0
	: atan2(sqrt(x*x+y*y),z);
    }
    float getPhi() const noexcept{
      auto x= getFloat(_px_order,_entry);
      auto y= getFloat(_py_order,_entry);
      return atan2(y,x);
    }
    
    void setEntry(short i){
      if( i<getRows() )_entry=i;
      else _entry = -1;
      
     }
    void setMatchEntry(short ip,short imc){
      _entry=imc;
      _matched_pindex=ip;
      if(_match.get())_match->setEntry(_matched_pindex);
     }
    void setBankEntry(short i){
      _entry=i;
    } //faster for BankHist
    short getEntry() const  noexcept{return _entry;}
    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * mcparticles and or map mcparticles by pid or type (i.e. charge)
    */
    void notify() final {
      _entry=-1;
      _matched_pindex=-1;
      bank::notify();
    }
    int match_to(int pindex){//pindex is index of reconstructed particle
      _matched_pindex=pindex;
      return _match.get()!=nullptr ?
	_entry=_match->getMCindex(pindex) : _entry=-1 ;
    }
    bool isMatched() const noexcept{return _entry>=0;}
    
    //mcmatch* getParticleMatch()const {return _match.get();}

    //if matched
    mcmatch* getMatch()const {
      return _match.get();
    }
      
  private:

    int _pid_order{-1};
    int _px_order{-1};
    int _py_order{-1};
    int _pz_order{-1};
    int _vx_order{-1};
    int _vy_order{-1};
    int _vz_order{-1};
    int _mass_order{-1};

    int _type_order{-1};
    int _life_order{-1};
    int _index_order{-1};
    int _parent_order{-1};
    int _daughter_order{-1};
    
    short _entry={0};
    short _matched_pindex;
    mcmatch_uptr _match;
  };
  
  using mcpar_ptr=clas12::mcparticle*;
  using mcpar_uptr=std::unique_ptr<clas12::mcparticle>;

}

#endif /* UTILS_H */
