#include "ntuple_reader.h"
#include <TH1.h>
#include <TBenchmark.h>


void Ex6b_NtupleReader(){

  //creare ntuple reader from hipo file
  hipo::ntuple_reader ntuple("ntuple1.hipo");

  //get reference for ntuple::particle::P_e
  auto& Pe = ntuple.getFloat("ntuple::particle","P_e");

  //create histogram for filling
  TH1F hist("Ee","Ee",100,0,10);

  //loop over ntuple reader and fill histogram
  while(ntuple.next()){
    hist.Fill(Pe);
  }
 
  hist.DrawClone();
 }
