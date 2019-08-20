
#include "HipoRootAction.h"
#include "ParticleTree.h"
#include "clas12reader.h"
#include "clas12defs.h"
#include "region_particle.h"
#include <TH1.h>
#include <TMath.h>
#include <memory>
#include <cmath>

namespace clas12root{
  using namespace clas12;

  class clas12data {
  public:

    
    ClassDef(clas12data,1);
  };
  
  class event_data {
  public:

    float  BeamCharge=0;
    float  StartTime=0;
    float  RFTime=0;
    float  ProcTime=0;
    float  LiveTime=0;
    float  FTBStartTime;
    long   Trigger=0;
    long   Category=0;
    long   Topology=0;
    int    Helicity=0;
    int    HelicityRaw=0;
    int    EventNumber=0;
    int    onlineHelicity=0;
    int    flipHelicity=0;

    ClassDef(event_data,1);
  };
  
  class NNNNN : public HipoRootAction{

  private :
    std::vector<clas12data> *_treedata=nullptr;
    event_data *_eventdata=nullptr;
    
  public:
    
    void Action(clas12root::HipoROOTOut *hiprout) final{
      auto ptree=dynamic_cast<ParticleTree*>(hiprout);
      
      auto tree=ptree->Tree();
      if(!tree->GetBranch("CLAS12Data")){
	_treedata=new std::vector<clas12data>;
	tree->Branch("CLAS12Data",&_treedata);
	//USEEVENTDATA
	
     }
      clas12::clas12reader c12(ptree->NextFile().Data());
      std::vector<region_part_ptr>* particles=c12.getDetParticlesPtr();
      //EVENTDATABANKS

      //add Pid selections
 
      clas12data particleData;
     
      while(c12.next()==true){
 	if(ECCCC) continue;
	
	for(auto& p : *particles){
	  ////particleData.VVVVIIII| = XXXX;

	  if(PCCCC)_treedata->push_back(particleData);
	}
	//////Fill Event Data

	if(_treedata->size()) tree->Fill();
 	_treedata->clear();
     }
    }
    
  };

  
}
