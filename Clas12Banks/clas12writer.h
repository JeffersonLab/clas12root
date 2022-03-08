/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   clas12writer.h
 * Author: rtyson
 *
 */

#ifndef CLAS12WRITER_H
#define CLAS12WRITER_H

#include "clas12defs.h"
#include "clas12reader.h"
#include "writer.h"
#include "bank.h"
#include "event.h"
#include "utils.h"
#include "dictionary.h"

#include <algorithm>
#include <string>
#include <iostream>

namespace clas12 {
  using std::cout;
  using std::endl;

  class clas12writer  {


  public:


    clas12writer()=default;
    clas12writer(std::string filename);
    virtual ~clas12writer()=default;

    hipo::writer& getWriter() {return _writer;}   
   
    void writeEvent();
    void clearEvent();
    void closeWriter();
    void addSchemas(hipo::dictionary& factory);
    void openFile();
    bool savedBankName(std::string name);
    void assignReader(clas12reader& c12reader);
    void writeSpecialBanks(bool specialBanksBool){_specialBanksBool=specialBanksBool;};
    void processSpecialBanks(std::string inputFilename);
    void addSchema(std::string schemaName, hipo::dictionary& factory);
    bool hasSchema(std::string schemaName);
    void setTag(long tag);
    void setFile(const std::string& file){_filename=file;}
    //////////////////////////////////////////////////////////////////////////////////////////
    ///adds a bank name to the list of banks to skip
    void skipBank(std::string bankName){
      if(!savedBankName(bankName)){
	_bankNamesToSkip.push_back(bankName);
      }
    } 
    bool isOpen()const {return _isOpen;}
  private:
    
    //reader
    hipo::writer     _writer;
    hipo::event      _outEvent;
    std::vector<std::string> _bankNamesToSkip;
    std::string _filename;
    std::vector<hipo::bank* > _banks;
    long _nEvents = 0;
    long _nSpecialEvents = 0;
    bool _specialBanksBool = true;
    bool _isOpen = false;

  }; 

}

#endif /* CLAS12WRITER_H */
