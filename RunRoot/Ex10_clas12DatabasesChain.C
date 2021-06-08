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
   * Several quality assurance requirements can be specified.
   * requireOkForAsymmetry requires that an event was deemed
   * suitable for asymmetry measurements.
   *
   * addRequirement requires that the event was not identified
   * as, for example, a marginal outlier. Several requirements
   * can be assigned at the same time.
   * 
   * See RGA analysis note and clasqaDB github repository for
   * additional information.
   */
  if(config_c12->qadb()!=nullptr){
    config_c12->db()->qadb_requireOkForAsymmetry(true);
    config_c12->db()->qadb_requireGolden(true);
    config_c12->db()->qadb_addQARequirement("MarginalOutlier");
    config_c12->db()->qadb_addQARequirement("TotalOutlier");
   /*
     * applyQA specifies to the clas12reader that quality assurance
     * cuts will be applied, based on the .json file given as an 
     * argument. This file should contain the Clas12 Quality Assurance
     * database.
     * i.e. clas12reader will only process events that pass quality assurance
     * and ignore those which fail
     */
    config_c12->applyQA();
  }

  gBenchmark->Start("db");
 
  //now get reference to (unique)ptr for accessing data in loop
  //this will point to the correct place when file changes
  auto& c12=chain.C12ref();
  while (chain.Next()){
   
    //The following run conditions can be returned directly by c12
    //cout<<"Event count: "<<rcdbData.event_count<<endl;
    //cout<<"Beam energy: "<<rcdbData.beam_energy<<endl;
    //cout<<"Beam current: "<<rcdbData.beam_current<<endl;
 
    if(c12->ccdb()){
      //cout<< ccdbElSF.size()<<endl; //-0.036 
      //cout<< ccdbElSF[0].size()<<endl; //-0.036 
      //cout<< ccdbElSF[0][5]<<endl; //-0.036 
      //cout<< ccdbElSF[1][5]<<endl; //-0.036 
    }
    //break;
  }

   /*
   * The clasqaDB software also provides the accumulated charge for events
   * that pass the quality assurance requirements.
   */
  cout<<"Accumulated charge past QA: "<< chain.db()->qa()->getAccCharge()<<" nC"<<endl;

  gBenchmark->Stop("db");
  gBenchmark->Print("db");
}
