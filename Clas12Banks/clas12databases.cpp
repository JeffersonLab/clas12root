#include "clas12databases.h"
#include "clas12reader.h"

#include <TFile.h>

namespace clas12 {
  
  string clas12databases::_RcdbPath="";
  string clas12databases::_CcdbPath="";
  string clas12databases::_QadbPath="";
  
  void clas12databases::SetQADBConnection(const string& name){_QadbPath=FullPath(name);}
  void clas12databases::SetCCDBLocalConnection(const string& name){_CcdbPath="sqlite://"+FullPath(name);}
  void clas12databases::SetCCDBRemoteConnection(){_CcdbPath="mysql://clas12reader@clasdb.jlab.org/clas12";}
  void clas12databases::SetRCDBLocalConnection(const string& name){_RcdbPath="sqlite://"+FullPath(name);}
  void clas12databases::SetRCDBRemoteConnection(){_RcdbPath="mysql://rcdb@clasdb.jlab.org/rcdb";}
  void clas12databases::SetRCDBRootConnection(const string& name){_RcdbPath=FullPath(name);}

  std::string clas12databases::FullPath(const string& fname){
    if(fname[0]!='/')
      return std::string( gSystem->Getenv("PWD")) +"/"+ fname;
    return fname;
  }

  clas12databases::clas12databases()
  {
    std::cout<<"clas12databases() rcdb path "<<_RcdbPath<<" "<<_myRcdbPath<<std::endl;
    std::cout<<"clas12databases() ccdb path "<<_CcdbPath<<std::endl;
    std::cout<<"clas12databases() qadb path "<<_QadbPath<<std::endl;

    _myRcdbPath=_RcdbPath;
    _myCcdbPath=_CcdbPath;
    _myQadbPath=_QadbPath;

    initDBs();
   }
  clas12databases& clas12databases::operator=(const clas12databases& other)
  {
    
    _myRcdbPath=other._myRcdbPath;
    _myCcdbPath=other._myCcdbPath;
    _myQadbPath=other._myQadbPath;
    
    initDBs();

   if(_qadb.get()){
     qadb_requireOkForAsymmetry(other._qadbReqOKAsymmetry);
     qadb_requireGolden(other._qadbReqGolden);
     qadb_setQARequirements(other._qadbReqsQA);
     
    }
 
    return *this;
  }
  clas12databases::clas12databases(const clas12databases &other){
    
    _myRcdbPath=other._myRcdbPath;
    _myCcdbPath=other._myCcdbPath;
    _myQadbPath=other._myQadbPath;
    
    
    initDBs();

    if(_qadb.get()){
      qadb_requireOkForAsymmetry(other._qadbReqOKAsymmetry);
      qadb_requireGolden(other._qadbReqGolden);
      qadb_setQARequirements(other._qadbReqsQA);
 
    }
  }
  
  void clas12databases::initDBs(){
    std::cout<<"clas12databases() rcdb path "<<_myRcdbPath<<std::endl;
    std::cout<<"clas12databases() ccdb path "<<_myCcdbPath<<std::endl;
    std::cout<<"clas12databases() qadb path "<<_myQadbPath<<std::endl;

     
    if(_myCcdbPath.empty()==false)
      _ccdb.reset( new ccdb_reader{_myCcdbPath.data()} );
    
    if(_myRcdbPath.empty()==false){
      if(_myRcdbPath.find(".root")!= std::string::npos)
	_rcdb.reset( new rcdb_root{_myRcdbPath.data()} );
      else
	_rcdb.reset( new rcdb_reader{_myRcdbPath.data()} );
    }
    
    if(_myQadbPath.empty()==false){
      _qadb.reset( new qadb_reader{_myQadbPath.data()} );
      qadb_requireOkForAsymmetry(_qadbReqOKAsymmetry);
      qadb_requireGolden(_qadbReqGolden);
      qadb_setQARequirements(_qadbReqsQA);
    }

}
  //update run number to each database
  void clas12databases::notifyRun(int runNb){
    _runNb=runNb;

    open();


    std::cout<<"clas12databases::notifyRun "<<runNb<<" ccdb = "<<(_ccdb.get()!=nullptr) <<" rcdb = "<<(_qadb.get()!=nullptr)<<" qadb = "<<(_rcdb.get()!=nullptr)<<std::endl;
    if(_ccdb.get()!=nullptr){
      _ccdb->setRun(runNb);
      _ccdb->updateTables();
    }
    if(_qadb.get()!=nullptr)_qadb->setRun(runNb);
    if(_rcdb.get()!=nullptr)_rcdb->readRun(runNb);

    close();
  }

  void clas12databases::close(){
     if(_ccdb.get()!=nullptr)_ccdb->close();
     if(_rcdb.get()!=nullptr)_rcdb->close();
    
  }
  void clas12databases::open(){
     if(_ccdb.get()!=nullptr)_ccdb->open();
     if(_rcdb.get()!=nullptr)_rcdb->open();
   
  }
  /////////////////////////////////////////////////////
  /// Get the rcdb info for all the files in the chain 
  //  void clas12databases::writeRcdbData(TString filename,const clas12root::HipoChain& chain){
  /*
  void clas12databases::writeRcdbData(TString filename){

    if(_rcdb.get()==nullptr){
      std::cout<<"clas12databases::writeRcdbData no rcdb intiated with clas12databases::SetRCDB... "<<std::endl;
      return;
    }
        
    auto rcdbFile=std::unique_ptr<TFile>{TFile::Open(filename,"recreate")};
    
    auto nfiles=chain.GetNFiles();
    //loop over files and get the rcdb data
    for(auto i=0;i<nfiles;++i){
      auto runNb=clas12reader::readQuickRunConfig(chain.GetFileName(i).Data());
      auto runName = Form("%d",runNb);
      auto vals=std::unique_ptr<clas12root::TRcdbVals>{new clas12root::TRcdbVals(_rcdb->readAll(runNb,runName))};
      vals->Write();
    }
    
    rcdbFile->ls();
    
  }
  */
  /*
  ////////////////////////////////////////////////////////////
  ///Get a copy of the rcdb values for run number runNb,
  ///from file fname created previously by WriteRcdbData
  clas12::rcdb_vals HipoChain::fetchRunRcdb(const TString& datafile){

    //make file and list unique_ptr so deleted when we return
    auto rcdbFile=std::unique_ptr<TFile>{TFile::Open(_rcdbFileName)};
    if(rcdbFile.get()==nullptr){
      Warning("HipoChain::FetchRunRcdb ",Form("No rcdb root file provided to the chain :  %s",_rcdbFileName.Data()),"");
      return rcdb_vals();
    }
    auto keys= rcdbFile->GetListOfKeys();
    for(const auto& key:*keys){
      //the rcdb_vals Title is mapped to the data file name
      auto baseName = gSystem->BaseName(datafile);//use base name in case user copies data to temp directory
      if(TString(key->GetTitle())==TString(baseName)){
	auto vals=std::unique_ptr<TRcdbVals>{dynamic_cast<TRcdbVals*>(rcdbFile->Get(key->GetName()))};
	if(vals.get())
	  return vals.get()->_data;
      }
    }
    Warning("HipoChain::FetchRunRcdb ",Form("run file %s not found in list in file %s",datafile.Data(),_rcdbFileName.Data()),"");

    //no rcdb     
    return rcdb_vals();
  }
  */
}
