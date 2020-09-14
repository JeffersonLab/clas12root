#include <TError.h>
#include <TFile.h>
#include "HipoChain.h"
#include "reader.h"

namespace clas12root {

  HipoChain::HipoChain(): TNamed("HIPOFILES","A chain of hipo files"){

    _ListOfFiles=_tchain.GetListOfFiles();
  }
  
  
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
      hiporeader.setTags(_readerTags);
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
  
  Bool_t HipoChain::Next(){

    bool more = kFALSE;

    if( _c12.get() )  more = _c12->next();
    if(more) return kTRUE;
   //When file opened add beam charge _c12.get()==nullptr when files are finished
    if(_idxFile>0&&_c12.get())_totBeamCharge+=_c12->getRunBeamCharge();
    if(NextFile()==false) return kFALSE;
    more = _c12->next();
    if(more) return kTRUE;
    
    //no more delete reader
    _c12.reset();
    _c12ptr=nullptr;
    //check if another file
    return Next();
    
  }
  
  Bool_t HipoChain::NextFile(){
    /////Warning any changes to this function should
    ////also be considered for  HipoSelector::Process
    std::cout<<"HipoChain::NextFile() "<<_idxFile<<" out of "<<GetNFiles()<<std::endl;
   if(_idxFile>=GetNFiles())
      return kFALSE;//no more files
    std::cout<<"HipoChain::NextFile() "<<GetFileName(_idxFile)<<std::endl;
    //open next file
    // _c12.reset(new clas12::clas12reader{*_c12.get(),GetFileName(_idxFile++).Data(),_readerTags});
   _c12.reset(new clas12::clas12reader{*GetC12Reader(),GetFileName(_idxFile++).Data(),_readerTags});
   //if there is a root rcdb file use it
   if(_rcdbFileName.Length())_c12->setRcdbVals(FetchRunRcdb(_c12->getFilename()));
   
    _c12ptr = _c12.get();	
    return kTRUE;
  }


  /////////////////////////////////////////////////////
  /// Get the rcdb info for all the files in the chain
  
  void HipoChain::WriteRcdbData(TString filename){

    _rcdbFileName=filename;
    
 #ifdef RCDB_MYSQL
  
    auto rcdbFile=std::unique_ptr<TFile>{TFile::Open(filename,"recreate")};
    clas12::rcdb_reader rc; //initialise rcdb_reader
  
    auto nfiles=GetNFiles();
    //loop over files and get the rcdb data
    for(auto i=0;i<nfiles;++i){
      auto runNb=clas12::clas12reader::readQuickRunConfig(GetFileName(i).Data());
      auto vals=std::unique_ptr<clas12root::TRcdbVals>{new clas12root::TRcdbVals(rc.readAll(runNb,GetFileName(i).Data()))};
      //auto vals=new clas12root::TRcdbVals(rc.readAll(runNb,GetFileName(i).Data()));

      vals->Write();
    }
    rcdbFile->ls();
  
#endif
    
  }
  ////////////////////////////////////////////////////////////
  ///Get a copy of the rcdb values for run number runNb,
  ///from file fname created previously by WriteRcdbData
  //#include ""
  clas12::rcdb_vals HipoChain::FetchRunRcdb(const TString& datafile){
#ifdef RCDB_MYSQL

    //make file and list unique_ptr so deleted when we return
    auto rcdbFile=std::unique_ptr<TFile>{TFile::Open(_rcdbFileName)};
    if(rcdbFile.get()==nullptr){
      Warning("HipoChain::FetchRunRcdb ",Form("No rcdb root file provided to the chain :  %s",_rcdbFileName.Data()),"");
      return clas12::rcdb_vals();
    }
    auto keys= rcdbFile->GetListOfKeys();
    for(const auto& key:*keys){
      //the rcdb_vals Title is mapped to the data file name
      if(TString(key->GetTitle())==TString(datafile)){
	auto vals=std::unique_ptr<TRcdbVals>{dynamic_cast<TRcdbVals*>(rcdbFile->Get(key->GetName()))};
	if(vals.get())
	  return vals.get()->_data;
      }
    }
    Warning("HipoChain::FetchRunRcdb ",Form("run file %s not found in list in file %s",datafile.Data(),_rcdbFileName.Data()),"");

#endif
    //no rcdb     
    return clas12::rcdb_vals();
  }
}
