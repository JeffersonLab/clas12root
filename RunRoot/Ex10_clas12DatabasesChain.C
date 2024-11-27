#include "clas12databases.h"
#include "clas12reader.h"

#include <TBenchmark.h>


using namespace clas12;
using namespace std;



void Ex10_clas12DatabasesChain(){

  /*For a database to be created you must specify the path to
    its database connection. In the case of ccdb this can be a
    a local sqlite file or a web link to the jlab clasdb account
    It is recommended to download a local copy to reduce connections
    It is recommended to edit and run the script PrepareDatabases.C
    for this purpose*/
  //clas12databases::SetCCDBLocalConnection("ccdb.sqlite");
  //clas12databases::SetRCDBRootConnection("rcdb.root");
  
  clas12root::HipoChain chain;
  // chain.Add("/where/are/my/files/f1.hipo");
  //chain.Add("/where/are/my/files/f2.hipo");

  chain.SetReaderTags({0});  //create clas12reader with just tag 0 events

  auto config_c12=chain.GetC12Reader();


  /*make a request for ccdb information (&=> reference, will update for next file)
    for list of tables , https://clasweb.jlab.org/cgi-bin/ccdb/objects*/
  //auto&  ccdbElSF=config_c12->ccdb()->requestTableDoubles("/calibration/eb/electron_sf");

  
  /*Get some data from the ccdb for this run
    rcdb info,(&=> reference, will update for next file)
    see Clas12Banks/rcdb_vals for full list of data
    this should match  https://clasweb.jlab.org/rcdb/conditions*/
  //auto& rcdbData= config_c12->rcdb()->current();//struct with all relevent rcdb values

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
  config_c12->applyQA(GETPASSSTRINGHERE);//GETPASSSTRINGHERE="latest", "pass1, "pass2",...
  config_c12->db()->qadb_addQARequirement("MarginalOutlier");
  config_c12->db()->qadb_addQARequirement("TotalOutlier");
  config_c12->db()->qadb_addQARequirement("TerminalOutlier");
  config_c12->db()->qadb_addQARequirement("MarginalOutlier");
  config_c12->db()->qadb_addQARequirement("SectorLoss");
  config_c12->db()->qadb_addQARequirement("LowLiveTime");
     

  gBenchmark->Start("db");
 
  //now get reference to (unique)ptr for accessing data in loop
  //this will point to the correct place when file changes
  auto& c12=chain.C12ref();
  while (chain.Next()){
   
    //The following run conditions can be returned directly by c12
    //cout<<"Event count: "<<rcdbData.event_count<<endl;
    //cout<<"Beam energy: "<<rcdbData.beam_energy<<endl;
    //cout<<"Beam current: "<<rcdbData.beam_current<<endl;
    //cout<<"Target Polarisation: "<<rcdbData.target_polarization<<endl;

    if(c12->ccdb()){
      //cout<< ccdbElSF.size()<<endl; //-0.036 
      //cout<< ccdbElSF[0].size()<<endl; //-0.036 
      //cout<< ccdbElSF[0][5]<<endl; //-0.036 
      //cout<< ccdbElSF[1][5]<<endl; //-0.036 
    }
    //break;
  }

   /*
   * The clas12-qadb software also provides the accumulated charge for events
   * that pass the quality assurance requirements.
   */
  cout<<"Accumulated charge past QA: "<< chain.TotalBeamCharge()<<" nC"<<endl;

  gBenchmark->Stop("db");
  gBenchmark->Print("db");
}
