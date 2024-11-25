#include <cstdlib>
#include <iostream>
#include <chrono>
#include <TFile.h>
#include <TTree.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include "clas12reader.h"
#include "HipoChain.h"

using namespace clas12;


void SetLorentzVector(TLorentzVector &p4,clas12::region_part_ptr rp){
  p4.SetXYZM(rp->par()->getPx(),rp->par()->getPy(),
	      rp->par()->getPz(),p4.M());

}

void Ex1_CLAS12ReaderChain(){


   //some particles
   auto db=TDatabasePDG::Instance();
   TLorentzVector beam(0,0,10.6,10.6);
   TLorentzVector target(0,0,0,db->GetParticle(2212)->Mass());
   TLorentzVector el(0,0,0,db->GetParticle(11)->Mass());
   TLorentzVector pr(0,0,0,db->GetParticle(2212)->Mass());
   TLorentzVector g1(0,0,0,0);
   TLorentzVector g2(0,0,0,0);
   TLorentzVector pip(0,0,0,db->GetParticle(211)->Mass());
   TLorentzVector pim(0,0,0,db->GetParticle(-211)->Mass());

   auto* hmiss=new TH1F("missM","missM",200,-2,3);
   auto* hm2g=new TH1F("m2g","m2g",200,0,1);
   auto* hm2gCut=new TH1F("m2gCut","m2g",200,0,1);
   
   gBenchmark->Start("timer");
   int counter=0;
   
   clas12root::HipoChain chain;
   chain.Add("tests/data/EptopX2pi_sample.hipo");
   chain.Add("tests/data/EptopX2pi_sample.hipo");
   chain.Add("tests/data/EptopX2pi_sample.hipo");
   //////////////////////////////////////
 
   auto config_c12=chain.GetC12Reader();
   //Add some event Pid based selections
   //////////config_c12->AddAtLeastPid(211,1); //at least 1 pi+
   // config_c12->addExactPid(11,1);    //exactly 1 electron
   // config_c12->addExactPid(211,1);    //exactly 1 pi+
   // config_c12->addExactPid(-211,1);    //exactly 1 pi-
   // config_c12->addExactPid(2212,1);    //exactly 1 proton
   // config_c12->addExactPid(22,2);    //exactly 2 gamma
   //////config_c12->addZeroOfRestPid();  //nothing else

   
   //////config_c12->useFTBased(); //and use the Pids from RECFT
   //////chain.db()->turnOffQADB(); //particularly useful if does not exist for run period
   //////config_c12->SetRecParticleOnly(); //for faster processing of just REC::Particle
   //////config_c12->ignoreBank("REC::CovMat"); //or just turn off chosen banks

   
   //now get reference to (unique)ptr for accessing data in loop
   //this will point to the correct place when file changes
   auto& c12=chain.C12ref();
   
   while (chain.Next()){
     
     //c12->event()->getStartTime();
     
     //Loop over all particles to see how to access detector info.
     
     for(auto& p : c12->getDetParticles()){
       //  get predefined selected information
	 p->getTime();
	 p->getDetEnergy();
	 p->getDeltaEnergy();
	 //check trigger bits
	 //	 if(c12->checkTriggerBit(25)) cout<<"MesonExTrigger"<<endl;
	 //	 else cout<<"NOT"<<endl;
	 
	 // get any detector information (if exists for this particle)
	 // there should be a get function for any entry in the bank
	 switch(p->getRegion()) {
	 case FD :
	   
	   p->cal(PCAL)->getEnergy();	   
	   p->cal(ECIN)->getEnergy();
	   p->cal(ECOUT)->getEnergy();
	   
	   p->sci(FTOF1A)->getEnergy();
	   p->sci(FTOF1B)->getEnergy();
	   p->sci(FTOF2)->getEnergy();
	   	   
	   p->trk(DC)->getSector();
	   // p->utrk(CVT)->getSector();//for pass 2
	   
	   p->che(HTCC)->getNphe();
	   p->che(LTCC)->getNphe();
 
	   //trajectories
	   p->traj(LTCC)->getX();
	   
	   p->traj(DC,DC1)->getCx();; //First layer of DC
	   break;
	 case FT :
	   p->ft(FTCAL)->getEnergy();
	   p->ft(FTHODO)->getEnergy();
	   break;
	 case CD:
	   p->sci(CTOF)->getEnergy();
	   p->sci(CND)->getEnergy();
	   break;
	 }
	 //   covariance matrix (comment in to see!)
	 // p->covmat()->print();
	 //p->cmat();
     }

     // get particles by type
     auto electrons=c12->getByID(11);
     auto gammas=c12->getByID(22);
     auto protons=c12->getByID(2212);
     auto pips=c12->getByID(211);
     auto pims=c12->getByID(-211);
       
     if(electrons.size()==1 && gammas.size()==2 && protons.size()==1 &&
	pips.size()==1 &&pims.size() == 1){
       
       // set the particle momentum
       SetLorentzVector(el,electrons[0]);
       SetLorentzVector(pr,protons[0]);
       SetLorentzVector(g1,gammas[0]);
       SetLorentzVector(g2,gammas[1]);
       SetLorentzVector(pip,pips[0]);
       SetLorentzVector(pim,pims[0]);
	
       TLorentzVector miss=beam+target-el-pr-g1-g2-pip-pim;
       hmiss->Fill(miss.M2());
       TLorentzVector pi0 = g1+g2;
       hm2g->Fill(pi0.M());
       if(TMath::Abs(miss.M2())<0.5)hm2gCut->Fill(pi0.M());

       //could also get particle time etc. here too
       //Double_t eTime=electrons[0]->sci(FTOF1A)->getTime();

       //if interested in vertdoca
       //this example checks proton pi-
       // auto vdentry=c12->vertdoca()->getCombinationEntry(protons[0]->getIndex(),pims[0]->getIndex());
       // cout<<"vertdoca : "vdentry<<" proton index "<<protons[0]->getIndex()
       // 	   <<" pi- index "<<pims[0]->getIndex()<<" vertdoca indices : "
       // 	   <<c12->vertdoca()->getIndex1(vdentry)<<" "
       // 	   <<c12->vertdoca()->getIndex2(vdentry)<<" proton track index1 ?= "
       // 	   <<c12->vertdoca()->isTrack1(protons[0]->getIndex(),vdentry)<<endl;
       
     
     }
    
       
     counter++;
   }
   auto bcharge = chain.TotalBeamCharge();
   cout<<"Number of Events = " <<counter<<" total charge = "<<bcharge<<endl;

   gBenchmark->Stop("timer");
   gBenchmark->Print("timer");
   TCanvas* can=new TCanvas();
   can->Divide(2,1);
   can->cd(1);
   hmiss->DrawCopy();
   can->cd(2);
   hm2g->DrawCopy();
   hm2gCut->SetLineColor(2);
   hm2gCut->DrawCopy("same");
  
  
} 
