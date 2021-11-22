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

void ExMC_CLAS12ReaderChain(){


 
   auto* hPDiff=new TH1F("PDiff","PDiff",200,-10,10);
   auto* hThDiff=new TH1F("ThDiff","ThDiff",200,-180,180);
   auto* hPhDiff=new TH1F("PhDiff","PhDiff",200,-360,360);
  
   gBenchmark->Start("timer");
   int counter=0;
   
   clas12root::HipoChain chain;
   chain.Add("/w/work5/jlab/hallb/clas12/rg-b/simulations/n2eFPS_wRafoTruthmatching/eD_n2eFPS_Recon_9.hipo");
   // chain.Add("/WHERE/IS/MY/HIPO/file*.hipo");
   
   //////////////////////////////////////
  //To creat rcdb data RCDB_HOME must be set prior to installation
  //chain.WriteRcdbData("rcdb.root"); //Must use this first time to create local copy
  //Then when we have local copy can just use the following
  //chain.SetRcdbFile("rcdb.root");

   auto config_c12=chain.GetC12Reader();
   chain.SetReaderTags({0});  //create clas12reader with just tag 0 events
   //config_c12->scalerReader();//if you want integrated charge
   //Add some event Pid based selections
   //////////config_c12->AddAtLeastPid(211,1); //at least 1 pi+
   //config_c12->addExactPid(11,1);    //exactly 1 electron
   //config_c12->addExactPid(211,1);    //exactly 1 pi+
   //config_c12->addExactPid(-211,1);    //exactly 1 pi-
   //config_c12->addExactPid(2212,1);    //exactly 1 proton
   //config_c12->addExactPid(22,2);    //exactly 2 gamma
   //////config_c12->addZeroOfRestPid();  //nothing else
   //////config_c12->useFTBased(); //and use the Pids from RECFT

   //now get reference to (unique)ptr for accessing data in loop
   //this will point to the correct place when file changes
   auto& c12=chain.C12ref();
   
   while (chain.Next()){
     //loop over all reconstructed particles
     for(auto p : c12->getDetParticles()){

       if(p->mc()->isMatched()){//this particle has an mc match
	 cout<<p->par()->getEntry()<<" rec pid "<<p->par()->getPid()<<" "<<p->mc()->getPid()<<" match pindex "<<p->mc()->getMatch()->getPindex()<<" mcindex "<<" "<<p->mc()->getMatch()->getQuality()<<" "<<p->mc()->getMatch()->getMCindex()<<endl;
	 if(p->mc()->getMatch()->getQuality()>0.9){
	   hPDiff->Fill(p->getMCPDiff());
	   hThDiff->Fill(p->getMCThetaDiff()*TMath::RadToDeg());
	   hPhDiff->Fill(p->getMCPhiDiff()*TMath::RadToDeg());
	 }
       }
     }
     counter++;
   }
   cout<<"Number of Events = " <<counter<<endl;

   gBenchmark->Stop("timer");
   gBenchmark->Print("timer");
   TCanvas* can=new TCanvas();
   can->Divide(3,1);
   can->cd(1);
   hPDiff->Draw();
   can->cd(2);
   hThDiff->Draw();
   can->cd(3);
   hPhDiff->Draw();
  
  
}
