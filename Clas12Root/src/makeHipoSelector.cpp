#include <TRint.h>
#include <TString.h>

int main(int argc, char **argv) {

  TString selname=argv[1];
  
  TRint  *app = new TRint("App", &argc, argv);
  app->ProcessLine(Form("gSystem->Exec(\"cp $CLAS12ROOT/Clas12Root/tempHipoSelector.h %s.h\");",selname.Data()));
  app->ProcessLine(Form("gSystem->Exec(\"cp $CLAS12ROOT/Clas12Root/tempHipoSelector.cpp %s.C\");",selname.Data()));

  app->ProcessLine(Form("gSystem->Exec(\"sed -i'' -e 's/tempHipoSelector/%s/g' %s*\");",selname.Data(),selname.Data()));
  
  return 0;
}
