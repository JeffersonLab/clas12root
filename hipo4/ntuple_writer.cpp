#include "ntuple_writer.h"

namespace hipo{
  using std::cout;
  using std::endl;
  
  ///////////////////////////////////////////////////////////////////////
  ntuple_writer::ntuple_writer(string filename) : _filename(filename){
   //create data pointers
    _itemInts.push_back(std::vector<int_uptr>() );
    _itemShorts.push_back(std::vector<short_uptr>() );
    _itemBytes.push_back(std::vector<byte_uptr>() );
    _itemLongs.push_back(std::vector<long_uptr>() );
    _itemFloats.push_back(std::vector<float_uptr>() );
    _itemDoubles.push_back(std::vector<double_uptr>() );

    //create putter functions
    _putters[0]=[](hipo::bank* b,void *addr,int item, int index){
      b->putInt(item,index,*static_cast<int32_t*>(addr));};
    _putters[1]=[](hipo::bank* b,void *addr,int item, int index){
      b->putShort(item,index,*static_cast<int16_t*>(addr));};
    _putters[2]=[](hipo::bank* b,void *addr,int item, int index){
      b->putByte(item,index,*static_cast<int8_t*>(addr));};
    _putters[3]=[](hipo::bank* b,void *addr,int item, int index){
      b->putLong(item,index,*static_cast<int64_t*>(addr));};
    _putters[4]=[](hipo::bank* b,void *addr,int item, int index){
      b->putFloat(item,index,*static_cast<float*>(addr));};
    _putters[5]=[](hipo::bank* b,void *addr,int item, int index){
      b->putDouble(item,index,*static_cast<double*>(addr));};

  }
  ///////////////////////////////////////////////////////////////////////
  void ntuple_writer::bank(string bankName,string schemaStr, int group){
    _schemaID[bankName]=_bankNumber;
    std::cout<<"ntuple_writer::bank "<<_bankNumber<<" "<<schemaStr<<" "<<_schemaID[bankName]<<std::endl;
   
    auto sch=schema_uptr(new schema(bankName.data(),group,_bankNumber++));
    sch->parse(schemaStr.data());

 
    _writer.getDictionary().addSchema(*sch.get());
    
    auto bank = bank_uptr{new hipo::bank{_writer.getDictionary().getSchema(bankName.data()),1}}; //create a bank with always 1 row

    //add to collections
    _itemLinksAndPuts.push_back(data_addrs_to_func(sch->getEntries()));
    _schemas.push_back(std::move(sch));
    _banks.push_back(std::move(bank));
    
    _writer.getDictionary().show();

   }
  
 
  
  ///////////////////////////////////////////////////////////////////////
  int32_t& ntuple_writer::intItem(string bankName, string itemName){
    auto isch = _schemaID[bankName];
    auto ptr = int_uptr(new int32_t);
    auto raw = ptr.get();
    linkItemFunc(bankName,itemName, raw);
    _itemInts[isch].push_back(std::move(ptr));
    return  *raw;
  }

   ///////////////////////////////////////////////////////////////////////
  int16_t& ntuple_writer::shortItem(string bankName, string itemName){
    auto isch = _schemaID[bankName];
    auto ptr = short_uptr(new int16_t);
    auto raw = ptr.get();
    linkItemFunc(bankName,itemName, raw);
    _itemShorts[isch].push_back(std::move(ptr));
    return  *raw;
  }

  ///////////////////////////////////////////////////////////////////////
  int8_t& ntuple_writer::byteItem(string bankName, string itemName){
    auto isch = _schemaID[bankName];
    auto ptr = byte_uptr(new int8_t);
    auto raw = ptr.get();
    linkItemFunc(bankName,itemName, raw);
    _itemBytes[isch].push_back(std::move(ptr));
    return  *raw;
  }

  ///////////////////////////////////////////////////////////////////////
  int64_t& ntuple_writer::longItem(string bankName, string itemName){
    auto isch = _schemaID[bankName];
    auto ptr = long_uptr(new int64_t);
    auto raw = ptr.get();
    linkItemFunc(bankName,itemName, raw);
    _itemLongs[isch].push_back(std::move(ptr));
    return  *raw;
  }

  ///////////////////////////////////////////////////////////////////////
  float& ntuple_writer::floatItem(string bankName, string itemName){
    auto isch = _schemaID[bankName];
    auto ptr = float_uptr(new float);
    auto raw = ptr.get();
    linkItemFunc(bankName,itemName, raw);
    _itemFloats[isch].push_back(std::move(ptr));
    return  *raw;
  }
  
  ///////////////////////////////////////////////////////////////////////
  double& ntuple_writer::doubleItem(string bankName, string itemName){
    cout<<bankName<<" "<<itemName<<endl;
    auto isch = _schemaID[bankName];
    cout<<isch<<endl;
    auto ptr = double_uptr(new double);
    cout<<ptr.get()<<endl;
    auto raw = ptr.get();
    cout<<raw<<endl;
    linkItemFunc(bankName,itemName, raw);
    cout<<"Link Done "<<endl;
    cout<<_itemDoubles[isch].size()<<endl;
    _itemDoubles[isch].push_back(std::move(ptr));
    return  *raw;
  }

 
  ///////////////////////////////////////////////////////////////////////
  void ntuple_writer::linkItemFunc(string bankName, string itemName, void* addr){
    auto isch = _schemaID[bankName];
    auto sch = _schemas[isch].get();
    auto order = sch->getEntryOrder(itemName.data());
    auto type = _typeToPos.at(sch->getEntryType(order));

    std::cout<<itemName<<" Linking address "<<addr<<" to putter type "<< type <<" at "<<order<<" size reserved "<<_itemLinksAndPuts[isch].size()<<std::endl;
    _itemLinksAndPuts[isch].at(order).reset(new addrs_to_func(_putters[type],addr));
    cout<<"done link"<<endl;
  }
  ///////////////////////////////////////////////////////////////////////
  void ntuple_writer::fill(){
    _event.reset();

    uint ibank=0;
    
    //loop over each defined bank
    for(auto& bankItems : _itemLinksAndPuts){
       uint pos=0;
      auto bank = _banks[ibank++].get();

      //loop over items in the bank
      for(auto& itemFuncAndAddr : bankItems){ //get the right put type
	//itemFuncAndAddr.first is = bank->putInt(...) etc
	//itemFuncAndAddr.second is = address of item
      	if(itemFuncAndAddr.get()!=nullptr)itemFuncAndAddr->first(bank,itemFuncAndAddr->second,pos++,0);
     }//done loop over items
      
      _event.addStructure(*bank);
    }//done loop over banks
 
    _writer.addEvent(_event);
  
  }
}
