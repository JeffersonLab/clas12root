#include <iostream>
#include <TRint.h>
#include <TString.h>


int main(int argc, char **argv) {

  auto Nworkers=argv[1];
  auto selectorMacro=argv[2];
  TRint  *app = new TRint("App", &argc, argv);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("clas12root [%d] ");
  app->ProcessLine(".x $CLAS12ROOT/RunRoot/LoadClas12Root.C");
  app->ProcessLine(Form(".x $CLAS12ROOT/RunRoot/LoadProofLib.C(%s)",Nworkers));
  app->ProcessLine(Form("gProof->Load(\"%s\");",selectorMacro));

  app->Run();

  app->Terminate(0);
  
  return 0;
  
}
