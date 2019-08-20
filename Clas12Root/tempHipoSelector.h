#ifndef tempHipoSelector_h
#define tempHipoSelector_h

#include "HipoSelector.h"


namespace clas12root{
   
  class tempHipoSelector : public HipoSelector {
      public :


      tempHipoSelector() =default;
      tempHipoSelector(TString filename) ;
      tempHipoSelector(HipoChain *chain) ;
      virtual ~tempHipoSelector() { }
      void    Begin(TTree *tree) final;
      void    SlaveBegin(TTree *tree) final;
      
      Bool_t ProcessEvent() final;
      
      void    SlaveTerminate() final;
      void    Terminate() final;

      void AddFilter() override;
 
  private:

      
      ClassDefOverride(clas12root::tempHipoSelector,0);

    };
}


#endif // #ifdef tempHipoSelector_cxx
