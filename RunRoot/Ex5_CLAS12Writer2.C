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
#include "clas12writer.h"

using namespace clas12;


void SetLorentzVector(TLorentzVector &p4,clas12::region_part_ptr rp){
  p4.SetXYZM(rp->par()->getPx(),rp->par()->getPy(),
	      rp->par()->getPz(),p4.M());

}

void Ex5_CLAS12Writer2(){
  // Record start time
  auto start = std::chrono::high_resolution_clock::now();


  /////////////////////////////////////
  //ignore this just getting input and output file names!
  std::string inFile;
  std::string outputFile;


  std::cout<<"*** please type in an input file name"<<std::endl;
  getline (cin, inFile);
  if(inFile.find(".hipo")==std::string::npos){
    std::cout<<" *** the input file must be a hipo file..."<<std::endl;
    exit(0);
  }
  std::cout<<"*** please type in an output file name"<<std::endl;
  getline (cin, outputFile);
  if(outputFile.find(".hipo")==std::string::npos){
    std::cout<<" *** the output file must be a hipo file..."<<std::endl;
    exit(0);
  }

  /////////////////////////////////////

  //initialising clas12writer with path to output file
  clas12writer c12writer(outputFile.c_str());

  //can as writer not to write certain banks
  c12writer.skipBank("REC::Cherenkov");
  c12writer.skipBank("REC::Traj");
  c12writer.skipBank("REC::CovMat");

  cout<<"Analysing hipo file "<<inFile<<endl;

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
   
  gBenchmark->Start("timer");
 
  int counter = 0;
  int writeCounter = 0;
   

  //create the event reader
  clas12reader c12(inFile.c_str());

  //assign a reader to the writer
  c12writer.assignReader(c12);
     
      
  while(c12.next()==true){

    // get particles by type
    auto electrons=c12.getByID(11);
    auto gammas=c12.getByID(22);
    auto protons=c12.getByID(2212);
    auto pips=c12.getByID(211);
    auto pims=c12.getByID(-211);
       
    if(electrons.size()>0 &&pips.size()>0 &&pims.size()>0){
       
    
	c12writer.writeEvent(); 

      counter++;
    }
  }
  

  //close writer
  c12writer.closeWriter();

  gBenchmark->Stop("timer");
  gBenchmark->Print("timer");
  
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  std::cout << "Elapsed time: " << elapsed.count()<< " read events = "<<counter<<" wrote events = "<<writeCounter<<" s\n";

}
