//setenv FMT .....(/usr/local)
//setenv IGUANA ....
//setenv HIPO ....
//setenv ROOT_INCLUDE_PATH ${IGUANA}:${ROOT_INCLUDE_PATH}

void LoadIguana(){

  gROOT->ProcessLine("#include <iguana/algorithms/AlgorithmBoilerplate.h>");
  gROOT->ProcessLine("#include <iguana/algorithms/AlgorithmSequence.h>");
  gROOT->ProcessLine("#include <iguana/algorithms/Algorithm.h>");
 
  gSystem->Load("$HIPO/lib/libhipo4");
  gSystem->Load("$FMT/lib/libfmt");
  gSystem->Load("$IGUANA/lib/libIguanaServices.so");
  gSystem->Load("$IGUANA/lib/libIguanaAlgorithms.so");
  gSystem->Load("$IGUANA/lib/libIguanaValidators.so");

  
}
