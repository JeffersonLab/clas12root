#pragma once


#include "HipoHist.h"

namespace clas12root {

  
  class BankHist : public HipoHist {


  public :
    BankHist(TString filename);
    virtual ~BankHist();


    void Loop() final;
    void PreCompileAction() final { ReplaceMacroText("BBBB", _mapOfParts[_bankName]);}
    HipoHist* Hist1D(TString varExp,Int_t nbins,
		     Float_t min,Float_t max,TString condExp) final;
    HipoHist* Hist2D(TString varExp,Int_t nbinsX,Float_t minX,Float_t maxX,
		     Int_t nbinsY,Float_t minY,Float_t maxY,
		     TString condExp) final;
    void CheckForBank(TString& varExp);

    TString GetBankName(){return _bankName;}
    
    /* using c12_uptr = std::unique_ptr<clas12::clas12reader>; */
    
    /* c12_uptr clas12(){ */
    /*   return std::move(c12_uptr */
    /* 		       (new clas12::clas12reader(HipoFileName().Data()))); */
    /* } */
    
  private :

      TString _bankName;
 
  };
}
