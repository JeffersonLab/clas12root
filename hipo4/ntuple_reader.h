#pragma once

#include "reader.h"
#include "bank.h"
#include "ntuple_writer.h"

namespace hipo {

  using std::string;
  using get_func = put_func;
  using get_funcs=std::vector<get_func>;

  class ntuple_reader {

  public: 

    ntuple_reader()=default;
    ntuple_reader(string filename);
    
    virtual ~ntuple_reader()=default;

    bool next();
    //hipo::bank *data() const noexcept{return _bank.get();}
    bool  loadIndex(int index);
    
    bool initBank(string name);
    void linkItemGetter(string bankName, string itemName, void* addr);
    
    int32_t& getInt(string bankName,string itemName);
    int16_t& getShort(string bankName,string itemName);
    int8_t& getByte(string bankName,string itemName);
    int64_t& getLong(string bankName,string itemName);
    float& getFloat(string bankName,string itemName);
    double& getDouble(string bankName,string itemName);

    int getBankRows(int i)const {return _banks[i]->getRows();}
    
    void show(const string& bank){
      auto sch=_dict.getSchema(bank.data());
      sch.show();
    }
  private:
     hipo::reader  _reader;
     hipo::dictionary  _dict;
     hipo::event _event;
     
     std::vector<hipo::schema> _schemas;//!
     //    schema_uptrs _schemas;//!
     bank_uptrs _banks;//!
     std::vector<string> _bankNames;
     
     get_funcs _getters{6}; //6 types allowed in schema, see typeToPos

     std::vector<data_addrs_to_func> _itemLinksAndGets;//!
     
     std::vector<std::vector<int_uptr > > _itemInts;//!
     std::vector<std::vector<short_uptr > > _itemShorts;//!
     std::vector<std::vector<byte_uptr > > _itemBytes;//!
     std::vector<std::vector<long_uptr > > _itemLongs;//!
     std::vector<std::vector<float_uptr > > _itemFloats;//!
     std::vector<std::vector<double_uptr > > _itemDoubles;//!
     
     uint _bankNumber{0}; //unique number associated with each bank

     std::map<string, uint> _schemaID;//!
     const std::map<int,int> _typeToPos{{3,0},{2,1},{1,2},{8,3},{4,4},{5,5}};
  };

}
