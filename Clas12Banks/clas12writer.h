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

    hipo::writer& getWriter(){return _writer;}
    
   
    void writeEvent();
    void clearEvent();
    void closeWriter();
    void addSchemas(hipo::dictionary& factory);
    void openFile();
    bool savedBankName(std::string name);
    void assignReader(clas12reader& c12reader);

    //////////////////////////////////////////////////////////////////////////////////////////
    ///removes a bank from the list of banks to write out
    void skipBank(std::string bankName){
      if(!savedBankName(bankName)){
	_bankNamesToSkip.push_back(bankName);
      }
    } 
    
  private:

    //reader
    hipo::writer     _writer;
    hipo::event      _outEvent;
    std::vector<std::string> _bankNamesToSkip;
    std::string _filename;
    std::vector<std::shared_ptr<hipo::bank> > _banks;
  }; 

}

#endif /* CLAS12WRITER_H */
