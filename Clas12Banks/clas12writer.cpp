/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "clas12writer.h"

namespace clas12 {

  clas12writer::clas12writer(std::string filename){
    _filename=filename;
  }

  /////////////////////////////////////////////////////////////////
  ///Assign a reader to the writer, allows for banks and schema
  ///info to be passed from reader.
  ///Do this here to allow multiple files to be read by
  ///different readers but written into one file.
  void clas12writer::assignReader(clas12reader& c12reader){
    _banks.clear();
    for(auto& bank : c12reader.getAllBanksPtrs()){
      std::string bankName = bank->getSchema().getName();
	if(!savedBankName(bankName)){
	 _banks.push_back(bank);
       }
    }
    
    if(_writer.getDictionary().getSchemaList().empty()){
      addSchemas(c12reader.getDictionary());
      openFile();	
    }
       
  }

  //////////////////////////////////////////////////////////////
  ///pass bank information to hipo::writer to write out events
  void clas12writer::writeEvent(){
     _outEvent.reset();   

    for(auto bank : _banks){
      _outEvent.addStructure(*bank.get());
    }
    
    _writer.addEvent(_outEvent);
  }

  /////////////////////////////////////////////////////////////
  ///closes hipo::writer, writes out events still on buffer
  void clas12writer::closeWriter(){
    _writer.close();
  }

  ////////////////////////////////////////////////////////////
  ///add schemas to writer
  void clas12writer::addSchemas(hipo::dictionary& factory){
    for(auto& name : factory.getSchemaList()){
      _writer.getDictionary().addSchema(factory.getSchema(name.c_str()));  
    }       
  }

  /////////////////////////////////////////////////////////////////////////
  ///opens file in hipo::writer, only open it once.
  void clas12writer::openFile(){
    cout<<" clas12writer::clas12writing writing to "<<_filename.data()<<endl;
    _writer.open(_filename.data()); //keep a pointer to the writer
  }

  ///////////////////////////////////////////////////////////////////////////
  ///checks if the name of a bank is kept in memory to be skipped
  bool clas12writer::savedBankName(std::string name){
    return std::find(_bankNamesToSkip.begin(), _bankNamesToSkip.end(), name) != _bankNamesToSkip.end();
  }

}
