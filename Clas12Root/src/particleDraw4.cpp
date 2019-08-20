#include <iostream>
#include <TRint.h>
#include <TString.h>
#include "ParticleHist.h"


int main(int argc, char **argv) {

  TString filename=argv[1];
  if(!filename.EndsWith(".hipo")){
    std::cerr<<"Need a hipo file! usage \n particleDraw dir/file.hipo OptionalScript.C"<<std::endl;
    return 0;
  }
  TRint  *app = new TRint("App", 0, nullptr);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("ParticleHist [%d] ");

  app->ProcessLine(".x $CLAS12TOOL/RunRoot/LoadClas12Root4.C");
 // app->ProcessLine("gSystem->Load(\"$CLAS12TOOL/lib/liblz4\");");
 //  app->ProcessLine("gSystem->Load(\"$CLAS12TOOL/lib/libHipo4\");");
 //  app->ProcessLine("gSystem->Load(\"$CLAS12TOOL/lib/libClas12Banks4\");");
 //  app->ProcessLine("gSystem->Load(\"$CLAS12TOOL/lib/libClas12Root4\");");
  app->ProcessLine(Form("clas12root::ParticleHist hists{\"%s\"}",filename.Data()));
  if(argc==3) {
    TString script=argv[2];
    if(script.EndsWith(".C"))
       app->ProcessLine(Form(".x %s",script.Data()));
    app->Run();
  }
  else  app->Run();

  app->Terminate(0);
  
  return 0;
  
}
