#include "clas12databases.h"
#include "clas12reader.h"

#include <TFile.h>

namespace clas12 {
  
  string clas12databases::_RcdbPath="";
  string clas12databases::_CcdbPath="";
  //string clas12databases::_QadbPath="";
  
  //void clas12databases::SetQADBConnection(const string& name){_QadbPath=FullPath(name);}
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
    if(_verbose){
      std::cout<<"clas12databases() rcdb path "<<_RcdbPath<<" "<<_myRcdbPath<<std::endl;
      std::cout<<"clas12databases() ccdb path "<<_CcdbPath<<std::endl;
      //std::cout<<"clas12databases() qadb path "<<_QadbPath<<std::endl;
    }
    _myRcdbPath=_RcdbPath;
    _myCcdbPath=_CcdbPath;
    //_myQadbPath=_QadbPath;

    initDBs();
   }
  clas12databases& clas12databases::operator=(const clas12databases& other)
  {
    
    _myRcdbPath=other._myRcdbPath;
    _myCcdbPath=other._myCcdbPath;
    //_myQadbPath=other._myQadbPath;
     _pass = other._pass;
   
    initDBs();

   if(other._qadb.get()){
     initQA();
     qadb_requireOkForAsymmetry(other._qadbReqOKAsymmetry);
     qadb_requireGolden(other._qadbReqGolden);
     qadb_setQARequirements(other._qadbReqsQA);
    }
 
    return *this;
  }
  clas12databases::clas12databases(const clas12databases &other){
    
    _myRcdbPath=other._myRcdbPath;
    _myCcdbPath=other._myCcdbPath;
    //_myQadbPath=other._myQadbPath;
    _pass = other._pass;
    
    initDBs();

    if(other._qadb.get()){
      initQA();
      qadb_requireOkForAsymmetry(other._qadbReqOKAsymmetry);
      qadb_requireGolden(other._qadbReqGolden);
      qadb_setQARequirements(other._qadbReqsQA);
 
    }
  }
  
  void clas12databases::initDBs(){
    //std::cout<<"clas12databases() rcdb path "<<_myRcdbPath<<std::endl;
    //std::cout<<"clas12databases() ccdb path "<<_myCcdbPath<<std::endl;
    //std::cout<<"clas12databases() qadb path "<<_myQadbPath<<std::endl;

     
    if(_myCcdbPath.empty()==false)
      _ccdb.reset( new ccdb_reader{_myCcdbPath.data()} );
    
    if(_myRcdbPath.empty()==false){
      if(_myRcdbPath.find(".root")!= std::string::npos)
	_rcdb.reset( new rcdb_root{_myRcdbPath.data()} );
      else
	_rcdb.reset( new rcdb_reader{_myRcdbPath.data()} );
    }
    
    
    // _qadb.reset( new qadb_reader{0} );
    // qadb_requireOkForAsymmetry(_qadbReqOKAsymmetry);
    // qadb_requireGolden(_qadbReqGolden);
    // qadb_setQARequirements(_qadbReqsQA);  

}
  //update run number to each database
  void clas12databases::notifyRun(int runNb){
    //check if already set up for this run
    if( _runNb==runNb) return;
    
    _runNb=runNb;

    open();


     if(_ccdb.get()!=nullptr){
      _ccdb->setRun(runNb);
      _ccdb->updateTables();
     }
     if(_qadb.get()!=nullptr){
       //     _qadb->setRun(runNb);
       initQA();
     }
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
}
