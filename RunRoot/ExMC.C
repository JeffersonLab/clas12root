#include <TLorentzVector.h>
#include "clas12reader.h"
#include "HipoChain.h"

void ExMC(){

   clas12root::HipoChain chain;
   chain.Add("/my/gemc/file.hipo");
   auto c12=chain.GetC12Reader(); //this line is only in for backward compatability with older versions of clas12root

   TLorentzVector p4;
   
   while (chain.Next()){
     auto c12=chain.GetC12Reader();

     auto mcpbank=c12->mcparts(); 
     const Int_t  Ngen=mcpbank->getRows();
     
     for(Int_t i=0;i<Ngen;i++){
       mcpbank->setEntry(i);
       
       auto px=mcpbank->getPx();
       auto py=mcpbank->getPy();
       auto pz=mcpbank->getPz();
       auto pm=mcpbank->getMass();
       p4.SetXYZM(px,py,pz,pm);

       auto pid = mcpbank->getPid();

       cout<<" particle "<<i<<" "<<pid<<" p4 = "<<p4.X()<<","<<p4.Y()<<","<<p4.Z()<<","<<p4.T()<<" and mass "<<p4.M()<<endl;
       
     }

   }
}
