/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   particle_detector.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef PARTICLE_DETECTOR_H
#define PARTICLE_DETECTOR_H

#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <memory>
#include "clas12defs.h"
#include <hipo4/bank.h>


namespace clas12 {

  class particle_detector : public hipo::bank {


  public:


    particle_detector()=default;

    particle_detector(hipo::schema __schema);

    ~particle_detector() override=default;

    ///////////////////////////////////////////////////////////////
    //utiltiy functions, same for derived classes
    void   scanIndex();
    std::vector<short >  scanForParticle(short iparticle);
    void readItems(const std::string& items);
    bool useItem(const std::string& item);
    
    ///////////////////////////////////////////////////////////////
    //getter functions, same for derived classes
    //get detector id
    virtual int getDetector(int index)   const noexcept{ return getByte(_detector_id_order,index);} //if the bank does not have a detector entry, you may inherit and chenge this getter.
    virtual int getDetector()   const noexcept{ return getByte(_detector_id_order,_index);}
    //get the pindex = corresponding particle index
    int getPindex(int index)   const noexcept{ return getShort(_pindex_order,index);}
    int getPindex()   const noexcept{ return getShort(_pindex_order,_index);}

    //given a detector (layer if exists)  and pindex find the position in this bank
    int getIndex()   const noexcept{return _index;}
    int getIndex(int pindex, int detector=0, int layer=0);
   
    virtual void setIndex(int ind){_index=ind;}
    void setBankEntry(short i){ _index=i;} //faster for BankHist
    void setEntry(int ind){_index=ind;}
    
    ////////////////////////////////////////////////////////////////
    //virtual functions can be overridden in derived class
    virtual int getLayer(int index)  const noexcept{return 0;}
    virtual int getLayer()  const noexcept{return 0;}
    virtual void print();

    ////////////////////////////////////////////////////////////////
    //override header notify, called at start of event
    void notify() override {
      if(_detector_id_order==-1) return;
      bank::notify();
      scanIndex();
    }


  protected:

    int _detector_id_order=-1;
    int  _pindex_order=-1;
    int _index=-1;
    
    // std::map<int,int> _rmap;
    std::vector<int> _rvec{};
    std::vector<std::string > _sitems{};

    
  };

}

#endif /* PARTICLE_DETECTOR_H */
