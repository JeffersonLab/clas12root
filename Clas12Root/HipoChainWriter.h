#pragma once


#include "clas12writer.h"
#include "HipoChain.h"

namespace clas12root {

  
  class HipoChainWriter : public HipoChain {


  public :
    HipoChainWriter()=delete;
    HipoChainWriter(const TString& output);
    virtual ~HipoChainWriter(){CloseOutput();}
    //virtual ~HipoChainWriter()=default;
    HipoChainWriter(const HipoChainWriter& other) = delete; //Copy Constructor
    HipoChainWriter(HipoChainWriter&& other) = delete; //Move Constructor
      
    HipoChainWriter& operator=(const HipoChainWriter& other)=delete;
    HipoChainWriter& operator=(HipoChainWriter&& other)=delete;


    clas12::clas12writer& GetWriter(){return _writer;}
    void WriteEvent(){_writer.writeEvent();}

    void CloseOutput(){_writer.closeWriter();}

    const TString& GetOutDir(){return _outdir;}
    
  private :
    Bool_t NextFile() final;
 
    clas12::clas12writer _writer;//!
    TString _outdir;
    Bool_t _multiFile=kFALSE;
 
    ClassDef(clas12root::HipoChainWriter,1);
  };

 
}
