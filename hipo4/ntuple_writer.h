#pragma once

#include <hipo4/writer.h>
#include <hipo4/bank.h>

#include <vector>
#include <string>

namespace hipo {

  using std::string;
  
  using schema_uptr =  std::unique_ptr<hipo::schema>;
  using schema_uptrs =  std::vector<schema_uptr >;
  using bank_uptr = std::unique_ptr<hipo::bank>;
  using bank_uptrs = std::vector<bank_uptr >;
  
  using data_addrs = std::vector<void* >;
  
  using put_func = void (*)(hipo::bank*,void *,int, int);//!
  using put_funcs=std::vector<hipo::put_func>;//!
  using addrs_to_func = std::pair< hipo::put_func, void* >;//!
  using addrs_to_func_uptr = std::unique_ptr<hipo::addrs_to_func>;//!
  //using data_addrs_to_func = std::vector<addrs_to_func_uptr>; //root V6.20 DOES NOT LIKE THIS
  using data_addrs_to_func = std::vector<std::unique_ptr<std::pair< void (*)(hipo::bank*,void *,int, int), void* >>>;//!

  using int_uptr = std::unique_ptr<int32_t>;
  using short_uptr = std::unique_ptr<int16_t>;
  using byte_uptr = std::unique_ptr<int8_t>;
  using long_uptr = std::unique_ptr<int64_t>;
  using float_uptr = std::unique_ptr<float>;
  using double_uptr = std::unique_ptr<double>;

 
  class ntuple_writer {

  public: 

    ntuple_writer()=default;
    ntuple_writer(string filename);
    
    virtual ~ntuple_writer()=default;


    void bank(string bankName,string schemaStr, int group=100);  //add a bank
    void linkItemFunc(string bankName, string itemName, void* addr);

    
    int32_t& intItem(string bankName, string itemName);
    int16_t& shortItem(string bankName, string itemName);
    int8_t& byteItem(string bankName, string itemName);
    int64_t& longItem(string bankName, string itemName);
    float& floatItem(string bankName, string itemName);
    double& doubleItem(string bankName, string itemName);
    
    void fill();
    void clear(){
      _event.reset();
    }
    void initBank(int index,int length){
      _banks[index]->setRows(length);
    }
    void copyRow(int index);
    void fillRows();
    
    void open(){_writer.open(_filename.data());}
    void close(){_writer.close();}
    
  private :
    writer _writer;

    schema_uptrs _schemas;//!
    bank_uptrs _banks;//!

    hipo::event _event;
    
    // std::vector<data_addrs> _itemLinks;//!
    std::vector<data_addrs_to_func> _itemLinksAndPuts;//!
    
    put_funcs _putters{6}; //6 types allowed in schema, see typeToPos
    
    std::vector<std::vector<int_uptr > > _itemInts;//!
    std::vector<std::vector<short_uptr > > _itemShorts;//!
    std::vector<std::vector<byte_uptr > > _itemBytes;//!
    std::vector<std::vector<long_uptr > > _itemLongs;//!
    std::vector<std::vector<float_uptr > > _itemFloats;//!
    std::vector<std::vector<double_uptr > > _itemDoubles;//!
    
    uint _bankNumber{0}; //unique number associated with each bank

    string _filename;
    std::map<string, uint> _schemaID;//!
    
    //convert hipo shecma type to position :   int  ,short,byte ,long, float, dou
    const std::map<int,int> _typeToPos{{3,0},{2,1},{1,2},{8,3},{4,4},{5,5}};
  };
}
//needed to create dictionary in v6.20
//#pragma link C++ class hipo::data_addrs_to_func;
//#pragma link C++ class hipo::addrs_to_func_uptr;
// #pragma link C++ class hipo::addrs_to_func;
// #pragma link C++ class hipo::put_funcs;
//#pragma link C++ class hipo::put_func;

#pragma hipo::std::unique_ptr<pair<void(*)(hipo::bank*,void*,int,int),void*>;
