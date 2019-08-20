
#include "HipoRootAction.h"
#include "BankHist.h"
#include "clas12reader.h"
#include "clas12defs.h"
#include "region_particle.h"
#include <TH1.h>
#include <TMath.h>
#include <memory>
#include <cmath>

namespace clas12root{
  using namespace clas12;

  class NNNNN : public HipoRootAction{
  
    void Action(clas12root::HipoROOTOut *hiprout) final{
      auto bhist=dynamic_cast<BankHist*>(hiprout);

      auto hists=bhist->Hists();
   
      hipo::reader  reader;
      reader.open(bhist->NextFile().Data());

      hipo::dictionary  factory;
      reader.readDictionary(factory);

      clas12::BBBB bank(factory.getSchema(bhist->GetBankName().Data()));
      hipo::event      event;
 
      while(reader.next()==true){
	reader.read(event); //read event
	event.getStructure(bank); //get particle data

	int nrows = bank.getRows();
     	for(int i = 0; i < nrows; i++){
	  bank.setBankEntry(i);
	  ////if(CCCC)hists->at(IIII)->Fill(XXXX);

	}
     }
    }
 
    
  };
}
