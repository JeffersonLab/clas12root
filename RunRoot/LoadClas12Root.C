namespace clas12{};
namespace hipo{};
namespace clas12root{};

using namespace clas12;
using namespace hipo;
using namespace clas12root;

void LoadClas12Root(){
 
  TString CLAS12TOOL=gSystem->Getenv("CLAS12TOOL");
  TString LIB=CLAS12TOOL+"/lib/";
  gSystem->Load(LIB+"liblz4");
  gSystem->Load(LIB+"libHipo4");
  gSystem->Load(LIB+"libClas12Banks");
  gSystem->Load(LIB+"libClas12Root");

  gROOT->SetMacroPath(Form("%s:%s/RunRoot/",gROOT->GetMacroPath(),CLAS12TOOL.Data()));

}
