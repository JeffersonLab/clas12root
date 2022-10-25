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

void FTElectronCorrection(TLorentzVector& el){
	auto p_electron_pre_correction = el;
	double p_mag = p_electron_pre_correction.P();
	double p_mag_corrected = (p_mag-0.03689+0.1412*p_mag-0.04316*pow(p_mag,2)+0.007046*pow(p_mag,3)-0.0004055*pow(p_mag,4))/p_mag;
	el.SetXYZM(el.X()*p_mag_corrected,el.Y()*p_mag_corrected,el.Z()*p_mag_corrected,0.00051099891);
}
 
void WillLambda(){

  //some particles 4-vectors
  auto db=TDatabasePDG::Instance();
  Double_t elMass=db->GetParticle(11)->Mass();
  TLorentzVector beam(0,0,10.594,sqrt(elMass*elMass+10.594*10.594));
  TLorentzVector target(0,0,0,db->GetParticle(2212)->Mass());
  TLorentzVector p4el(0,0,0,db->GetParticle(11)->Mass());
  TLorentzVector p4pr(0,0,0,db->GetParticle(2212)->Mass());
  TLorentzVector p4kp(0,0,0,db->GetParticle(321)->Mass());
  TLorentzVector p4pim(0,0,0,db->GetParticle(-211)->Mass());

  //some histograms 
  TH1F hppim("ppimMass","P#pi- Mass", 200, 0.9, 2.0);
  TH1F hlambdamass("LambdaMass","#Lambda Mass", 200, 0.9, 2.0);
  TH1F hmissingMass("missingMass","Missing Mass", 100, -1.5, 1.5);
  TH1F hmissingMassSelection("missingMassSelection","Selected Missing Mass", 100, -1.5, 1.5);
  TH1F hmissingLambda("MissingLambda","Missing Mass with e- and K+",200,0.9,2.0);
  TH1F hP("p","p",1000,-10,10);
  //create Hipo chain for processing data
  clas12root::HipoChain chain;
  chain.Add("/cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v1/dst/train/eK+/*.hipo");
  //chain.Add("/hdd/jlab/clas12data/skim3_00564*.hipo");
  chain.db()->turnOffQADB();

  auto config_c12=chain.GetC12Reader();
  chain.SetReaderTags({0});  //create clas12reader with just tag 0 events

   //choose e- detector region
  auto elRegion=FD;
  auto kpPmax=2.0;
  ////////////c12->useFTBased();elRegion=FT;kpPmax=9.0;//use this line to switch to FT
  //create filter
  config_c12->addAtLeastPid(11,1);    //at least 1 electrons
  config_c12->addExactPid(2212,1);    //exactly 1 proton
  config_c12->addExactPid(321,1);     //exactly 1 K+
  config_c12->addExactPid(-211,1);    //exacty  1 pi-
  //and anything else

  gBenchmark->Start("lambda");
  //now get reference to (unique)ptr for accessing data in loop
  //this will point to the correct place when file changes
  auto& c12=chain.C12ref();

  while (chain.Next()){ //Loop over events
   // get particles by type, filter garauntees [0]
    auto electron =c12->getByID(11)[0];
    auto kp       =c12->getByID(321)[0];

    //initial cuts K+P<2; K+ and e- in FD
    //initial cuts K+P<9; K+ and e- in FT
     if (kp->getP()<kpPmax && kp->getRegion()==FD && electron->getRegion()==elRegion){
      //Get the other particles
      auto pim     =c12->getByID(-211)[0];
      auto proton  =c12->getByID(2212)[0];
       
      SetLorentzVector(p4el,electron);
      SetLorentzVector(p4pr,proton);
      SetLorentzVector(p4kp,kp);
      SetLorentzVector(p4pim,pim);

      if(elRegion==FT) FTElectronCorrection(p4el);
       
      auto p4lambda = p4pr + p4pim; //Lambda
      auto missParticle= beam + target - p4el - p4pr - p4kp - p4pim;//miss exclusive
      auto missElectron= beam + target - p4pr - p4kp - p4pim;//miss exclusive
      auto missLambda  = beam + target - p4el - p4kp;//miss exclusive
      
      hppim.Fill(p4lambda.M());
      hmissingMass.Fill(missParticle.M2());
      hP.Fill(missParticle.P());
      
      //FT electron cut
      if (elRegion==FT && !(TMath::Abs(missParticle.M2()) < 0.05
			    && missParticle.P()<2 && p4el.P() >0.7))
	continue;
      //FD electron cut
      if (elRegion==FD && !(TMath::Abs(missParticle.M2()) < 0.05
			    && missParticle.P()<0.1))
	continue;

      //////////////////////////////////////////////////////	
      hmissingMassSelection.Fill(missParticle.M2());
      if(missLambda.M2()>0) {
	double missingLambda_mass = missLambda.M();
	hmissingLambda.Fill(missingLambda_mass);
	hlambdamass.Fill(p4lambda.M());
	
      }	
      
    }
  }//end event loop
  gBenchmark->Stop("lambda");
  gBenchmark->Print("lambda");

  auto* rootFile=TFile::Open("lambda.root","recreate");
  hP.Write();
  hppim.Write();
  hlambdamass.Write();
  hmissingMass.Write();
  hmissingMassSelection.Write();
  hmissingLambda.Write();
  delete rootFile;//save histograms
}
