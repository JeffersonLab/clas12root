#include "ntuple_reader.h"
#include <algorithm>

namespace hipo{
  using std::cout;
  using std::endl;

  ntuple_reader::ntuple_reader(string name){
    _reader.open(name.data());
    _reader.readDictionary(_dict);
    //_dict.show();
    
    //create getters functions
    _getters[0]=[](hipo::bank* b,void *addr,int item, int index){
      *static_cast<int32_t*>(addr)=b->getInt(item,index);};
    _getters[1]=[](hipo::bank* b,void *addr,int item, int index){
      *static_cast<int16_t*>(addr)=b->getShort(item,index);};
    _getters[2]=[](hipo::bank* b,void *addr,int item, int index){
      *static_cast<int8_t*>(addr)=b->getByte(item,index);};
    _getters[3]=[](hipo::bank* b,void *addr,int item, int index){
      *static_cast<int64_t*>(addr)=b->getLong(item,index);};
    _getters[4]=[](hipo::bank* b,void *addr,int item, int index){
      *static_cast<float*>(addr)=b->getFloat(item,index);};
    _getters[5]=[](hipo::bank* b,void *addr,int item, int index){
      *static_cast<double*>(addr)=b->getDouble(item,index);};

  }
  bool ntuple_reader::initBank(string name){
    if(_dict.hasSchema(name.data())==false){
      std::cout<<"Error hipo file does not contain bank "<<name<<endl;
      _dict.show();
      return false;
    }
    _schemaID[name]=_bankNumber++;
    auto sch=_dict.getSchema(name.data());
 
    bank_uptr bank{new hipo::bank{sch}};
    _bankNames.push_back(name);
    _itemLinksAndGets.push_back(data_addrs_to_func(sch.getEntries()));
    _banks.push_back(std::move(bank) );
    _schemas.push_back(sch);
    //create data pointers
    _itemInts.push_back(std::vector<int_uptr>() );
    _itemShorts.push_back(std::vector<short_uptr>() );
    _itemBytes.push_back(std::vector<byte_uptr>() );
    _itemLongs.push_back(std::vector<long_uptr>() );
    _itemFloats.push_back(std::vector<float_uptr>() );
    _itemDoubles.push_back(std::vector<double_uptr>() );

    return true;
  }
  ///////////////////////////////////////////////////////////////////////////
  int32_t& ntuple_reader::getInt(string bankName,string itemName){
   
    if((std::find(_bankNames.begin(), _bankNames.end(), bankName)) == _bankNames.end())
      if(initBank(bankName)==false) {exit(0);}

    auto ptr = int_uptr(new int32_t);
    auto raw = ptr.get();
    auto isch=_schemaID[bankName];

    //Check item exists
    auto& sch = _schemas[isch];
    if(sch.exists(itemName.data())==false){
      std::cerr<<"Error item "<<itemName<<" does not exist in bank "<<bankName<<std::endl;
      exit(0);
    }
    linkItemGetter(bankName,itemName, raw);
    _itemInts[isch].push_back(std::move(ptr));//add address for this bank
    return  *raw;
  }
 ///////////////////////////////////////////////////////////////////////////
  int16_t& ntuple_reader::getShort(string bankName,string itemName){
   
    if((std::find(_bankNames.begin(), _bankNames.end(), bankName)) == _bankNames.end())
      if(initBank(bankName)==false) {exit(0);}

    auto ptr = short_uptr(new int16_t);
    auto raw = ptr.get();
    auto isch=_schemaID[bankName];

    //Check item exists
    auto& sch = _schemas[isch];
    if(sch.exists(itemName.data())==false){
      std::cerr<<"Error item "<<itemName<<" does not exist in bank "<<bankName<<std::endl;
      exit(0);
    }
    linkItemGetter(bankName,itemName, raw);
    _itemShorts[isch].push_back(std::move(ptr));//add address for this bank
    return  *raw;
  }
   ///////////////////////////////////////////////////////////////////////////
  int8_t& ntuple_reader::getByte(string bankName,string itemName){
   
    if((std::find(_bankNames.begin(), _bankNames.end(), bankName)) == _bankNames.end())
      if(initBank(bankName)==false) {exit(0);}

    auto ptr = byte_uptr(new int8_t);
    auto raw = ptr.get();
    auto isch=_schemaID[bankName];

    //Check item exists
    auto& sch = _schemas[isch];
    if(sch.exists(itemName.data())==false){
      std::cerr<<"Error item "<<itemName<<" does not exist in bank "<<bankName<<std::endl;
      exit(0);
    }
    linkItemGetter(bankName,itemName, raw);
    _itemBytes[isch].push_back(std::move(ptr));//add address for this bank
    return  *raw;
  }
 ///////////////////////////////////////////////////////////////////////////
  int64_t& ntuple_reader::getLong(string bankName,string itemName){
   
    if((std::find(_bankNames.begin(), _bankNames.end(), bankName)) == _bankNames.end())
      if(initBank(bankName)==false) {exit(0);}

    auto ptr = long_uptr(new int64_t);
    auto raw = ptr.get();
    auto isch=_schemaID[bankName];

    //Check item exists
    auto& sch = _schemas[isch];
    if(sch.exists(itemName.data())==false){
      std::cerr<<"Error item "<<itemName<<" does not exist in bank "<<bankName<<std::endl;
      exit(0);
    }
    linkItemGetter(bankName,itemName, raw);
    _itemLongs[isch].push_back(std::move(ptr));//add address for this bank
    return  *raw;
  }
 ///////////////////////////////////////////////////////////////////////////
  float& ntuple_reader::getFloat(string bankName,string itemName){
   
    if((std::find(_bankNames.begin(), _bankNames.end(), bankName)) == _bankNames.end())
      if(initBank(bankName)==false) {exit(0);}

    auto ptr = float_uptr(new float);
    auto raw = ptr.get();
    auto isch=_schemaID[bankName];

    //Check item exists
    auto& sch = _schemas[isch];
    if(sch.exists(itemName.data())==false){
      std::cerr<<"Error item "<<itemName<<" does not exist in bank "<<bankName<<std::endl;
      exit(0);
    }
    linkItemGetter(bankName,itemName, raw);
    _itemFloats[isch].push_back(std::move(ptr));//add address for this bank
    return  *raw;
  }
 ///////////////////////////////////////////////////////////////////////////
  double& ntuple_reader::getDouble(string bankName,string itemName){
   
    if((std::find(_bankNames.begin(), _bankNames.end(), bankName)) == _bankNames.end())
      if(initBank(bankName)==false) {exit(0);}

    auto ptr = double_uptr(new double);
    auto raw = ptr.get();
    auto isch=_schemaID[bankName];

    //Check item exists
    auto& sch = _schemas[isch];
    if(sch.exists(itemName.data())==false){
      std::cerr<<"Error item "<<itemName<<" does not exist in bank "<<bankName<<std::endl;
      exit(0);
    }
    linkItemGetter(bankName,itemName, raw);
    _itemDoubles[isch].push_back(std::move(ptr));//add address for this bank
    return  *raw;
  }

  ///////////////////////////////////////////////////////////////////////
  void ntuple_reader::linkItemGetter(string bankName, string itemName, void* addr){
    auto isch = _schemaID[bankName];
    auto& sch = _schemas[isch];
    auto order = sch.getEntryOrder(itemName.data());
    auto type = _typeToPos.at(sch.getEntryType(order));

    std::cout<<itemName<<" Linking address "<<addr<<" to getter type "<< type <<" at "<<order<<" size reserved "<<_itemLinksAndGets[isch].size()<<std::endl;

    //Note entry order == position in  _itemLinksAndGets[isch]
    _itemLinksAndGets[isch].at(order).reset(new addrs_to_func(_getters[type],addr));
  }
  //////////////////////////////////////////////////////////////////////////
  bool  ntuple_reader::next(){
    auto ismore=_reader.next();
    if(ismore==false) return false;
    _reader.read(_event);

    auto ibank=0;
    for(auto& bank: _banks){
      auto* rawBank=bank.get();
      _event.getStructure(*rawBank);
      auto pos=0;
      for(auto& itemFuncAndAddr :_itemLinksAndGets[ibank]){
	if(itemFuncAndAddr.get()!=nullptr)
	  itemFuncAndAddr->first(rawBank,itemFuncAndAddr->second,pos++,0);
      }
      ++ibank;
    }

	
    return true;
  }


}
