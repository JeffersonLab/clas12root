//setenv FMT .....(/usr/local?)
//setenv IGUANA ....
//setenv ROOT_INCLUDE_PATH ${FMT}/include:${ROOT_INCLUDE_PATH}

void LoadIguana(){

  //external iguana linking
  TString IGUANA = gSystem->Getenv("IGUANA");
  gInterpreter->AddIncludePath(IGUANA+"/include");

  gROOT->ProcessLine("#include <iguana/algorithms/Algorithm.h>");

  gSystem->Load("$IGUANA/lib/libIguanaServices.so");
  gSystem->Load("$IGUANA/lib/libIguanaAlgorithms.so");
}
