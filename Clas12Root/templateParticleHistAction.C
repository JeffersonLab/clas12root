
#include "HipoRootAction.h"
#include "ParticleHist.h"
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
      auto phist=dynamic_cast<ParticleHist*>(hiprout);

      auto hists=phist->Hists();
      clas12::clas12reader c12(phist->NextFile().Data());
      c12.setEntries(NENTRIESTOPROCESS);
      std::vector<region_part_ptr>* particles=c12.getDetParticlesPtr();

      while(c12.next()==true){
	for(auto& p : *particles){
	  ////if(CCCC)hists->at(IIII)->Fill(XXXX);
	}
     }
    }
 
    
  };
}
