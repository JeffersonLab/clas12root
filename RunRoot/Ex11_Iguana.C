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

#include <iguana/algorithms/clas12/ZVertexFilter/Algorithm.h>
#include <iguana/algorithms/clas12/SectorFinder/Algorithm.h>
#include <iguana/algorithms/clas12/MomentumCorrection/Algorithm.h>
#include <iguana/algorithms/physics/InclusiveKinematics/Algorithm.h>

using namespace clas12;


void SetLorentzVector(TLorentzVector &p4,clas12::region_part_ptr rp){
  p4.SetXYZM(rp->par()->getPx(),rp->par()->getPy(),
	      rp->par()->getPz(),p4.M());

}

void Ex11_Iguana(){
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


   //////////////////////////////////////////////////////////////////////////////////
   // start Iguana algorithms

   // create the algorithms
   iguana::clas12::ZVertexFilter        algo_z_vertex_filter;      // filter the z-vertex (a filter algorithm)
   iguana::clas12::SectorFinder         algo_sector_finder;        // get the sector for each particle (a creator algorithm)
   iguana::clas12::MomentumCorrection   algo_momentum_correction;  // momentum corrections (a transformer algorithm)
   iguana::physics::InclusiveKinematics algo_inclusive_kinematics; // calculate inclusive kinematics (a creator algorithm)

   /* configure iguana algorithms here (see iguana documentation) */

   // start the algorithms
   algo_z_vertex_filter.Start();
   algo_sector_finder.Start();
   algo_momentum_correction.Start();
   algo_inclusive_kinematics.Start();

   // create bank objects, which creator-type algorithms will populate
   auto created_bank_sector = algo_sector_finder.GetCreatedBank();
   auto created_bank_inclusive_kinematics = algo_inclusive_kinematics.GetCreatedBank();

   // define a lambda function that processes HIPO banks, in particular, with iguana
   // - this function will be executed by `clas12reader` as soon as each event's `hipo::bank`
   //   objects are filled, i.e., before `clas12reader` further processes the banks
   // - this example macro owns algorithm and created bank objects, therefore the lambda
   //   function must capture them by reference
   // - the parameter `cr` is a pointer, which is your `clas12reader` instance, whether you are
   //   using `clas12reader` directly (see `RunRoot/Ex1_CLAS12Reader.C`) or
   //   using `HipoChain` (see `RunRoot/Ex1_CLAS12ReaderChain.C`)
   // - use `cr` to access DST banks from the `clas12reader`, use `cr->getBankName()` methods,
   //   where "BankName" is the name of the bank without the colons (`::`); e.g., use
   //   `cr->getRECFTParticle` to access the `RECFT::Particle` bank
   auto iguana_action = [
     // captured algorithms
     &algo_z_vertex_filter,
     &algo_sector_finder,
     &algo_momentum_correction,
     &algo_inclusive_kinematics,
     // captured banks
     &created_bank_sector,
     &created_bank_inclusive_kinematics
   ](clas12reader* cr)
   {
     // call Iguana run functions, in your prefered order
     algo_z_vertex_filter.Run(
         cr->getRECParticle(),
         cr->getRUNconfig()
         );
     algo_sector_finder.Run(
         cr->getRECParticle(),
         cr->getRECTrack(),
         cr->getRECCalorimeter(),
         cr->getRECScintillator(),
         created_bank_sector
         );
     algo_momentum_correction.Run(
         cr->getRECParticle(),
         created_bank_sector,
         cr->getRUNconfig()
         );
     algo_inclusive_kinematics.Run(
         cr->getRECParticle(),
         cr->getRUNconfig(),
         created_bank_inclusive_kinematics
         );
   };
   //////////////////////////////////////////////////////////////////////////////////


   for(Int_t i=0;i<files->GetEntries();i++){
     //create the event reader
     clas12reader c12(files->At(i)->GetTitle(),{0});

     // attach the iguana-running lambda function to the event reader, so
     // `c12.next()` will call `iguana_action(c12)` internally
     c12.SetReadAction(iguana_action);

      while(c12.next()==true){

        // print the created inclusive kinematics bank
        created_bank_inclusive_kinematics.show();

 	for(auto& p : c12.getDetParticles()){
  	 //  get predefined selected information
	 p->getTime();
	 p->getDetEnergy();
	 p->getDeltaEnergy();

	 //check trigger bits
	 //	 if(c12.checkTriggerBit(25)) cout<<"MesonExTrigger"<<endl;
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
	   p->che(HTCC)->getNphe();
	   p->che(LTCC)->getNphe();
	   //trajectories
	   p->traj(LTCC)->getX();
	   // p->traj(DC,DC1)->getCx();; //First layer of DC, hipo4
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
	 p->cmat();
       }

       // get particles by type
       auto electrons=c12.getByID(11);
       auto gammas=c12.getByID(22);
       auto protons=c12.getByID(2212);
       auto pips=c12.getByID(211);
       auto pims=c12.getByID(-211);
       
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
       }
    
       
       counter++;
     }

   }
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
  
   auto finish = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = finish - start;
   std::cout << "Elapsed time: " << elapsed.count()<< " events = "<<counter<< " s\n";

}
