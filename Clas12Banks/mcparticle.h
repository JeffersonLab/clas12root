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


    int    getPid(int index) { return getInt(pid_order,index);}
    float  getPx(int index)  { return getFloat(px_order,index);}
    float  getPy(int index)  { return getFloat(py_order,index);}
    float  getPz(int index)  { return getFloat(pz_order,index);}
    float  getVx(int index)  { return getFloat(vx_order,index);}
    float  getVy(int index)  { return getFloat(vy_order,index);}
    float  getVz(int index)  { return getFloat(vz_order,index);}
    float  getMass(int index)  { return getFloat(mass_order,index);}

    int    getPid() { return getInt(pid_order,_entry);}
    float  getPx()  { return getFloat(px_order,_entry);}
    float  getPy()  { return getFloat(py_order,_entry);}
    float  getPz()  { return getFloat(pz_order,_entry);}
    float  getVx()  { return getFloat(vx_order,_entry);}
    float  getVy()  { return getFloat(vy_order,_entry);}
    float  getVz()  { return getFloat(vz_order,_entry);}
    float  getMass()  { return getFloat(mass_order,_entry);}
 
    /* void  getVector3(int index, vector3 &vect){ */
    /*   vect.setXYZ(getFloat(px_order,index),getFloat(py_order,index), */
    /*         getFloat(pz_order,index)); */
    /* } */

    /* void getVector4(int index, vector4 &vect, double mass){ */
    /*   vect.setXYZM(getFloat(px_order,index),getFloat(py_order,index), */
    /*         getFloat(pz_order,index),getFloat(mass_order,index)); */
    /* } */

    float getP(){
      auto x= getFloat(px_order,_entry);
      auto y= getFloat(py_order,_entry);
      auto z= getFloat(pz_order,_entry);
      return sqrt(x*x+y*y+z*z);
    }
    
    void setEntry(short i){ _entry=i;}
    short getEntry() const {return _entry;}
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

    int pid_order;
    int px_order;
    int py_order;
    int pz_order;
    int vx_order;
    int vy_order;
    int vz_order;
    int mass_order;
    
    short _entry=0;

  };
  
  using mcpar_ptr=std::shared_ptr<clas12::mcparticle>;

}

#endif /* UTILS_H */
