#define tempHipoSelector_cxx
// The class definition in tempHipoSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//


#include "tempHipoSelector.h"
#include <TH2.h>
#include <TSystem.h>

namespace clas12root{
  
   tempHipoSelector::tempHipoSelector(TString filename) : HipoSelector(filename) {
   
  }
  tempHipoSelector::tempHipoSelector(HipoChain* chain) : HipoSelector(chain) {
  
  }

  void tempHipoSelector::Begin(TTree * /*tree*/)
  {
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    HipoSelector::Begin(0); //Do not remove this line!
  }
  
  void tempHipoSelector::SlaveBegin(TTree * /*tree*/)
  {
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    HipoSelector::SlaveBegin(0); //Do not remove this line!
    
  }
  Bool_t tempHipoSelector::Notify(){
    HipoSelector::Notify();
    //Add your code here
    //e.g.
    //if(_c12->rcdb()){
    // cout<<"Using beam energy for rcdb "<< _c12->rcdb()->current().beam_energy<<endl;
    //_beam.SetE( _c12->rcdb()->current().beam_energy/1000);
    }
  }
  
  void tempHipoSelector::AddFilter(){
    //   _c12->addExactPid(11,1);    //exactly 1 electron
   }

  Bool_t tempHipoSelector::ProcessEvent(){
    //Equivalent to TSelector Process
    //Fill in what you would like to do with
    //the data for each event....

    return kTRUE;
  }
 
 void tempHipoSelector::SlaveTerminate()
  {
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.
      
  }

  void tempHipoSelector::Terminate()
  {
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.

  }

    
}
