#pragma once


#include <TNamed.h>
#include <TChain.h>
#include <TObjArray.h>
#include <TString.h>

namespace clas12root {

  
  class HipoChain : public TNamed {


  public :
    HipoChain();
    virtual ~HipoChain();


    void Add(TString name);
    Int_t GetNFiles(){return _ListOfFiles->GetEntries();}
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
  private :
    TChain _tchain;

    TObjArray* _ListOfFiles=nullptr;

    Long64_t _Nrecords=-1;
    std::vector<Int_t> _fileRecords;
    
    ClassDef(clas12root::HipoChain,1);
  };
}
