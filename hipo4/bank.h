//******************************************************************************
//*       ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗                  *
//*       ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗                 *
//*       ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║                 *
//*       ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║                 *
//*       ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝                 *
//*       ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝                  *
//************************ Jefferson National Lab (2017) ***********************
/*
 *   Copyright (c) 2017.  Jefferson Lab (JLab). All rights reserved. Permission
 *   to use, copy, modify, and distribute  this software and its documentation
 *   for educational, research, and not-for-profit purposes, without fee and
 *   without a signed licensing agreement.
 *
 *   IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL
 *   INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 *   OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS
 *   BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *   JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. THE HIPO DATA FORMAT SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
 *   ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO
 *   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 *   This software was developed under the United States Government license.
 *   For more information contact author at gavalian@jlab.org
 *   Department of Experimental Nuclear Physics, Jefferson Lab.
 */
/*******************************************************************************
 * File:   bank.h
 * Author: gavalian
 *
 * Created on April 12, 2017, 10:14 AM
 */

#ifndef HIPO_BANK_H
#define HIPO_BANK_H
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include "dictionary.h"

namespace hipo {

  class structure {

    private:

      std::vector<char> structureBuffer;
      char *structureAddress{};
      void setAddress(const char *address);

    protected:
      void initStructureBySize(int __group, int __item, int __type, int __size);
      std::vector<char>  &getStructureBuffer(){ return structureBuffer;}
      int                 getStructureBufferSize(){ return 8+getSize();}
    public:

      structure(){ structureAddress = nullptr;}
      structure(int size){ allocate(size);}
      structure(int __group, int __item, std::string &str);

      virtual     ~structure()= default;
      bool         allocate(int size);
      int          getSize() const noexcept{
	         return *reinterpret_cast<uint32_t *>(structureAddress+4);
      }
      int          getType();
      int          getGroup();
      int          getItem();
      void         init(const char *buffer, int size);
      void         initNoCopy(const char *buffer, int size);

      const char  *getAddress();
      virtual void  show();
      void         setSize(int size);


      int          getIntAt   ( int index) const noexcept {
        return *reinterpret_cast<int32_t*>(&structureAddress[index+8]);
      }
      int16_t      getShortAt ( int index) const noexcept {
        return *reinterpret_cast<int16_t*>(&structureAddress[index+8]);
      }
      int8_t       getByteAt  ( int index) const noexcept {
        return *reinterpret_cast<int8_t*>(&structureAddress[index+8]);
      }
      float        getFloatAt ( int index) const noexcept {
        return *reinterpret_cast<float*>(&structureAddress[index+8]);
      }
      double       getDoubleAt( int index) const noexcept {
        return *reinterpret_cast<double*>(&structureAddress[index+8]);
      }
      long         getLongAt  ( int index) const noexcept {
        return *reinterpret_cast<int64_t*>(&structureAddress[index+8]);
      }

      std::string  getStringAt(int index);

      void         putIntAt(int index, int value){
        *reinterpret_cast<int32_t*>(&structureAddress[index+8]) = value;
      }

      void         putShortAt(int index, int16_t value){
        *reinterpret_cast<int16_t*>(&structureAddress[index+8]) = value;
      }

      void         putByteAt(int index, int8_t value){
        *reinterpret_cast<int8_t*>(&structureAddress[index+8]) = value;
      }

      void         putFloatAt(int index, float value){
        *reinterpret_cast<float*>(&structureAddress[index+8]) = value;
      }

      void         putDoubleAt(int index, double value){
        *reinterpret_cast<double*>(&structureAddress[index+8]) = value;
      }

      void         putLongAt(int index, int64_t value){
        *reinterpret_cast<int64_t*>(&structureAddress[index+8]) = value;
      }

      void         putStringAt(int index, std::string &str);

      virtual void notify(){}
      friend class event;
  };

  //typedef std::auto_ptr<hipo::generic_node> node_pointer;

    class bank : public hipo::structure {

    private:

      hipo::schema  bankSchema;
      int           bankRows{-1};

    protected:
        void setBankRows(int rows){ bankRows = rows;}

    public:

        bank();
        // constructor initializes the nodes in the bank
        // and they will be filled automatically by reader.next()
        // method.
        bank(const hipo::schema& __schema){
          bankSchema = __schema;
          bankRows   = -1;
        }

        bank(const hipo::schema& __schema, int __rows){
          bankSchema = __schema;
          bankRows   = __rows;
          int size   = bankSchema.getSizeForRows(__rows);
          initStructureBySize(bankSchema.getGroup(),bankSchema.getItem(), 11, size);
        }

        ~bank() override;
        // display the content of the bank
        //void show();

        hipo::schema  &getSchema() { return bankSchema;}

        int    getRows()  const noexcept{ return bankRows;}
        void   setRows(   int rows);
        int    getInt(    int item, int index) const noexcept;
        int    getShort(  int item, int index) const noexcept;
        int    getByte(   int item, int index) const noexcept;
        float  getFloat(  int item, int index) const noexcept;
        double getDouble( int item, int index) const noexcept;
        long   getLong(   int item, int index) const noexcept;

        int    getInt(    const char *name, int index) const noexcept;
        int    getShort(  const char *name, int index) const noexcept;
        int    getByte(   const char *name, int index) const noexcept;
        float  getFloat(  const char *name, int index) const noexcept;
        double getDouble( const char *name, int index) const noexcept;
        long   getLong(   const char *name, int index) const noexcept;

        void    putInt(    const char *name, int index, int32_t value);
        void    putShort(  const char *name, int index, int16_t value);
        void    putByte(   const char *name, int index, int8_t value);
        void    putFloat(  const char *name, int index, float value);
        void    putDouble( const char *name, int index, double value);
        void    putLong(   const char *name, int index, int64_t value);

 	void    putInt(int item, int index, int32_t value);
        void    putShort(int item, int index, int16_t value);
        void    putByte(int item, int index, int8_t value);
        void    putFloat(int item, int index, float value);
        void    putDouble(int item, int index, double value);
        void    putLong(int item, int index, int64_t value);
     
        void    show() override;
        void    reset();
        //virtual  void notify(){ };

        void notify() override;



  };
    /////////////////////////////////////
    //inlined getters

    inline float  bank::getFloat(int item, int index) const noexcept{
      if(bankSchema.getEntryType(item)==4){
	       int offset = bankSchema.getOffset(item, index, bankRows);
         return getFloatAt(offset);
      }
      return 0.0;
    }

    inline double  bank::getDouble(int item, int index) const noexcept{
        if(bankSchema.getEntryType(item)==5){
	         int offset = bankSchema.getOffset(item, index, bankRows);
	         return getDoubleAt(offset);
      }
      return 0.0;
    }

    inline long bank::getLong(int item, int index) const noexcept{
      if(bankSchema.getEntryType(item)==8){
	       int offset = bankSchema.getOffset(item, index, bankRows);
	        return getLongAt(offset);
      }
      return 0;
    }

    inline int    bank::getInt(int item, int index) const noexcept{
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case 1: return (int) getByteAt(offset);
      case 2: return (int) getShortAt(offset);
      case 3: return getIntAt(offset);
      default: printf("---> error : requested INT for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }

    inline int    bank::getShort(int item, int index) const noexcept{
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case 1: return (int) getByteAt(offset);
      case 2: return (int) getShortAt(offset);
      default: printf("---> error : requested SHORT for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }

    inline int    bank::getByte(int item, int index) const noexcept{
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case 1: return (int) getByteAt(offset);
      default: printf("---> error : requested BYTE for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }
    inline int    bank::getInt(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case 1: return (int) getByteAt(offset);
      case 2: return (int) getShortAt(offset);
      case 3: return getIntAt(offset);
      default: printf("---> error : requested INT for [%s] type = %d\n",name,type); break;
      }
      return 0;
    }

    inline int    bank::getShort(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case 1: return (int) getByteAt(offset);
      case 2: return (int) getShortAt(offset);
      default: printf("---> error : requested SHORT for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }
    inline int    bank::getByte(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case 1: return (int) getByteAt(offset);
      default: printf("---> error : requested BYTE for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }

    inline float  bank::getFloat(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      if(bankSchema.getEntryType(item)==4){
	int offset = bankSchema.getOffset(item, index, bankRows);
	return getFloatAt(offset);
      }
      return 0.0;
    }

    inline double  bank::getDouble(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      if(bankSchema.getEntryType(item)==5){
	int offset = bankSchema.getOffset(item, index, bankRows);
	return getDoubleAt(offset);
      }
      return 0.0;
    }

    inline long bank::getLong(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      if(bankSchema.getEntryType(item)==8){
	int offset = bankSchema.getOffset(item, index, bankRows);
	return getLongAt(offset);
      }
      return 0;
    }
  inline void    bank::putInt(int item, int index, int32_t value){
    //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putIntAt(offset,value);
    }
    inline void    bank::putShort(int item, int index, int16_t value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putShortAt(offset,value);
    }
    inline void    bank::putByte(int item, int index, int8_t value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putByteAt(offset,value);
    }
    inline  void    bank::putFloat(int item, int index, float value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      //printf("---- put float %f at position = %d\n",value,offset);
      putFloatAt(offset,value);
    }
    inline void    bank::putDouble(int item, int index, double value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putDoubleAt(offset,value);
    }
    inline void    bank::putLong(int item, int index, int64_t value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putLongAt(offset,value);
    }
}
#endif /* BANK_H */
