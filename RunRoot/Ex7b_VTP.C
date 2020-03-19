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

using namespace clas12;


void Ex7b_VTP(){
 
 
  int counter=0;
 
   
  clas12reader c12("/work/jlab/clas12data/pass0/skim3_005424.hipo",{0});
     //  clas12reader c12(files->At(i)->GetTitle(),{0});//add tags {tag1,tag2,tag3,...}
  
      //Add some event Pid based selections
      //////////c12.AddAtLeastPid(211,1); //at least 1 pi+
      //c12.addExactPid(11,1);    //exactly 1 electron
      //c12.addExactPid(211,1);    //exactly 1 pi+
      //c12.addExactPid(-211,1);    //exactly 1 pi-
      //c12.addExactPid(2212,1);    //exactly 1 proton
      //c12.addExactPid(22,2);    //exactly 2 gamma
      //////c12.addZeroOfRestPid();  //nothing else
      //////c12.useFTBased(); //and use the Pids from RECFT

     //can also access the integrated current at this point
     //c12.scalerReader();//must call this first
     //c12.getRunBeamCharge();
     
      while(c12.next()==true){
	//can get an estimate of the beam current to this event
	//c12.getCurrApproxCharge();//if called c12.scalerReader();
	
        //c12.event()->getStartTime();

	if(c12.checkTriggerBit(24)){
	  cout<<"Got mesonex trigger "<<endl;
	  for(Int_t ivtp=0;ivtp<c12.vtp()->getRows();ivtp++){
	
	    cout<<ivtp<<" VTP word "<<c12.vtp()->getWord(ivtp)<<" "<<" VTP crate "<<c12.vtp()->getCrate(ivtp)<<" "<<endl;
	    cout<<"VTP trigger pattern "<<c12.vtp()->makeVTPTriggers()<<endl;
	    cout<<"VTP trigger BIT 10"<<c12.checkVTPTriggerBit(10)<<endl;
	  }
	}
       
	counter++;
      }
      
   

  
  
}
