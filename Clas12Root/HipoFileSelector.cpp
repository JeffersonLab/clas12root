#define HipoFileSelector_cxx
// The class definition in HipoFileSelector.h has been generated automatically
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


#include "HipoFileSelector.h"
#include <TH2.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include <chrono>
#include <thread>
#include <iostream>

namespace clas12root{
  
  HipoFileSelector::HipoFileSelector(TTree * ){

  }
  HipoFileSelector::HipoFileSelector(TString filename) : _chain(new HipoChain()) {
    _chain->Add(filename);
  }
  HipoFileSelector::HipoFileSelector(HipoChain* chain) : _chain(chain) {
  
  }

  // void HipoFileSelector::Run(){

  // }

  void HipoFileSelector::Begin(TTree * /*tree*/)
  {
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();

    
    if(!fInput){
      fInput=new TList();
      //  fInput->SetOwner(kTRUE);
    }
     
    if(!_chain){
      cout<<"Sorry no hipofiles given, exiting...."<<endl;
      exit(0);
    }
    if(!_chain->GetNFiles()){
      cout<<"Sorry no hipofiles given, exiting...."<<endl;
      exit(0);
    }
    
    fInput->Add(_chain);//make chain of files avaialbel on slaves
  }
  
  void HipoFileSelector::SlaveBegin(TTree * /*tree*/)
  {
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
    fInput->Print();
    _chain=dynamic_cast<HipoChain*>(fInput->FindObject("HIPOFILES"));
    //TString outdirstr=TString(outdir->GetTitle());
    cout<<"HipoFileSelector::SlaveBegin( "<<_chain<<endl;
    cout<<"HipoFileSelector::SlaveBegin( "<<_chain->GetNFiles()<<endl;

    _hist1=new TH1F("Time","Time",600,-100,500);
    fOutput->Add(_hist1);
  }

  Bool_t HipoFileSelector::Process(Long64_t entry)
  {
         
    _c12.reset(new clas12::clas12reader(_chain->GetFileName(entry).Data()));
    
    while(_c12->next()==true){
      ProcessEvent();
    }

    //cout<<"Done record "<<_iRecord<<" "<<_iFile<<endl;
    return kTRUE;
  }
  Bool_t HipoFileSelector::ProcessEvent(){

    _hist1->Fill(_c12->head()->getStartTime());
    return kTRUE;    
  }
  void HipoFileSelector::SlaveTerminate()
  {
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.
      
  }

  void HipoFileSelector::Terminate()
  {
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.

   TCanvas *c1 = new TCanvas("c1", "Proof ProofFirst canvas",200,10,400,400);
   _hist1 = dynamic_cast<TH1F*>(fOutput->FindObject("Time"));
   if (_hist1) _hist1->Draw();
   c1->Update();
  }

    
}
