#ifndef HipoSelector_h
#define HipoSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

#include <TString.h>
#include <TH1.h>
#include <vector>
#include <iostream>

// Headers needed by this particular selector
#include "HipoChain.h"
#include "clas12reader.h"

namespace clas12root{
  using std::cout;
  using std::endl;
  
  
  class HipoSelector : public TSelector {
      public :


    HipoSelector(TTree * /*tree*/ =0);
      HipoSelector(TString filename) ;
      HipoSelector(HipoChain *chain) ;
      virtual ~HipoSelector() { }
      Int_t   Version() const final{ return 2; }
      
      void    Begin(TTree *tree) override;
      void    SlaveBegin(TTree *tree) override;
      Bool_t  Process(Long64_t entry) override;
     
      virtual Bool_t ProcessEvent() =  0; //loop action to be defined in derived class

      virtual void AddFilter(){};
 
  protected:
      
      std::unique_ptr<clas12::clas12reader> _c12;//!
 
    private:

      HipoChain* _chain=nullptr;

      Int_t _NcurrRecords=0;
      Int_t _iRecord=0;
      Int_t _iFile=0;
      Long64_t _NfileRecords=0;
      
       
      ClassDefOverride(clas12root::HipoSelector,0);

    };
}


#endif // #ifdef HipoSelector_cxx
