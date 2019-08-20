#include "BankHist.h"
#include "HipoRootAction.h"
#include "clas12reader.h"
#include <TObjArray.h>

namespace clas12root {

  BankHist::BankHist(TString filename): HipoHist(filename){
    _tempActionName="BankHistAction";

    _mapOfParts["BANK"]="bank.";
    _mapOfParts["REC::Particle"]="particle";
    _mapOfParts["REC::Calorimeter"]="calorimeter";
    _mapOfParts["REC::Scintillator"]="scintillator";
    _mapOfParts["REC::Cherenkov"]="cherenkov";
    _mapOfParts["REC::Track"]="tracker";
    _mapOfParts["REC::Traj"]="traj";
    _mapOfParts["REC::ForwardTagger"]="forwardtagger";
    _mapOfParts["MC::Lund"]="mcparticle";
    _mapOfParts["REC::CovMat"]="covmatrix";
    _mapOfParts["RAW::vtp"]="vtp";
    _mapOfParts["RAW::scaler"]="scaler";
    // _mapOfParts["REC::Event"]="header";
    _mapOfParts["REC::Event"]="event";
    _mapOfParts["RECFT::Event"]="ftbevent";
    _mapOfParts["RUN::config"]="runconfig";
    _mapOfParts["HEL::online"]="helonline";
    _mapOfParts["HEL::flip"]="helflip";
  

  }
  BankHist::~BankHist(){
  }
  void BankHist::Loop(){
    while(IsMoreFiles())
      gHipoRootAction->Action(this);

    _bankName=TString();
  }
  HipoHist* BankHist::Hist1D(TString varExp,Int_t nbins,Float_t min,Float_t max,TString condExp){

    CheckForBank(varExp);
    auto checkName1=_bankName;

    if(checkName1==TString())
      cout<<"WARNING BankHist::Hist1D" <<" no bank found in "<<varExp<<endl;
    CheckForBank(condExp);
    auto checkName2=_bankName;
    if(checkName1!=checkName2)
      cout<<"WARNING BankHist::Hist1D" <<"condition "<<checkName2<<" from different bank than var "<<checkName1<<endl;
    
    return HipoHist::Hist1D(varExp,nbins,min,max,condExp);
    
  }

  HipoHist* BankHist::Hist2D(TString varExp,Int_t nbinsX,Float_t minX,Float_t maxX,Int_t nbinsY,Float_t minY,Float_t maxY,TString condExp){

    CheckForBank(varExp);
    auto checkName1=_bankName;

    if(checkName1==TString())
      cout<<"WARNING BankHist::Hist2D" <<" no bank found in "<<varExp<<endl;
    CheckForBank(condExp);
    auto checkName2=_bankName;
    if(checkName1!=checkName2)
      cout<<"WARNING BankHist::Hist2D" <<"condition "<<checkName2<<" from different bank than var "<<checkName1<<endl;
    
    return HipoHist::Hist2D(varExp,nbinsX,minX,maxX,nbinsY,minY,maxY,condExp);
    
  }

  void BankHist::CheckForBank(TString& varExp){

    for(auto const& bank : _mapOfParts )
      if(varExp.Contains(bank.first)){
	_bankName=bank.first;
	varExp.ReplaceAll(_bankName+"::","BANK.");
	//	return varExp;
      }
    //return varExp;
  }
}
