/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   particle.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include "bank.h"
#include "dictionary.h"
#include "ftbparticle.h"
#include <cmath>
#include <memory>

namespace clas12 {

  class particle : public hipo::bank {

  private:
    ftbpar_ptr _ftbpar; //FT based bank
    
    int _pid_order{-1};
    int _px_order{-1};
    int _py_order{-1};
    int _pz_order{-1};
    int _vx_order{-1};
    int _vy_order{-1};
    int _vz_order{-1};
    int _vt_order{-1};
    int _beta_order{-1};
    int _ch_order{-1};
    int _st_order{-1};
    int _chi2pid_order{-1};
    
    short _entry={0};
    
  public:

    particle() = default;
    
    
    particle(hipo::schema __schema);
    particle(hipo::schema __schema,ftbpar_ptr ftbpar);
    
    ~particle() override = default;
    
    
    int    getPid(int index)  const noexcept{ return getInt(_pid_order,index);}
    float  getPx(int index)   const noexcept{ return getFloat(_px_order,index);}
    float  getPy(int index)   const noexcept{ return getFloat(_py_order,index);}
    float  getPz(int index)   const noexcept{ return getFloat(_pz_order,index);}
    float  getVx(int index)   const noexcept{ return getFloat(_vx_order,index);}
    float  getVy(int index)   const noexcept{ return getFloat(_vy_order,index);}
    float  getVz(int index)   const noexcept{ return getFloat(_vz_order,index);}
    float  getVt(int index)   const noexcept{ return getFloat(_vt_order,index);}
    float  getBeta(int index)   const noexcept{ return getFloat(_beta_order,index);}
    float  getChi2Pid(int index)   const noexcept{ return getFloat(_chi2pid_order,index);}
    int    getCharge(int index)   const noexcept{ return getByte(_ch_order,index);}
    int    getStatus(int index)   const noexcept{ return getShort(_st_order,index);}

    int    getPid()  const noexcept{ return getInt(_pid_order,_entry);}
    float  getPx()   const noexcept{ return getFloat(_px_order,_entry);}
    float  getPy()   const noexcept{ return getFloat(_py_order,_entry);}
    float  getPz()   const noexcept{ return getFloat(_pz_order,_entry);}
    float  getVx()   const noexcept{ return getFloat(_vx_order,_entry);}
    float  getVy()   const noexcept{ return getFloat(_vy_order,_entry);}
    float  getVz()   const noexcept{ return getFloat(_vz_order,_entry);}
    float  getVt()   const noexcept{ return getFloat(_vt_order,_entry);}
    float  getBeta()   const noexcept{ return getFloat(_beta_order,_entry);}
    float  getChi2Pid()   const noexcept{ return getFloat(_chi2pid_order,_entry);}
    int    getCharge()   const noexcept{ return getByte(_ch_order,_entry);}
    int    getStatus()   const noexcept{ return getShort(_st_order,_entry);}

    int    getFTBPid()   const noexcept{ return _ftbpar->getPid();}
    float  getFTBVt()    const noexcept{ return _ftbpar->getVt();}
    float  getFTBBeta()    const noexcept{ return _ftbpar->getBeta();}
    float  getFTBChi2Pid()    const noexcept{ return _ftbpar->getChi2Pid();}
    int    getFTBStatus()    const noexcept{ return _ftbpar->getStatus();}

    float getP() const noexcept{
      auto x= getFloat(_px_order,_entry);
      auto y= getFloat(_py_order,_entry);
      auto z= getFloat(_pz_order,_entry);
      return sqrt(x*x+y*y+z*z);
    }
    
    void setEntry(short i){ _entry=i;if(_ftbpar)_ftbpar->setEntry(i);}
    void setBankEntry(short i){ _entry=i;} //faster for BankHist
    short getEntry() const noexcept{return _entry;}
    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * particles and or map particles by pid or type (i.e. charge)
    */
    void notify() final {
      bank::notify();
      //printf("particle class is read again\n");
    }
  };
  
  using par_ptr=clas12::particle*;
  using par_uptr=std::unique_ptr<clas12::particle>;

}

#endif /* UTILS_H */
