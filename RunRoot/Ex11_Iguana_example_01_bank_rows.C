#include "clas12reader.h"
#include "HipoChain.h"
#include <hipo4/reader.h>
#include <iguana/algorithms/clas12/MomentumCorrection/Algorithm.h>

void Ex11_Iguana_example_01_bank_rows(){
  // clas12databases::SetRCDBRootConnection("rcdb.root");
   
  clas12root::HipoChain chain;
  chain.Add("/home/dglazier/Jlab/clas12/data/hipo/DVPipPimP_006733.hipo");
  chain.SetReaderTags({0});  //create clas12reader with just tag 0 events
  auto config_c12=chain.GetC12Reader();
  config_c12->addExactPid(11,1);    //exactly 1 electron
  config_c12->addExactPid(211,1);    //exactly 1 pi+
  config_c12->addExactPid(-211,1);    //exactly 1 pi-

  // create the algorithms
  iguana::clas12::MomentumCorrection algo_momentum_correction;
  // set log levels
  algo_momentum_correction.SetOption("log", "debug");
  // start the algorithms
  algo_momentum_correction.Start();

  //get run clas12reader
  auto& c12=chain.C12ref();
  int counter =0;

  //decalre 4-vector objects
  auto db=TDatabasePDG::Instance();
  TLorentzVector p4beam(0,0,10.6,10.6);
  TLorentzVector p4target(0,0,0,db->GetParticle(2212)->Mass());
  TLorentzVector p4el(0,0,0,db->GetParticle(11)->Mass());
  TLorentzVector p4pip(0,0,0,db->GetParticle(211)->Mass());
  TLorentzVector p4pim(0,0,0,db->GetParticle(-211)->Mass());

  //define mometnum correction function taking a region_particle and a lorentzvector
  auto momCorrection = [&algo_momentum_correction,&c12](region_particle *p, TLorentzVector& p4){
    
    auto [px, py, pz] = algo_momentum_correction.Transform(p->getPx(),p->getPy(),p->getPz(),p->getSector(),p->getPid(),c12->runconfig()->getTorus());
    
    p4.SetXYZM(px,py,pz,p4.M());
  };

  //loop over all events
      
  while (chain.Next()){
    //auto ebeam = c12->rcdb()->current().beam_energy/1000;
    // p4beam.SetXYZT(0,0,ebeam,ebeam); //approx. mass =0
    // get particles by type
    // note we applied a filter to ensure size of all ==1
    auto electron=c12->getByID(11)[0];
    auto pip=c12->getByID(211)[0];
    auto pim=c12->getByID(-211)[0];

    momCorrection(electron,p4el);
    momCorrection(pip,p4pip);
    momCorrection(pim,p4pim);
    cout<<"el px "<<electron->getPx()<<" to "<<p4el.X()<<endl;
    cout<<"el py "<<electron->getPy()<<" to "<<p4el.Y()<<endl;
    cout<<"el pz "<<electron->getPz()<<" to "<<p4el.Z()<<endl;
    counter++;

  }

}
