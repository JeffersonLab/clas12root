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
    
    int _pid_order;
    int _px_order;
    int _py_order;
    int _pz_order;
    int _vx_order;
    int _vy_order;
    int _vz_order;
    int _beta_order;
    int _ch_order;
    int _st_order;
    int _chi2pid_order;
    
    short _entry=0;
    
  public:

    particle() = default;
    
    
    particle(hipo::schema __schema);
    particle(hipo::schema __schema,ftbpar_ptr ftbpar);
    
    virtual ~particle() = default;
    
    
    int    getPid(int index) { return getInt(_pid_order,index);}
    float  getPx(int index)  { return getFloat(_px_order,index);}
    float  getPy(int index)  { return getFloat(_py_order,index);}
    float  getPz(int index)  { return getFloat(_pz_order,index);}
    float  getVx(int index)  { return getFloat(_vx_order,index);}
    float  getVy(int index)  { return getFloat(_vy_order,index);}
    float  getVz(int index)  { return getFloat(_vz_order,index);}
    float  getBeta(int index)  { return getFloat(_beta_order,index);}
    float  getChi2Pid(int index)  { return getFloat(_chi2pid_order,index);}
    int    getCharge(int index)  { return getByte(_ch_order,index);}
    int    getStatus(int index)  { return getShort(_st_order,index);}

    int    getPid() { return getInt(_pid_order,_entry);}
    float  getPx()  { return getFloat(_px_order,_entry);}
    float  getPy()  { return getFloat(_py_order,_entry);}
    float  getPz()  { return getFloat(_pz_order,_entry);}
    float  getVx()  { return getFloat(_vx_order,_entry);}
    float  getVy()  { return getFloat(_vy_order,_entry);}
    float  getVz()  { return getFloat(_vz_order,_entry);}
    float  getBeta()  { return getFloat(_beta_order,_entry);}
    float  getChi2Pid()  { return getFloat(_chi2pid_order,_entry);}
    int    getCharge()  { return getByte(_ch_order,_entry);}
    int    getStatus()  { return getShort(_st_order,_entry);}

    int    getFTBPid() { return _ftbpar->getPid();}
    float  getFTBBeta()  { return _ftbpar->getBeta();}
    float  getFTBChi2Pid()  { return _ftbpar->getChi2Pid();}
    int    getFTBStatus()  { return _ftbpar->getStatus();}

    float getP(){
      auto x= getFloat(_px_order,_entry);
      auto y= getFloat(_py_order,_entry);
      auto z= getFloat(_pz_order,_entry);
      return sqrt(x*x+y*y+z*z);
    }
    
    void setEntry(short i){ _entry=i;if(_ftbpar)_ftbpar->setEntry(i);}
    void setBankEntry(short i){ _entry=i;} //faster for BankHist
    short getEntry() const {return _entry;}
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
  
  using par_ptr=std::shared_ptr<clas12::particle>;

}

#endif /* UTILS_H */
