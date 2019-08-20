#include <iostream>
#include <TRint.h>
#include <TString.h>
#include "ParticleTree.h"


int main(int argc, char **argv) {

  TString infilename=argv[1];
  if(!infilename.EndsWith(".hipo")){
    std::cerr<<"Need a hipo file! usage \n particleDraw dir/file.hipo OptionalScript.C"<<std::endl;
    return 0;
  }
  TString outfilename=argv[2];
  if(!outfilename.EndsWith(".root")){
    std::cerr<<"Need a root file! usage \n particleDraw dir1/file.hipo dir2/outfile.root OptionalScript.C"<<std::endl;
    return 0;
  }

  TRint  *app = new TRint("App", 0, nullptr);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("ParticleTree [%d] ");

  app->ProcessLine(".x $CLAS12TOOL/RunRoot/LoadClas12Root4.C");
  app->ProcessLine(Form("clas12root::ParticleTree treemaker{\"%s\",\"%s\"}",infilename.Data(),outfilename.Data()));

  if(argc==4) {
    TString script=argv[3];
    std::cout<<script<<std::endl;
    if(script.EndsWith(".C"))
       app->ProcessLine(Form(".x %s",script.Data()));
    app->Run();
  }
  else  app->Run();

  app->Terminate(0);
  
  return 0;
  
}
