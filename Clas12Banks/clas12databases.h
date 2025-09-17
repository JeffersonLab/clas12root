/*
 * File:   clas12databases.h
 * Author: dglazier
 *
 */

#pragma once

#include "rcdb_reader.h"
#include "ccdb_reader.h"
#include "qadb_reader.h"
#include "TRcdbVals.h"

#include <TString.h>
#include <string>
#include <TSystem.h>

namespace clas12 {

  using std::string;

  
  class clas12databases {

   
    //where to make database connections
    static string  _RcdbPath;//!
    static string  _CcdbPath;//!
    //static string  _QadbPath;//!

  public:
    
    clas12databases();
    clas12databases& operator=(const clas12databases& other);
    clas12databases(const clas12databases &other);
    ~clas12databases()=default;

    void initDBs();

    void notifyRun(int runNb);

    
    //static void SetQADBConnection(const string& name);
    static void SetCCDBLocalConnection(const string& name);
    static void SetCCDBRemoteConnection();
    static void SetRCDBLocalConnection(const string& name);
    static void SetRCDBRemoteConnection();
    static void SetRCDBRootConnection(const string& name);
    static std::string FullPath(const string& fname);

    ccdb_reader* cc() const{return _ccdb.get();}
    rcdb_reader* rc()const {return _rcdb.get();}
    qadb_reader* qa()const {return _qadb.get();}

    void close();
    void open();
 
    const string&  ccdbPath() const {return _myCcdbPath;}
    const string&  rcdbPath() const {return _myRcdbPath;}
    //const string&  qadbPath() const {return _myQadbPath;}


 
    //qadb stuff
    //I would rather this was not needed here
    //but it is to make sure it gets passed to PROOF
    void qadb_addQARequirement(string req){
      if(_qadb.get()==nullptr){
	throw std::runtime_error("clas12databases::qadb_addQARequirement - Need to call clas12reader::applyQA(cook) first");
      }
      _qadb->addQARequirement(req);_qadbReqsQA.push_back(req);
    }
    void qadb_setQARequirements( std::vector<string> reqs){_qadb->setQARequirements(reqs); _qadbReqsQA = reqs;};

    void qadb_requireOkForAsymmetry(bool ok){_qadb->requireOkForAsymmetry(ok);_qadbReqOKAsymmetry=ok;};
    void qadb_requireGolden(bool ok){_qadb->requireGolden(ok);_qadbReqGolden=ok;};
    
    void  setVerbose(short level=1){_verbose=level;}

    void turnOffQADB(){_qadb.reset();}
    void turnOffRCDB(){_rcdb.reset();}
    void turnOffCCDB(){_ccdb.reset();}

    void setPass(const string& pass){
      _pass = pass;
      initQA();
    }
    void initQA(){
      std::cout<<"clas12databases::initQA pass = "<<_pass<<" run ="<<_runNb<<std::endl;
     _qadb.reset( new qadb_reader{_pass,_runNb} );
      qadb_requireOkForAsymmetry(_qadbReqOKAsymmetry);
      qadb_requireGolden(_qadbReqGolden);
      qadb_setQARequirements(_qadbReqsQA);  
    }
  private:
    
   //names for copying to ROOT file for selector
    string _myRcdbPath;
    string  _myCcdbPath;
    //string  _myQadbPath;

    string _pass; //!
    int _runNb={0};//!

    std::unique_ptr<rcdb_reader> _rcdb;//!
    std::unique_ptr<ccdb_reader> _ccdb;//!
    std::unique_ptr<qadb_reader> _qadb;//!


    //qadb stuff
    //I would rather this wwas not needed here
    //but it is to make sure it gets passed to PROOF
    std::vector<string> _qadbReqsQA;
    bool _qadbReqOKAsymmetry{false};
    bool _qadbReqGolden{false};

    short _verbose={0};
    };
  

  
}
