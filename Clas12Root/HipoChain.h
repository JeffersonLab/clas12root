#pragma once


#include "clas12reader.h"
#include "TRcdbVals.h"

#include <TNamed.h>
#include <TChain.h>
#include <TObjArray.h>
#include <TString.h>
#include <TSystem.h>

//#ifdef CLAS_RCDB
//   #include "rcdb_reader.h"
//#endif


namespace clas12root {

  
  class HipoChain : public TNamed {


  public :
    HipoChain();
    virtual ~HipoChain()=default;
    HipoChain(const HipoChain& other) = delete; //Copy Constructor
    HipoChain(HipoChain&& other) = delete; //Move Constructor
      
    HipoChain& operator=(const HipoChain& other)=delete;
    HipoChain& operator=(HipoChain&& other)=delete;


    void Add(TString name);
    Int_t GetNFiles() const {return _ListOfFiles->GetEntries();}
    Long64_t GetNRecords();

    TString GetFileName(Int_t i)const{
      if(i>=GetNFiles()) return TString();
      return _ListOfFiles->At(_index[i])->GetTitle();
    }

    Int_t GetFileRecords(Int_t i)const{
      if(i<GetNFiles()) return _fileRecords[i];
      return 0 ;
    };
    Long64_t GetRecordsToHere(Int_t ifile);
    Int_t GetFileFromRecord(Long64_t irec);

    //Functions for sequential processing of chains of files
    Bool_t Next();
    Bool_t NextFile();
    void SetReaderTags(std::vector<long> tags){_readerTags=tags;}
    std::vector<long> ReaderTags()const noexcept{return _readerTags;}
    
    clas12::clas12reader* GetC12Reader();
    const std::unique_ptr<clas12::clas12reader>& C12ref()const {return _c12;}
    
    //void AddBeamCharge(Double_t bc){_totBeamCharge+=bc;}
    // Double_t TotalBeamCharge() const noexcept{return _totBeamCharge;}
    Double_t TotalBeamCharge() const noexcept{
      //check if set manually (e.g. from HipoSelector)
      if(_totBeamCharge!=0) return _totBeamCharge;
      //or check if we have qadb
      if(_db.qa()){
	return _db.qa()->getAccCharge();
      }
      //else do not try and get it ourselves (so accidents do not occur)
      return 0.;
    }
    void SetTotalBeamCharge(Double_t bc){_totBeamCharge=bc;}

    clas12::clas12databases* db() {return &_db;}
    void ConnectDataBases(){_c12->connectDataBases(&_db);}
    
///////////////////////////////RCDB
    void WriteRcdbData(TString filename);
    // clas12::rcdb_vals FetchRunRcdb(const TString& datafile);

    /*    void SetRcdbFile(const TString& filename ){
      //needs full path for PROOF
      if(filename.BeginsWith("/")==kFALSE&&filename.BeginsWith("$")==kFALSE)
	_rcdbFileName = TString(gSystem->Getenv("PWD"))+"/"+filename;
      else _rcdbFileName=filename;
      }*/
///////////////////////////////
    
  private :
    TChain _tchain;

    TObjArray* _ListOfFiles{nullptr}; //!owned by _tchain


    std::unique_ptr<clas12::clas12reader> _c12;

    clas12::clas12reader* _c12ptr{nullptr};//!

    clas12::clas12databases _db;
    
    Long64_t _Nrecords{-1};
    std::vector<Int_t> _fileRecords;

    std::vector<long> _readerTags;
    std::vector<int> _index;

    Int_t _idxFile{0};

    Double_t _totBeamCharge{0};

    TString _rcdbFileName;
    
    ClassDef(clas12root::HipoChain,1);
  };

 
}
