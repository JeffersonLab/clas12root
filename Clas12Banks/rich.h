
/*
 * File:   rich.h
 * Author: dglazier
 *
 * Created on March 02, 2023
 */

#ifndef CLAS12_RICH_H
#define CLAS12_RICH_H

#include "particle_detector.h"


namespace clas12 {
  
  class rich;
 
  //sub class for RICH::Ring banks
  class richring : public  hipo::bank {

    
  public:


    richring()=default;

    richring(hipo::schema aschema);

    ~richring() override=default;
    
    void setBankEntry(short i){ _index=i;} //faster for BankHist
    //getter funtions for items in rich bank
 
    int getId() const noexcept{ 
      if(_index>-1)return getShort(_id_order,_index);
      return 0;
    }
    int getHindex() const noexcept{ 
      if(_index>-1)return getShort(_hindex_order,_index);
      return 0;
    }
    int getPindex() const noexcept{ 
      if(_index>-1)return getByte(_pindex_order,_index);
      return 0;
    }
    int getSector() const noexcept{ 
      if(_index>-1)return getByte(_sector_order,_index);
      return 0;
    }
    int getPmt() const noexcept{ 
      if(_index>-1)return getShort(_pmt_order,_index);
      return 0;
    }
    int getAnode() const noexcept{ 
      if(_index>-1)return getByte(_anode_order,_index);
      return 0;
    }
    int getHypo() const noexcept{ 
      if(_index>-1)return getInt(_hypo_order,_index);
      return 0;
    }
    double getDtime() const noexcept{ 
      if(_index>-1)return getFloat(_dtime_order,_index);
      return 0;
    }
    double getEtaC() const noexcept{ 
      if(_index>-1)return getFloat(_etaC_order,_index);
      return 0;
    }
    double getProb() const noexcept{ 
      if(_index>-1)return getFloat(_prob_order,_index);
      return 0;
    }
    int getUse() const noexcept{ 
      if(_index>-1)return getByte(_use_order,_index);
      return 0;
    }
    double getDangle() const noexcept{ 
      if(_index>-1)return getFloat(_dangle_order,_index);
      return 0;
    }
    int getCompos() const noexcept{ 
      if(_index>-1)return getInt(_compos_order,_index);
      return 0;
    }
    int getLayers() const noexcept{ 
      if(_index>-1)return getInt(_layers_order,_index);
      return 0;
    }

 
  private:
     
    friend clas12::rich;
    
    void setIndex(short i){ _index=i;}
 
 
    int    _id_order=-1;
    int    _hindex_order=-1;
    int    _pindex_order=-1;
    int    _sector_order=-1;
    int    _pmt_order=-1;
    int    _anode_order=-1;
    int    _hypo_order=-1;
    int    _dtime_order=-1;
    int    _etaC_order=-1;
    int    _prob_order=-1;
    int    _use_order=-1;
    int    _dangle_order=-1;
    int    _layers_order=-1;
    int    _compos_order=-1;
 
    int _index=-1;
 
  };
  
  using richring_ptr=clas12::richring*;
  using richring_uptr=std::unique_ptr<clas12::richring>;
  ///////////////////////////////////////////////////////////
  
  class rich : public particle_detector {
    
 
  public:


    rich()=default;

    rich(hipo::schema __schema,hipo::schema ringschema);
    rich(hipo::schema __schema);
    
    ~rich() override=default;
    
 
    int getId() const noexcept{ 
      if(_index>-1)return getByte(_id_order,_index);
      return 0;
    }
    int getHindex() const noexcept{ 
      if(_index>-1)return getShort(_hindex_order,_index);
      return 0;
    }
    int getEmilay() const noexcept{
      if(_index>-1)return getByte(_emilay_order,_index);
      return 0;
    }
    int getEmico() const noexcept{ 
      if(_index>-1)return getByte(_emico_order,_index);
      return 0;
    }
    int getEnico() const noexcept{ 
      if(_index>-1)return getByte(_enico_order,_index);
      return 0;
    }
    int getEmqua() const noexcept{ 
      if(_index>-1)return getShort(_emqua_order,_index);
      return 0;
    }
    double getMchi2() const noexcept{ 
      if(_index>-1)return getFloat(_mchi2_order,_index);
      return 0;
    }
    double getMass() const noexcept{ 
      if(_index>-1)return getFloat(_mass_order,_index);
      return 0;
    }
    int getBest_PID() const noexcept{ 
      if(_index>-1)return getShort(_best_PID_order,_index);
      return 0;
    }
    double getRQ() const noexcept{ 
      if(_index>-1)return getFloat(_RQ_order,_index);
      return 0;
    }
    double getReQ() const noexcept{ 
      if(_index>-1)return getFloat(_ReQ_order,_index);
      return 0;
    }
    double getEl_logl() const noexcept{ 
      if(_index>-1)return getFloat(_el_logl_order,_index);
      return 0;
    }
    double getPi_logl() const noexcept{ 
      if(_index>-1)return getFloat(_pi_logl_order,_index);
      return 0;
    }
    double getK_logl() const noexcept{ 
      if(_index>-1)return getFloat(_k_logl_order,_index);
      return 0;
    }
    double getPr_logl() const noexcept{ 
      if(_index>-1)return getFloat(_pr_logl_order,_index);
      return 0;
    }
    double getBest_ch() const noexcept{ 
      if(_index>-1)return getFloat(_best_ch_order,_index);
      return 0;
    }
    double getBest_c2() const noexcept{ 
      if(_index>-1)return getFloat(_best_c2_order,_index);
      return 0;
    }
    double getBest_RL() const noexcept{ 
      if(_index>-1)return getFloat(_best_RL_order,_index);
      return 0;
    }
    double getBest_ntot() const noexcept{ 
      if(_index>-1)return getFloat(_best_ntot_order,_index);
      return 0;
    }
    double getBest_mass() const noexcept{ 
      if(_index>-1)return getFloat(_best_mass_order,_index);
      return 0;
    }
 

 

    int getDetector(int index)   const noexcept final{ return clas12::RICH;} //if the bank does not have a detector entry, you may inherit and chenge this getter.
    int getDetector()   const noexcept final{ return clas12::RICH;}

  //RICH::Ring has many entries for each REC::Particle
    richring_ptr const ring(uint index=0) {_ring->setIndex(index);return _ring.get();}
  uint getNRing() const {return _ring->getRows();}
  private:

    int  _id_order=-1;
    int  _hindex_order=-1;
    int  _emilay_order=-1;
    int  _emico_order=-1;
    int  _enico_order=-1;
    int  _emqua_order=-1;
    int  _mchi2_order=-1;
    int  _mass_order=-1;
    int  _best_PID_order=-1;
    int  _RQ_order=-1;
    int  _ReQ_order=-1;
    int  _el_logl_order=-1;
    int  _pi_logl_order=-1;
    int  _k_logl_order=-1;
    int  _pr_logl_order=-1;
    int  _best_ch_order=-1;
    int  _best_c2_order=-1;
    int  _best_RL_order=-1;
    int  _best_ntot_order=-1;
    int  _best_mass_order=-1;
   
    richring_uptr _ring;

 
  }; //class rich

  using rich_ptr=clas12::rich*;
  using rich_uptr=std::unique_ptr<clas12::rich>;

}//namespace clas12

#endif /* UTILS_H */
