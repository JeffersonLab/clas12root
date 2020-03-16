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

void Ex7_ScalerReader(){
  // Record start time
  auto start = std::chrono::high_resolution_clock::now();


  /////////////////////////////////////
  //ignore this just getting file name!
   TString inputFile;
   TString outputFile;

   for(Int_t i=1;i<gApplication->Argc();i++){
    TString opt=gApplication->Argv(i);
    if((opt.Contains(".hipo"))){
      inputFile=opt(5,opt.Sizeof());
    }
   }
   if(inputFile==TString())  {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(0);
   }
   /////////////////////////////////////

   
   cout<<"Analysing hipo file "<<inputFile<<endl;

   TChain fake("hipo");
   fake.Add(inputFile.Data());
   //get the hipo data
   //   reader.open(inputFile.Data());
   auto files=fake.GetListOfFiles();

  
   gBenchmark->Start("timer");
   int counter=0;
 
   
   for(Int_t i=0;i<files->GetEntries();i++){
     //create the event reader
     //just use tag 0 events
     clas12reader c12(files->At(i)->GetTitle(),{0});

     //create the scaler reader and make some counters
     //the counters have an entry for each scaler read
     //you can increment them for each real event corresponding
     //to the scaler read in the normal event loop
     auto scal=c12.scalerReader();
     auto count_mesonex=scal->addLongCounter();
     auto count_events=scal->addLongCounter();
     
     cout<<"Charge "<< c12.getRunBeamCharge()<<endl;

      
     while(c12.next()==true){
   
       auto currEvent=c12.runconfig()->getEvent();
       
       //check for valid charge events, i.e. RUN::Scaler::BeamCharge!<0
       if( scal->validCharge(currEvent) ==false ) continue;
 
       if( c12.checkTriggerBit(24) )
	 scal->incrementLong(count_mesonex,currEvent,1);

       scal->incrementLong(count_events,currEvent,1);

       counter++;
     }

     //can get the difference in accumulated charge
     //between each scaler read
     auto charges = scal->getDeltaCharges();
     //can get vectors for the counters
     auto mesonex_rates = scal->getLongCounter(count_mesonex);
     auto all_rates = scal->getLongCounter(count_events);
     Double_t totCharge=0;
     Double_t totAll=0;
     Double_t totMesonEx=0;
     for(UInt_t i=0;i<charges.size();i++){
       if( scal->validChargePos(i)){
	 totCharge+=charges[i];
	 totAll+=all_rates[i];
	 totMesonEx+=mesonex_rates[i];
       }
       //cout<<charges[i]<<" "<<all_rates[i]<<" "<<mesonex_rates[i]<<endl;
     }
     cout<<"Charge compare "<<totCharge<<" "<<scal->getBeamCharge()<<endl;
     cout<<"Rates all "<<totAll/totCharge<<" "<<totMesonEx/totCharge<<endl;
   }
   gBenchmark->Stop("timer");
   gBenchmark->Print("timer");
   
   auto finish = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = finish - start;
   std::cout << "Elapsed time: " << elapsed.count()<< " events = "<<counter<< " s\n";

}
