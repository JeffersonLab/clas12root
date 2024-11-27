#include "clas12reader.h"
#include "region_particle.h"
#include "HipoChain.h"
#include "Iguana.h"
#include <hipo4/reader.h>
#include <TBenchmark.h>
#include <TLorentzVector.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <Math/Vector4D.h>
#include <TH1.h>
#include <TCanvas.h>

void Ex11_Iguana_MomentumCorrection(){

  using FourVector = ROOT::Math::PxPyPzMVector;
  // clas12databases::SetRCDBRootConnection("rcdb.root");
  gBenchmark->Start("iguana");
  //create hipo chain for looping over events
  clas12root::HipoChain chain;
  chain.Add("/home/dglazier/Jlab/clas12/data/hipo/DVPipPimP_006733.hipo");
  chain.SetReaderTags({0});  //create clas12reader with just tag 0 events
  //add some filters
  auto config_c12=chain.GetC12Reader();
  config_c12->addAtLeastPid(11,1);    //exactly 1 electron
  config_c12->addAtLeastPid(211,1);    //exactly 1 pi+
  config_c12->addAtLeastPid(-211,1);    //exactly 1 pi-
  config_c12->ignoreBank("REC::CovMat");

  
  // create the chosen algorithms
  clas12root::Iguana ig;
  ig.SetClas12(chain.C12ptr());//connect to clas12reader
  ig.GetTransformers().Use("clas12::MomentumCorrection");
  ig.GetTransformers().Use("clas12::FTEnergyCorrection");
  ig.GetFilters().Use("clas12::ZVertexFilter");
  ig.GetCreators().Use("physics::InclusiveKinematics");
  
  ig.SetOptionAll("log", "debug");
  ig.Start();
 
 
  //decalre 4-vector objects
  auto db=TDatabasePDG::Instance();
  FourVector p4beam(0,0,10.6,10.6);
  FourVector p4target(0,0,0,db->GetParticle(2212)->Mass());
  FourVector p4el(0,0,0,db->GetParticle(11)->Mass());
  FourVector p4pip(0,0,0,db->GetParticle(211)->Mass());
  FourVector p4pim(0,0,0,db->GetParticle(-211)->Mass());
  TLorentzVector tp4el(0,0,0,db->GetParticle(11)->Mass());

  TH1D hQ2{"Q2","Q^{2}",100,0,10};
  TH1D hx{"x","x",100,0,1};
  TH1D hy{"y","y",100,0,1};
  TH1D hW{"W","W",100,0,6};
  TH1D hcQ2{"cQ2","corrected Q^{2}",100,0,10};
  TH1D hcx{"cx","corrected x",100,0,1};
  TH1D hcy{"cy","corrected y",100,0,1};
  TH1D hcW{"cW","corrected W",100,0,6};

  
  //get run clas12reader
  auto& c12=chain.C12ref();
  while ( chain.Next() ){
    ig.PrepareEvent(c12->getRunNumber());
    
    //auto ebeam = c12->rcdb()->current().beam_energy/1000;
    //p4beam.SetXYZT(0,0,ebeam,ebeam); //approx. mass =0
    //filter on z-vertices of the particles
    //filter on fiducial cuts
    //filter on PhotonGBT
    ig.GetFilters().doAllFilters();

 
    //Check if particles still exist after filters
    //and if they do assign them
    auto getFirstParticle=[&c12](short pdg)->clas12::region_particle*{
      auto particles=c12->getByID(pdg);
      if(particles.size()>0){
	return particles[0];
      }
      return nullptr;
    };
    
    auto electron=getFirstParticle(11);
    if(electron==nullptr) continue;
    auto pip=getFirstParticle(211);
    if(pip==nullptr) continue;
    auto pim=getFirstParticle(-211);
    if(pim==nullptr) continue;
 
    //correct momentum and get 4-vectors
    ig.GetTransformers().doAllCorrections({electron,pip,pim},{&p4el,&p4pip,&p4pim});

    //calculate inclusive kinematics
    auto kine = ig.GetCreators().doInclusiveKinematics(electron);
    auto corrkine = ig.GetCreators().doInclusiveKinematics(p4el);

    hQ2.Fill(kine.Q2);
    hx.Fill(kine.x);
    hy.Fill(kine.y);
    hW.Fill(kine.W);
    
    hcQ2.Fill(corrkine.Q2);
    hcx.Fill(corrkine.x);
    hcy.Fill(corrkine.y);
    hcW.Fill(corrkine.W);

    //if you unfortunately prefer TLorentzVector, there is a transform function to help.
    // ig.GoodToBad(p4el,tp4el);
    // cout<<"check good to bad "<<p4el.T()<<" = "<<tp4el.E()<<endl;
  }
  ig.Stop();
  gBenchmark->Stop("iguana");
  gBenchmark->Print("iguana");

  auto* can = new TCanvas();
  can->Divide(2,2);
  
  can->cd(1);
  hQ2.DrawCopy();
  hcQ2.SetLineColor(2);
  hcQ2.DrawCopy("same");
  
  can->cd(2);
  hW.DrawCopy();
  hcW.SetLineColor(2);
  hcW.DrawCopy("same");
  
  can->cd(3);
  hx.DrawCopy();
  hcx.SetLineColor(2);
  hcx.DrawCopy("same");
  
  can->cd(4);
  hy.DrawCopy();
  hcy.SetLineColor(2);
  hcy.DrawCopy("same");

  std::cout<<"analysed "<<hQ2.GetEntries()<<" events "<<std::endl;
}
