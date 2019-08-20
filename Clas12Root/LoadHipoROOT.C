#include <TSystem.h>
#include <TString.h>
#include <TInterpreter.h>
#include <TROOT.h>

namespace clas12root{}
using namespace clas12root;

void LoadHipoROOT(){

 
  gROOT->LoadMacro("$CLAS12TOOL/RunRoot/importToROOT.C");

  TString HIPOROOT=TString(gSystem->Getenv("CLAS12TOOL"))+"/RunRoot/clas12root/";
  gSystem->AddIncludePath(TString("-I")+HIPOROOT);
  

  gROOT->LoadMacro(HIPOROOT+"HipoROOTOut.C+");
  gROOT->LoadMacro(HIPOROOT+"HipoRootAction.C+");
  gROOT->LoadMacro(HIPOROOT+"HipoHist.C+");
  gROOT->LoadMacro(HIPOROOT+"ParticleHist.C+");
  gROOT->LoadMacro(HIPOROOT+"BankHist.C+");
  gROOT->LoadMacro(HIPOROOT+"HipoTreeMaker.C+");
  gROOT->LoadMacro(HIPOROOT+"ParticleTree.C+");



}
