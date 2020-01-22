#include "HipoChain.h"
#include "reader.h"

namespace clas12root {

  HipoChain::HipoChain(): TNamed("HIPOFILES","A chain of hipo files"){

    _ListOfFiles=_tchain.GetListOfFiles();
  }
  
  // HipoChain::~HipoChain(){
  // }

  
  void HipoChain::Add(TString name){

    _tchain.Add(name);
    _Nrecords=-1; //must recalculate    
  }

  Long64_t HipoChain::GetNRecords(){
    if(_Nrecords!=-1)
      return _Nrecords;

    _Nrecords=0;
    _fileRecords.clear();
    auto nfiles=GetNFiles();
    //loop over files and get the number of records
    for(auto i=0;i<nfiles;++i){
      hipo::reader hiporeader;
      hiporeader.open(GetFileName(i));
      _Nrecords+=hiporeader.getNRecords();
      _fileRecords.push_back(hiporeader.getNRecords());
    }
    return _Nrecords;
  }
  Long64_t HipoChain::GetRecordsToHere(Int_t ifile){
    Long64_t n=0;
    if(ifile>GetNFiles()) ifile=GetNFiles();
    for(Int_t i=0;i<ifile;i++)
      n+=_fileRecords[i];
    return n;
    
  }
  Int_t HipoChain::GetFileFromRecord(Long64_t irec){
    Int_t n = _fileRecords.size();
    for(Int_t i=0;i<n;i++){
      if(GetRecordsToHere(i)>irec)
	return i-1;
    }
    return n-1;
  }
}
