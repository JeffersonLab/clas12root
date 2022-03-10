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
    
    _readerDict=c12reader.getDictionary();
    
    for(auto& bank : c12reader.getAllBanksPtrs()){
      auto bankName = bank->getSchema().getName();
	if(!savedBankName(bankName)){
	 _banks.push_back(bank);
	 //Only add schema for banks we are writing
	 addSchema(bankName.data(),_readerDict);
       }
    }
    
    //special banks first so get written to dictionary
    if(_specialBanksBool){
      //This will call openFile to write the specialBanks
      processSpecialBanks(c12reader.getFilename());
    }
    else{
      //ready to open file
      openFile();
    }
  }

  ////////////////////////////////////////////////////////////////////
  ///Write the special banks (tag 1) to the output hipo file.
  void clas12writer::processSpecialBanks(std::string inputFilename){
    hipo::event inEvent_;
    hipo::event outEvent_;
    hipo::dictionary factory_;
    hipo::reader reader_;
    std::vector<hipo::bank> specialBanks;
    
    setTag(1);
    reader_.setTags(1);
    reader_.open(inputFilename.data());
    reader_.readDictionary(factory_);

    if(hasSchema("RUN::config")) specialBanks.push_back(factory_.getSchema("RUN::config"));
    if(hasSchema("HEL::online")) specialBanks.push_back(factory_.getSchema("HEL::online"));
    if(hasSchema("HEL::flip")) specialBanks.push_back(factory_.getSchema("HEL::flip"));
    if(hasSchema("RUN::scaler")) specialBanks.push_back(factory_.getSchema("RUN::scaler"));
    if(hasSchema("RAW::scaler")) specialBanks.push_back(factory_.getSchema("RAW::scaler"));
    if(hasSchema("RAW::epics")) specialBanks.push_back(factory_.getSchema("RAW::epics"));

    //add schema to dictionary
     for(auto& bank : specialBanks){
       
      auto bankName = bank.getSchema().getName();
      addSchema(bankName.data(),factory_);
     }
     //now we have defined dictionary for special banks
     //we open file for writing specialBanks
     //it is then ready for other events too
     openFile();
     
     while(reader_.next()==true){
      reader_.read(inEvent_);
      outEvent_.reset();
      for(auto& bank : specialBanks){
	inEvent_.getStructure(bank);
	outEvent_.addStructure(bank);
      }
      _writer.addEvent(outEvent_);  
      _nSpecialEvents++;
    }     
    setTag(0);
    
  }
  ////////////////////////////////////////////////////////
  ///check if the writer contains a given schema
  bool clas12writer::hasSchema(std::string schemaName){
    std::vector<std::string> schemaList =  _readerDict.getSchemaList();
    return std::find( schemaList.begin(), schemaList.end(), schemaName) != schemaList.end();
  }

  

  //////////////////////////////////////////////////////////////
  ///pass bank information to hipo::writer to write out events
  void clas12writer::writeEvent(){
     _outEvent.reset();   

    for(auto bank : _banks){
      _outEvent.addStructure(*bank);
    }
    
    _writer.addEvent(_outEvent);
    _nEvents++;
  }

  /////////////////////////////////////////////////////////////
  ///closes hipo::writer, writes out events still on buffer
  void clas12writer::closeWriter(){
    _writer.close();
    std::cout<<"clas12writer closed. Wrote "<<_nEvents<<" events and "<<_nSpecialEvents<<" events from special banks"<<std::endl;
    _isOpen=false;
  }

  ////////////////////////////////////////////////////////////
  ///add schemas to writer
  void clas12writer::addSchemas(hipo::dictionary& factory){
    for(auto& schemaName : factory.getSchemaList()){
      _writer.getDictionary().addSchema(factory.getSchema(schemaName.c_str()));  
    }       
  }

  void clas12writer::addSchema(std::string schemaName, hipo::dictionary& factory){
    _writer.getDictionary().addSchema(factory.getSchema(schemaName.c_str()));  
  }

  /////////////////////////////////////////////////////////////////////////
  ///opens file in hipo::writer, only open it once.
  void clas12writer::openFile(){
    std::cout<<" clas12writer writing to "<<_filename.data()<<std::endl;
    _writer.open(_filename.data()); //keep a pointer to the writer
    _isOpen=true;
  }

  ///////////////////////////////////////////////////////////////////////////
  ///checks if the name of a bank is kept in memory to be skipped
  bool clas12writer::savedBankName(std::string name){
    return std::find(_bankNamesToSkip.begin(), _bankNamesToSkip.end(), name) != _bankNamesToSkip.end();
  }

  //////////////////////////////////////////////////////////////////////////////////
  ///Sets a tag for the events record in the writer.
  ///For the tag to apply to the correct events need to write record using flush().
  void clas12writer::setTag(long tag){
    _writer.flush();
    _writer.setUserIntegerOne(tag);
  }

}
