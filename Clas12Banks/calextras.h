
/*
 * File:   calextras.h
 * Author: dglazier
 *
 * Created on Feb 16, 2023
 */

#ifndef CLAS12_CALEXTRAS_H
#define CLAS12_CALEXTRAS_H

#include <hipo4/bank.h>
#include <memory>


namespace clas12 {

  class calorimeter;
  
   class calextras : public  hipo::bank {

 
  public:


    calextras()=default;

    calextras(hipo::schema __schema);

    ~calextras() override=default;
    

   int getDbstU() const noexcept{ 
      if(_index>-1)return getShort(_dbstU_order,_index);
      return 0;
    }
   int getDbstV() const noexcept{ 
      if(_index>-1)return getShort(_dbstV_order,_index);
      return 0;
    }
   int getDbstW() const noexcept{ 
      if(_index>-1)return getShort(_dbstW_order,_index);
      return 0;
    }
  
  double getRawEU() const noexcept{ 
      if(_index>-1)return getFloat(_rawEU_order,_index);
      return 0.;
    }
     double getRawEV() const noexcept{ 
      if(_index>-1)return getFloat(_rawEV_order,_index);
      return 0.;
    }
    double getRawEW() const noexcept{ 
      if(_index>-1)return getFloat(_rawEW_order,_index);
      return 0.;
    }
    double getRecEU() const noexcept{ 
      if(_index>-1)return getFloat(_recEU_order,_index);
      return 0.;
    }
    double getRecEV() const noexcept{ 
      if(_index>-1)return getFloat(_recEV_order,_index);
      return 0.;
    }
    double getRecEW() const noexcept{ 
      if(_index>-1)return getFloat(_recEW_order,_index);
      return 0.;
    }
    double getRecDTU() const noexcept{ 
      if(_index>-1)return getFloat(_recDTU_order,_index);
      return 0.;
    }
    double getRecDTV() const noexcept{ 
      if(_index>-1)return getFloat(_recDTV_order,_index);
      return 0.;
    }
    double getRecDTW() const noexcept{ 
      if(_index>-1)return getFloat(_recDTW_order,_index);
      return 0.;
    }
    double getRecFTU() const noexcept{ 
      if(_index>-1)return getFloat(_recFTU_order,_index);
      return 0.;
    }
    double getRecFTV() const noexcept{ 
      if(_index>-1)return getFloat(_recFTV_order,_index);
      return 0.;
    }
   double getRecFTW() const noexcept{ 
      if(_index>-1)return getFloat(_recFTW_order,_index);
      return 0.;
    }

  int getDbstU(int index) const noexcept{
    if(index>-1)return getShort(_dbstU_order,index);
      return 0;
    }
   int getDbstV(int index) const noexcept{ 
      if(index>-1)return getShort(_dbstV_order,index);
      return 0;
    }
   int getDbstW(int index) const noexcept{ 
      if(index>-1)return getShort(_dbstW_order,index);
      return 0;
    }
  
  double getRawEU(int index) const noexcept{ 
      if(index>-1)return getFloat(_rawEU_order, index);
      return 0.;
    }
     double getRawEV(int index) const noexcept{ 
      if(index>-1)return getFloat(_rawEV_order,index);
      return 0.;
    }
    double getRawEW(int index) const noexcept{ 
      if(index>-1)return getFloat(_rawEW_order,index);
      return 0.;
    }
    double getRecEU(int index) const noexcept{ 
      if(index>-1)return getFloat(_recEU_order,index);
      return 0.;
    }
    double getRecEV(int index) const noexcept{ 
      if(index>-1)return getFloat(_recEV_order,index);
      return 0.;
    }
    double getRecEW(int index) const noexcept{ 
      if(index>-1)return getFloat(_recEW_order,index);
      return 0.;
    }
    double getRecDTU(int index) const noexcept{ 
      if(index>-1)return getFloat(_recDTU_order,index);
      return 0.;
    }
    double getRecDTV(int index) const noexcept{ 
      if(index>-1)return getFloat(_recDTV_order,index);
      return 0.;
    }
    double getRecDTW(int index) const noexcept{ 
      if(index>-1)return getFloat(_recDTW_order,index);
      return 0.;
    }
    double getRecFTU(int index) const noexcept{ 
      if(index>-1)return getFloat(_recFTU_order,index);
      return 0.;
    }
    double getRecFTV(int index) const noexcept{ 
      if(index>-1)return getFloat(_recFTV_order,index);
      return 0.;
    }
   double getRecFTW(int index) const noexcept{ 
      if(index>-1)return getFloat(_recFTW_order,index);
      return 0.;
    }
  
  
  
    void setBankEntry(short i){ _index=i;} //faster for BankHist

 private:
     friend calorimeter;
    
     void setIndex(short i){ _index=i;}
 
 
     int    _dbstU_order=-1;
     int    _dbstV_order=-1;
     int    _dbstW_order=-1;
     int    _rawEU_order=-1;
     int    _rawEV_order=-1;
     int    _rawEW_order=-1;
     int    _recEU_order=-1;
     int    _recEV_order=-1;
     int    _recEW_order=-1;
     int    _recDTU_order=-1;
     int    _recDTV_order=-1;
     int    _recDTW_order=-1;
     int    _recFTU_order=-1;
     int    _recFTV_order=-1;
     int    _recFTW_order=-1;
     
     int _index=-1;
 
   }; //class calextras

   using calextra_ptr=clas12::calextras*;
   using calextra_uptr=std::unique_ptr<clas12::calextras>;

}//namespace clas12

#endif /* UTILS_H */
