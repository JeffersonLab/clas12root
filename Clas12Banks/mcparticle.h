/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   mcparticle.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef MCPARTICLE_H
#define MCPARTICLE_H

#include "bank.h"
#include <cmath>
#include <memory>

namespace clas12 {

  class mcparticle : public hipo::bank {

    
  public:

    mcparticle() = default;

    mcparticle(hipo::schema __schema);

    virtual ~mcparticle() = default;


    int    getPid(int index) const noexcept { return getInt(pid_order,index);}
    float  getPx(int index) const noexcept  { return getFloat(px_order,index);}
    float  getPy(int index) const noexcept  { return getFloat(py_order,index);}
    float  getPz(int index) const noexcept  { return getFloat(pz_order,index);}
    float  getVx(int index) const noexcept  { return getFloat(vx_order,index);}
    float  getVy(int index) const noexcept  { return getFloat(vy_order,index);}
    float  getVz(int index) const noexcept  { return getFloat(vz_order,index);}
    float  getMass(int index) const noexcept  { return getFloat(mass_order,index);}

    int    getPid() const noexcept { return getInt(pid_order,_entry);}
    float  getPx()  const noexcept { return getFloat(px_order,_entry);}
    float  getPy() const noexcept  { return getFloat(py_order,_entry);}
    float  getPz() const noexcept  { return getFloat(pz_order,_entry);}
    float  getVx() const noexcept  { return getFloat(vx_order,_entry);}
    float  getVy() const noexcept  { return getFloat(vy_order,_entry);}
    float  getVz() const noexcept  { return getFloat(vz_order,_entry);}
    float  getMass() const noexcept  { return getFloat(mass_order,_entry);}
 
    /* void  getVector3(int index, vector3 &vect){ */
    /*   vect.setXYZ(getFloat(px_order,index),getFloat(py_order,index), */
    /*         getFloat(pz_order,index)); */
    /* } */

    /* void getVector4(int index, vector4 &vect, double mass){ */
    /*   vect.setXYZM(getFloat(px_order,index),getFloat(py_order,index), */
    /*         getFloat(pz_order,index),getFloat(mass_order,index)); */
    /* } */

    float getP() const noexcept{
      auto x= getFloat(px_order,_entry);
      auto y= getFloat(py_order,_entry);
      auto z= getFloat(pz_order,_entry);
      return sqrt(x*x+y*y+z*z);
    }
    
    void setEntry(short i){ _entry=i;}
    short getEntry() const  noexcept{return _entry;}
    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * mcparticles and or map mcparticles by pid or type (i.e. charge)
    */
    void notify() final {
      bank::notify();
      //printf("mcparticle class is read again\n");
    }

  private:

    int pid_order{-1};
    int px_order{-1};
    int py_order{-1};
    int pz_order{-1};
    int vx_order{-1};
    int vy_order{-1};
    int vz_order{-1};
    int mass_order{-1};
    
    short _entry=0;

  };
  
  using mcpar_ptr=std::shared_ptr<clas12::mcparticle>;

}

#endif /* UTILS_H */
