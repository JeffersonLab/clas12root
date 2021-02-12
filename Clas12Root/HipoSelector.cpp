#define HipoSelector_cxx
// The class definition in HipoSelector.h has been generated automatically
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


#include "HipoSelector.h"
#include <TH2.h>
#include <TSystem.h>

namespace clas12root{
  
  HipoSelector::HipoSelector(TTree * ){

  }
  HipoSelector::HipoSelector(TString filename) : _chain(new HipoChain()) {
    _chain->Add(filename);
  }
  HipoSelector::HipoSelector(HipoChain* chain) : _chain(chain) {
  }

  void HipoSelector::Begin(TTree * /*tree*/)
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
  
  void HipoSelector::SlaveBegin(TTree * /*tree*/)
  {
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).
    fInput->Print();
    TString option = GetOption();
    _chain=dynamic_cast<HipoChain*>(fInput->FindObject("HIPOFILES"));
    std::cout<<"CHAIN OPEN "<<endl;
    _chain->db()->initDBs();
  }
  Bool_t  HipoSelector::Notify() {
    // Called at the start of a new file
    //Rcdb(); //set rcdb info if exists
    //_chain->ConnectDataBases();
    AddFilter();
    return kTRUE;
  }
  
  Bool_t HipoSelector::Process(Long64_t entry)
  {
    //check if need new file
    _iRecord=entry-_NfileRecords; //get record to analyse,subtract records of previous files

    //Check if we need a new file
    if( _iRecord>=_NcurrRecords ){
      _iFile=_chain->GetFileFromRecord(entry);
      _NfileRecords=_chain->GetRecordsToHere(_iFile); //Add records from previous file to give offset
   
      _c12.reset(new clas12::clas12reader{*_chain->GetC12Reader(),_chain->GetFileName(_iFile).Data(),_chain->ReaderTags()});
      _c12->connectDataBases(_chain->db());
      _NcurrRecords= _c12->getReader().getNRecords(); //records in this file
      _iRecord=entry-_NfileRecords; //get first record in this file to process

      if(_c12.get()){
	Notify();
      }
      
     }
 
    //load a record from the file
    _c12->getReader().loadRecord(_iRecord);
    
    while(_c12->nextInRecord()==true){
      //abtract function to be supplied by user base class
      ProcessEvent();
    }
    return kTRUE;
  }

  void HipoSelector::SlaveTerminate()
  {
    //Get accumulated charge for each and add to fOutput to concatenate
#ifdef CLAS_QADB
    if(qadb()!=nullptr){
      auto chargeHist=new TH1D("accumulatedCharge","accumulatedCharge",1,0,1);
      chargeHist->SetBinContent(1,qadb()->getAccCharge());
      fOutput->Add(chargeHist);
    }
#endif
    
  }
  
  void HipoSelector::Terminate()
  {
    
#ifdef CLAS_QADB
    //Get total charge accumulated charge
    if(qadb()!=nullptr){
      auto chargeHist=dynamic_cast<TH1D*>(fOutput->FindObject("accumulatedCharge")); 
      _chain->SetTotalBeamCharge(chargeHist->GetBinContent(1));
      std::cout<<"Total accumulated charge: "<<chargeHist->GetBinContent(1)<<std::endl;
    }
#endif

  }
  
  
}
