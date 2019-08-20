/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ftparticle.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef FTPARTICLE_H
#define FTPARTICLE_H

#include "bank.h"
#include "vectors.h"
#include "dictionary.h"
#include <memory>

namespace clas12 {

  class ftparticle : public hipo::bank {

  private:

    int _pid_order;
    int _beta_order;
    int _st_order;
    int _chi2pd_order;
    
    short _entry=0;
    
  public:

    ftparticle() = default;
    
    
    ftparticle(hipo::dictionary __factory);
    ftparticle(hipo::schema __schema);
    
    virtual ~ftparticle() = default;
    
    
    //    void   init(const char *bankName, hipo::reader &r);
    int    getPid(int index) { return getInt(_pid_order,index);}
    float  getBeta(int index)  { return getFloat(_beta_order,index);}
    float  getChi2Pid(int index)  { return getFloat(_chi2pid_order,index);}
    int    getStatus(int index)  { return getShort(_st_order,index);}

    int    getPid() { return getInt(_pid_order,_entry);}
    float  getBeta()  { return getFloat(_beta_order,_entry);}
    float  getChi2Pid()  { return getFloat(_chi2pid_order,_entry);}
    int    getStatus()  { return getShort(_st_order,_entry);}

 
    
    void setEntry(short i){ _entry=i;}
    short getEntry() const {return _entry;}
    /**
    * This is virtual method from hipo::bank it will be called
    * every time a bank is read in the reader. Can be used to sort
    * ftparticles and or map ftparticles by pid or type (i.e. charge)
    */
    void notify() final {
      bank::notify();
      //printf("ftparticle class is read again\n");
    }
  };
  
  using ftpar_ptr=std::shared_ptr<clas12::ftparticle>;

}

#endif /* UTILS_H */
