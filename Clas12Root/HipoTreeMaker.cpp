#include "HipoTreeMaker.h"
#include <TBenchmark.h>

namespace clas12root {
  
  HipoTreeMaker::HipoTreeMaker(TString filename, TString treefilename): HipoROOTOut(filename){
 
    _treefile=new TFile(treefilename,"recreate");
    _tree =new TTree("hipotree","tree converted from hipo data");
    _tree->SetDirectory(_treefile);
  }
  HipoTreeMaker::~HipoTreeMaker(){
 }
  void  HipoTreeMaker::AddParticleCut(TString cut){
    if(_pcut==TString("1"))
      _pcut=TString();
    _pcut+=ExpandExpression(cut,"");
  }
  void  HipoTreeMaker::AddEventCut(TString cut){
    if(_evcut==TString("0"))
      _evcut=TString();
    _evcut+=ExpandExpression(cut,"");
  }
  
  HipoTreeMaker* HipoTreeMaker::Branch(TString varExp,TString alias){
    TString branchType=varExp;
    branchType.Remove(0,branchType.Sizeof()-3); //keep /F etc
    varExp.Remove(varExp.Sizeof()-3,2); //remove /F etc

    if(alias!=TString()) _branchNames.push_back(alias);
    else _branchNames.push_back(varExp);
    _branchTypes.push_back(branchType);
    
    varExp=ExpandExpression(varExp,"");
    AddAction(varExp);
  
   return this;
  }
  // HipoTreeMaker* HipoTreeMaker::EvBranch(TString varExp,TString alias){
  //   TString branchType=varExp;
  //   branchType.Remove(0,branchType.Sizeof()-3); //keep /F etc
  //   varExp.Remove(varExp.Sizeof()-3,2); //remove /F etc

  //   if(alias!=TString()) _evbranchNames.push_back(alias);
  //   else _evbranchNames.push_back(varExp);
  //   _evbranchTypes.push_back(branchType);
    
  //   varExp=ExpandExpression(varExp,"");
  //   AddAction(varExp);
  
  //  return this;
  // }
  void HipoTreeMaker::Fill(){
    gBenchmark->Reset();
    gBenchmark->Start("compile time");
    CompileAction();
    gBenchmark->Stop("compile time");
    gBenchmark->Print("compile time");
    gBenchmark->Start("loop time");
    Loop();
    gBenchmark->Stop("loop time");
    gBenchmark->Print("loop time");
    CleanAction();
    _tree->Write();
    delete _treefile;

   }
  
  
}
