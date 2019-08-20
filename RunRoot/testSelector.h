#ifndef testSelector_h
#define testSelector_h

#include "HipoSelector.h"
#include <TLorentzVector.h>
#include <TH1F.h>


namespace clas12root{
   
  class testSelector : public HipoSelector {
      public :


      testSelector() =default;
      testSelector(TString filename) ;
      testSelector(HipoChain *chain) ;
      virtual ~testSelector() { }
      void    Begin(TTree *tree) final;
      void    SlaveBegin(TTree *tree) final;
      
      Bool_t ProcessEvent() final;
      
      void    SlaveTerminate() final;
      void    Terminate() final;

      void AddFilter() override;
      
  private:

      TLorentzVector _beam{0,0,10.6,10.6};
      TLorentzVector _target{0,0,0,0.93827200};
      TLorentzVector _el{0,0,0,0.00051099891};
      TLorentzVector _pr{0,0,0,0.93827200};
      TLorentzVector _g1{0,0,0,0};
      TLorentzVector _g2{0,0,0,0};
      TLorentzVector _pip{0,0,0,0.13957000};
      TLorentzVector _pim{0,0,0,0.13957000};

      TH1F *hmiss=nullptr;
      TH1F *hm2g=nullptr;
      TH1F *hm2gCut=nullptr;
      
      ClassDefOverride(clas12root::testSelector,0);

    };
}


#endif // #ifdef testSelector_cxx
