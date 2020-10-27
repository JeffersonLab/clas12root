#include <iostream>
#include <TRint.h>
#include <TString.h>
#include <TSystem.h>

int main(int argc, char **argv) {

  TRint  *app = new TRint("App", &argc, argv);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("clas12root [%d] ");

  app->ProcessLine(".x $CLAS12ROOT/RunRoot/LoadClas12Root.C");


  app->Run();

  app->Terminate(0);
  
  return 0;
  
}
