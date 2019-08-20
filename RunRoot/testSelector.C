#define testSelector_cxx
// The class definition in testSelector.h has been generated automatically
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


#include "testSelector.h"
#include <TH2.h>
#include <TSystem.h>
#include <TCanvas.h>

namespace clas12root{
  
   testSelector::testSelector(TString filename) : HipoSelector(filename) {
   
  }
  testSelector::testSelector(HipoChain* chain) : HipoSelector(chain) {
  
  }

  void testSelector::Begin(TTree * /*tree*/)
  {
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    HipoSelector::Begin(0); //Do not remove this line!
  }
  
  void testSelector::SlaveBegin(TTree * /*tree*/)
  {
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).
    hmiss=new TH1F("missM","missM",200,-2,3);
    hm2g=new TH1F("m2g","m2g",200,0,1);
    hm2gCut=new TH1F("m2gCut","m2g",200,0,1);

    fOutput->Add(hmiss);
    fOutput->Add(hm2g);
    fOutput->Add(hm2gCut);
    
 
    HipoSelector::SlaveBegin(0); //Do not remove this line!
    
  }

  void testSelector::AddFilter(){
    _c12->addExactPid(11,1);    //exactly 1 electron
    _c12->addExactPid(211,1);    //exactly 1 pi+
    _c12->addExactPid(-211,1);    //exactly 1 pi-
    _c12->addExactPid(2212,1);    //exactly 1 proton
    _c12->addExactPid(22,2);    //exactly 2 gamma
  }
  
  void SetLorentzVector(TLorentzVector &p4,clas12::region_part_ptr rp){
    p4.SetXYZM(rp->par()->getPx(),rp->par()->getPy(),
	       rp->par()->getPz(),p4.M());
    
  }

  Bool_t testSelector::ProcessEvent(){
    //Equivalent to TSelector Process
    //Fill in what you would like to do with
    //the data for each event....
  
    auto electrons=_c12->getByID(11);
    auto gammas=_c12->getByID(22);
    auto protons=_c12->getByID(2212);
    auto pips=_c12->getByID(211);
    auto pims=_c12->getByID(-211);
    
    if(electrons.size()==1 && gammas.size()==2 && protons.size()==1 &&
       pips.size()==1 &&pims.size() == 1){
      
      // set the particle momentum
      SetLorentzVector(_el,electrons[0]);
      SetLorentzVector(_pr,protons[0]);
      SetLorentzVector(_g1,gammas[0]);
      SetLorentzVector(_g2,gammas[1]);
      SetLorentzVector(_pip,pips[0]);
      SetLorentzVector(_pim,pims[0]);
      
      TLorentzVector miss=_beam+_target-_el-_pr-_g1-_g2-_pip-_pim;
      hmiss->Fill(miss.M2());
      TLorentzVector pi0 = _g1+_g2;
      hm2g->Fill(pi0.M());
      if(TMath::Abs(miss.M2())<0.5)hm2gCut->Fill(pi0.M());
      
      //could also get particle time etc. here too
      //Double_t eTime=electrons[0]->sci(FTOF1A)->getTime();
    }
    return kTRUE;
  }
  
 void testSelector::SlaveTerminate()
  {
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.
      
  }

  void testSelector::Terminate()
  {
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.

    TCanvas* can=new TCanvas();
    can->Divide(2,1);
    can->cd(1);
    hmiss->DrawCopy();
    can->cd(2);
    hm2g->DrawCopy();
    hm2gCut->SetLineColor(2);
    hm2gCut->DrawCopy("same");
    
  }

    
}
