#include "clas12databases.h"
#include "clas12reader.h"

#include <TBenchmark.h>


using namespace clas12;
using namespace std;



void Ex10_clas12Databases(){

  //For a database to be created you must specify the path to
  //its database connection. In the case of ccdb this can be a
  //a local sqlite file or a web link to the jlab clasdb account
  //It is recommended to download a local copy to reduce connections
  //It is recommended to edit and run the scrpit PrepareDatabases.C
  //for this purpose
  //clas12databases::SetCCDBLocalConnection("ccdb.sqlite");
  //clas12databases::SetRCDBRootConnection("rcdb.root");
  

  //create clas12reader with just tag 0 events
  clas12reader c12("/where/are/my/files/f1.hipo",{0});
  clas12databases db;
  c12.connectDataBases(&db);
  // c12.scalerReader();//for beam charge

  //Get some data from the ccdb for this run
  if(c12.ccdb()){
    auto& ccdbElSF = c12.ccdb()->requestTableDoubles("/calibration/eb/electron_sf");

    //checking againt ccdb online for this run number
    //https://clasweb.jlab.org/cgi-bin/ccdb/objects
    cout<< ccdbElSF[0][5]<<endl; //-0.036 
    cout<< ccdbElSF[1][5]<<endl; // 0.139
  }
  //rcdb info
  auto& rcdbData= c12.rcdb()->current();//struct with all relevent rcdb values
  //The following run conditions can be returned directly by c12
  cout<<"Event count: "<<rcdbData.event_count<<endl;
  cout<<"Beam energy: "<<rcdbData.beam_energy<<endl;
  cout<<"Beam current: "<<rcdbData.beam_current<<endl;

  //configure qadb requirements
   //configure qadb requirements
   /*
   *
   * addRequirement requires that the event was not identified
   * as, for example, a marginal outlier. Several requirements
   * can be assigned at the same time.
   * 
   * See RGA analysis note and clas12-qadb github repository for
   * additional information.
   */
  if(c12.qadb()!=nullptr){
    c12.db().applyQA(GETPASSSTRINGHERE);//GETPASSSTRINGHERE="latest", "pass1, "pass2",...
    c12.db().qadb_addQARequirement("MarginalOutlier");
    c12.db().qadb_addQARequirement("TotalOutlier");
    c12.db().qadb_addQARequirement("TerminalOutlier");
    c12.db().qadb_addQARequirement("MarginalOutlier");
    c12.db().qadb_addQARequirement("SectorLoss");
    c12.db().qadb_addQARequirement("LowLiveTime");
  }
 

  gBenchmark->Start("db");
  
  while(c12.next()) {
   
  }

  /*
   * The clas12-qadb software also provides the accumulated charge for events
   * that pass the quality assurance requirements.
   */
  cout<<"Accumulated charge past QA: "<<c12.db().qa()->getAccCharge()<<" nC"<<endl;

  
  gBenchmark->Stop("db");
  gBenchmark->Print("db");


  
}
