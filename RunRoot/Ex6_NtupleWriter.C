#include "clas12reader.h"
#include "ntuple_writer.h"
#include <TBenchmark.h>


void Ex6_NtupleWriter(){


  clas12::clas12reader c12("/where/is/my/hipo/file.hipo");

  hipo::ntuple_writer ntuple("ntuple1.hipo");

  //create bank
  ntuple.bank("ntuple::particle","P_e/F,P_p/F,P_pip/F,P_pim/F,pid/I",100);

  //get item reference from writer
  //(int, short, byte, long, float, double)
  auto& Pe=ntuple.floatItem("ntuple::particle","P_e");
  auto& Pp=ntuple.floatItem("ntuple::particle","P_p");
  auto& pid=ntuple.intItem("ntuple::particle","pid");
  
  //or give item reference to writer
  float Ppip=0;
  float Ppim=0;
  ntuple.linkItemFunc("ntuple::particle","P_pip",&Ppip);
  ntuple.linkItemFunc("ntuple::particle","P_pim",&Ppim);

  //optionally create more than one bank
  ntuple.bank("ntuple::theta","e/F,p/F,pip/F,pim/F",100);
  auto& The=ntuple.floatItem("ntuple::theta","e");
  auto& Thp=ntuple.floatItem("ntuple::theta","p");
  auto& Thpip=ntuple.floatItem("ntuple::theta","pip");
  auto& Thpim=ntuple.floatItem("ntuple::theta","pim");


   ntuple.open();
 
   gBenchmark->Start("banked");

   //normal loop with clas12reader
   while(c12.next()==true){
     
     // get particles by type
     auto electrons=c12.getByID(11);
     auto gammas=c12.getByID(22);
     auto protons=c12.getByID(2212);
     auto pips=c12.getByID(211);
     auto pims=c12.getByID(-211);
     
     if(electrons.size()>0  && protons.size()>0 &&
	pips.size()>0 &&pims.size() >0){

       //give the references their values for this event
       Pe=electrons[0]->getP();
       Pp=protons[0]->getP();
       Ppip=pips[0]->getP();
       Ppim=pims[0]->getP();
       
       The=electrons[0]->getTheta();
       Thp=protons[0]->getTheta();
       Thpip=pips[0]->getTheta();
       Thpim=pims[0]->getTheta();

       //fill ntuple for this event
       ntuple.fill();
     }
   }
   ntuple.close();
   gBenchmark->Stop("banked");
   gBenchmark->Print("banked");
}
