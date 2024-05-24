//setenv FMT .....(/usr/local?)
//setenv IGUANA ....
//setenv ROOT_INCLUDE_PATH ${FMT}/include:${ROOT_INCLUDE_PATH}

void LoadIguana(){

  //external iguana linking
  TString IGUANA = gSystem->Getenv("IGUANA");
  gInterpreter->AddIncludePath(IGUANA+"/include");
  
  gROOT->ProcessLine("#include <iguana/algorithms/AlgorithmBoilerplate.h>");
  gROOT->ProcessLine("#include <iguana/algorithms/AlgorithmSequence.h>");
  gROOT->ProcessLine("#include <iguana/algorithms/Algorithm.h>");
 
  gSystem->Load("$IGUANA/lib/libIguanaServices.so");
  gSystem->Load("$IGUANA/lib/libIguanaAlgorithms.so");
  gSystem->Load("$IGUANA/lib/libIguanaValidators.so");

  //clas12root iguana stuff
  TString CLAS12ROOT = gSystem->Getenv("CLAS12ROOT");
  gInterpreter->AddIncludePath(Form("%s/iguana",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/IguanaAlgo.h\" ",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/Transformers.h\" ",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/Filters.h\" ",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/Creators.h\" ",CLAS12ROOT.Data()));
  gROOT->ProcessLine(Form("#include \"%s/iguana/Iguana.h\" ",CLAS12ROOT.Data()));

}
