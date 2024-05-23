//setenv FMT .....(/usr/local)
//setenv IGUANA ....
//setenv HIPO ....
//setenv ROOT_INCLUDE_PATH ${IGUANA}/include:${ROOT_INCLUDE_PATH}

void LoadIguana(){

  TString IGUANA = gSystem->Getenv("IGUANA");
  gInterpreter->AddIncludePath(IGUANA+"/include");
  
  gROOT->ProcessLine("#include <iguana/algorithms/AlgorithmBoilerplate.h>");
  gROOT->ProcessLine("#include <iguana/algorithms/AlgorithmSequence.h>");
  gROOT->ProcessLine("#include <iguana/algorithms/Algorithm.h>");
 
  gSystem->Load("$HIPO/lib/libhipo4");
  //  gSystem->Load("$FMT/lib/libfmt");
  gSystem->Load("$IGUANA/lib/libIguanaServices.so");
  gSystem->Load("$IGUANA/lib/libIguanaAlgorithms.so");
  gSystem->Load("$IGUANA/lib/libIguanaValidators.so");

  TString CLAS12ROOT = gSystem->Getenv("CLAS12ROOT");
  gInterpreter->AddIncludePath(Form("%s/iguana",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/IguanaAlgo.h\" ",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/Transformers.h\" ",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/Filters.h\" ",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/Creators.h\" ",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/Iguana.h\" ",CLAS12ROOT.Data()));

}
