#pragma once


#include "clas12reader.h"

#include <TNamed.h>
#include <TChain.h>
#include <TObjArray.h>
#include <TString.h>

namespace clas12root {

  
  class HipoChain : public TNamed {


  public :
    HipoChain();
    virtual ~HipoChain()=default;


    void Add(TString name);
    Int_t GetNFiles() const {return _ListOfFiles->GetEntries();}
    Long64_t GetNRecords();

    TString GetFileName(Int_t i){
      if(i>=GetNFiles()) return TString();
      return _ListOfFiles->At(i)->GetTitle();
    }

    Int_t GetFileRecords(Int_t i){
      if(i<GetNFiles()) return _fileRecords[i];
      return 0 ;
    };
    Long64_t GetRecordsToHere(Int_t ifile);
    Int_t GetFileFromRecord(Long64_t irec);

    //Functions for sequential processing of chains of files
    Bool_t Next();
    Bool_t NextFile();
    void SetReaderTags(std::vector<long> tags){_readerTags=tags;}
    
    clas12::clas12reader* GetC12Reader() {
      if( (_c12ptr=_c12.get()) )
	return _c12ptr;
      _c12.reset(new clas12::clas12reader{""});
      _c12ptr = _c12.get();
      return  _c12ptr;
    }

    void AddBeamCharge(Double_t bc){_totBeamCharge+=bc;}
    Double_t TotalBeamCharge() const noexcept{return _totBeamCharge;}
    
  private :
    TChain _tchain;

    TObjArray* _ListOfFiles{nullptr}; //owned by _tchain

    std::unique_ptr<clas12::clas12reader> _c12;
    clas12::clas12reader* _c12ptr{nullptr};
    
    Long64_t _Nrecords{-1};
    std::vector<Int_t> _fileRecords;

    std::vector<long> _readerTags;
    Int_t _idxFile{0};

    Double_t _totBeamCharge{0};
    
    ClassDef(clas12root::HipoChain,1);
  };
}
