#include "rcdb_reader.h"
#include <TRcdbVals.h>
#include <TFile.h>

namespace clas12 {

  /* Empty constructor needed to link _connection to database.
   */
#ifdef CLAS_RCDB  
  rcdb_reader::rcdb_reader(string connect_path):
    _connection{new rcdb::Connection{connect_path.data(), true} }
  {

  }
   /* explicit close in destructor.
   */
  rcdb_reader::~rcdb_reader(){close();_connection.reset();};
  
  /* Function to return a bool type value, given the value's name and a run
   * number.
   * Will return false if the condition doesn't exist for this run, or the run
   * number isn't recorded.
   * If the value is not of type bool the program will stop.
   */
  bool rcdb_reader::getBoolValue(int runNb, std::string value){
    auto cnd = _connection->GetCondition(runNb, value);
    bool val = false;
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToBool();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not a bool."<<std::endl;
	}
    }
    return val;
  }

  /* Function to return an integer type value, given the value's name and a run
   * number.
   * Will return -1 if the condition doesn't exist for this run, or the run
   * number isn't recorded.
   * If the value is not of type integer the program will stop.
   */
  int rcdb_reader::getIntValue(int runNb, std::string value){
    auto cnd = _connection->GetCondition(runNb, value);
    int val = -1;
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToInt();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not an int."<<endl;
	}
    }
    return val;
  }

  /* Function to return a double type value, given the value's name and a run
   * number.
   * Will return -1 if the condition doesn't exist for this run, or the run
   * number isn't recorded.
   * If the value is not of type double the program will stop.
   */
  double rcdb_reader::getDoubleValue(int runNb, std::string value){
    auto cnd = _connection->GetCondition(runNb, value);
    double val = -1;
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToDouble();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not a double."<<endl;
	}
    }
    return val;
  }

  /* Function to return a string type value, given the value's name and a run
   * number.
   * Will return Error if the condition doesn't exist for this run, or the run
   * number isn't recorded.
   * If the value is not of type string the program will stop.
   */
  std::string rcdb_reader::getStringValue(int runNb, std::string value){
    auto cnd = _connection->GetCondition(runNb, value);
    std::string val = "Error";
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToString();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not a double."<<endl;
	}
    }
    return val;
  }
  
  /* Function to return a time_point type value, given the value's name and a 
   * run number.
   * Will return the current time if the condition doesn't exist for this run,
   * or if the run number isn't recorded.
   * If the value is not of type time_point the program will stop.
   */
  std::chrono::time_point<std::chrono::system_clock> rcdb_reader::getTimeValue(int runNb, std::string value){
    auto cnd = _connection->GetCondition(runNb, value);
    std::chrono::time_point<std::chrono::system_clock> val = std::chrono::system_clock::now();
    if(!cnd){
      cout<<"The condition "<<value<<" does not exist for run "<<runNb<<"."<<endl;
    }
    else{
      try{
	val = cnd->ToTime();
      }
      catch(int e)
	{
	  cout<<"The value for this condition is not a double."<<endl;
	}
    }
    return val;
  }

  void rcdb_reader::close(){if(_connection)_connection->Close();}
  void rcdb_reader::open(){if(_connection)_connection->Connect();}

 
#else
  rcdb_reader::rcdb_reader(string connect_path)
  {

  }
   /* explicit close in destructor.
   */
  rcdb_reader::~rcdb_reader(){close();};

  void rcdb_reader::close(){}
  void rcdb_reader::open(){}

#endif
  
  /* Function to read all conditions given in https://clasweb.jlab.org/rcdb/conditions/
     returns a copy of all vals
   */
  void rcdb_reader::readRun(int runNb){
    setCurrentVals(readAll(runNb,""));
  }
  rcdb_vals rcdb_reader::readAll(int runNb,const string& filename){

    rcdb_vals fetcher;
    fetcher.run_number=runNb;//additional keep the run number
#ifdef CLAS_RCDB  
    fetcher.beam_current=getDoubleValue(runNb, "beam_current");
    fetcher.beam_energy=getDoubleValue(runNb, "beam_energy");
    fetcher.events_rate=getDoubleValue(runNb, "events_rate");
    fetcher.solenoid_current=getDoubleValue(runNb, "solenoid_current");
    fetcher.solenoid_scale=getDoubleValue(runNb, "solenoid_scale");
    fetcher.target_position=getDoubleValue(runNb, "target_position");
    fetcher.test=getDoubleValue(runNb, "test");
    fetcher.torus_current=getDoubleValue(runNb, "torus_current");
    fetcher.torus_scale=getDoubleValue(runNb, "torus_scale");
    
    fetcher.event_count=getIntValue(runNb, "event_count");
    fetcher.evio_files_count=getIntValue(runNb, "evio_files_count");
    fetcher.half_wave_plate=getIntValue(runNb, "half_wave_plate");
    fetcher.megabyte_count=getIntValue(runNb, "megabyte_count");
    fetcher.status=getIntValue(runNb, "status");
    fetcher.temperature=getIntValue(runNb, "temperature");

   
    fetcher.file_name=filename;
    fetcher.beam_current_request=getStringValue(runNb, "beam_current_request");
    fetcher.daq_comment=getStringValue(runNb, "daq_comment");
    fetcher.daq_config=getStringValue(runNb, "daq_config");
    fetcher.daq_setup=getStringValue(runNb, "daq_setup");
    fetcher.daq_trigger=getStringValue(runNb, "daq_trigger");
    fetcher.operators=getStringValue(runNb, "operators");
    fetcher.run_type=getStringValue(runNb, "run_type");
    fetcher.target=getStringValue(runNb, "target");
    fetcher.user_comment=getStringValue(runNb, "user_comment");
#endif
    return fetcher;
  }
  //////////////////////////////////////////////////////////////////
  rcdb_root::rcdb_root(string connect_path):
    _rootFile{connect_path.data()}
  {
    
  }

  void rcdb_root::readRun(int runNb){

    if(runNb==11) return;//Simulation
    if(runNb==10) return;//Simulation
    
    //make file and list unique_ptr so deleted when we return
    auto rcdbFile=std::unique_ptr<TFile>{TFile::Open(_rootFile.data())};
    if(rcdbFile.get()==nullptr){
      std::cout<<"rcdb_root::readRun "<<Form("No rcdb root file provided to the chain :  %s",_rootFile.data())<<std::endl;
      setCurrentVals(rcdb_vals());
      return;
    }
    auto keys= rcdbFile->GetListOfKeys();
    for(const auto& key:*keys){
      //the rcdb_vals Title is mapped to the data file name
      auto runName=Form("%d",runNb);
      if(TString(key->GetTitle())==TString(runName)){
	auto vals=std::unique_ptr<clas12root::TRcdbVals>{dynamic_cast<clas12root::TRcdbVals*>(rcdbFile->Get(key->GetName()))};
	if(vals.get()){
	  setCurrentVals(vals.get()->_data);
	  return;
	}
      }
    }
    std::cout<<"rcdb_root::readRun "<<Form("run file %d not found in list in file %s",runNb,_rootFile.data())<<std::endl;
    std::cout<<" You should run PrepareDatabases.C editing/including all the files you are running on to HipoChain."<<endl;
    exit(0);
    //no rcdb     
    setCurrentVals(rcdb_vals());

    
  }
}
