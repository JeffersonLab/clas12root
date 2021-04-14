#include <TError.h>
#include <TFile.h>
#include "HipoChain.h"
#include "reader.h"
#include "clas12reader.h"

namespace clas12root {
  
  HipoChain::HipoChain(): TNamed("HIPOFILES","A chain of hipo files"){

    _ListOfFiles=_tchain.GetListOfFiles();

   }
  
  
  void HipoChain::Add(TString name){

    _tchain.Add(name);
    _Nrecords=-1; //must recalculate

    auto nfiles=GetNFiles();
    _index.resize(nfiles);

    for(int i=0;i<nfiles;i++)
      _index[i]=i;

  }

  Long64_t HipoChain::GetNRecords(){
    if(_Nrecords!=-1)
      return _Nrecords;

    _Nrecords=0;
    _fileRecords.clear();
    auto nfiles=GetNFiles();
    //loop over files and get the number of records
    int ngood=0;
    for(auto i=0;i<nfiles;++i){
      hipo::reader hiporeader;
      hiporeader.setTags(_readerTags);
      hiporeader.open(GetFileName(ngood));
      if(hiporeader.getNRecords()<=0){
	Warning("GetNRecords()",Form("problem with hipo file %s, will ignore",GetFileName(ngood).Data()),"");
	//remove file from chain, need to use i rather than ngood here
	_ListOfFiles->RemoveAt(i);
	//adjust index to avoid _ListOfFiles->At(i)
	//which will be null
	_index.erase(_index.begin() + ngood);
	continue;
      }
      _Nrecords+=hiporeader.getNRecords();
      _fileRecords.push_back(hiporeader.getNRecords());
      ngood++;
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
    //open next file
    _c12.reset(new clas12::clas12reader{*GetC12Reader(),GetFileName(_idxFile++).Data(),_readerTags});
    ConnectDataBases();
    
    _c12ptr = _c12.get();	
    return kTRUE;
  }
  

  clas12::clas12reader* HipoChain::GetC12Reader() {
      if( (_c12ptr=_c12.get()) )
	return _c12ptr;
      _c12.reset(new clas12::clas12reader{""});
      ConnectDataBases();
      _c12ptr = _c12.get();
      return  _c12ptr;
    }



  /////////////////////////////////////////////////////
  /// Get the rcdb info for all the files in the chain 
  void HipoChain::WriteRcdbData(TString filename){

    _rcdbFileName=filename;
    
 #ifdef CLAS_RCDB
    std::cout<<"HipoChain "<<std::endl;
    auto rcdbFile=std::unique_ptr<TFile>{TFile::Open(filename,"recreate")};
    clas12::rcdb_reader rc{clas12::clas12databases().rcdbPath()}; //initialise rcdb_reader
  
    auto nfiles=GetNFiles();
    //loop over files and get the rcdb data
    for(auto i=0;i<nfiles;++i){
      auto runNb=clas12::clas12reader::readQuickRunConfig(GetFileName(i).Data());
      auto runName = Form("%d",runNb);
      auto vals=std::unique_ptr<clas12root::TRcdbVals>{new clas12root::TRcdbVals(rc.readAll(runNb,runName))};
      //auto vals=new clas12root::TRcdbVals(rc.readAll(runNb,GetFileName(i).Data()));

      vals->Write();
    }
    rcdbFile->ls();
  
#endif
    
  }
 
}
