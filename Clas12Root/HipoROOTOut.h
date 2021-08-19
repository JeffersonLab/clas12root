#pragma once


#include "region_particle.h"
#include <TString.h>
#include <TChain.h>
#include <map>
#include <vector>


namespace clas12root {

  using std::vector;
  using std::cout;
  using std::endl;
    
  class HipoROOTOut {

     
  public :
    HipoROOTOut(TString filename);
    virtual ~HipoROOTOut();
  
    void AddAction(TString varExp,TString condExp="1");
    void CompileAction();
    void CleanAction();
    void ReplaceMacroText(TString from,TString to);
    //  virtual TString ExpandVars(TString varExp0,TString seperator){return varExp0;};    
    virtual TString ExpandExpression(TString varExp0,TString seperator);    
    virtual void Loop()=0;
    virtual void PreCompileAction(){};
 
    //  virtual Double_t Action()=0;
    virtual Bool_t Condition(){return kTRUE;}

    TString HipoFileName(){return _hipoFileName;}
    TString NextFile();
    Bool_t IsMoreFiles();
    TChain* GetChain(){return &_chain;}


    TString ExpandVars(TString varExp0,TString seperator);
    TString ExpandPart(TString exp);
    TString ExpandParenthesis(TString varExp0,TString seperator);
    TString AddParenthesis(TString varExp0);
    vector<TString> RemoveArithmetic(TString& expr);

    void SetEntries(Long64_t n){_nEntriesToProcess=n;}
    void SetVerbose(short v=1){_verbose=v;}

    void CreateBankLink(TString label,TString code){
      _mapOfParts[label]=code;
    }
    
  protected :
    
    TString _tempActionName;
    TString _actionClassName;
    std::map<TString,TString> _mapOfParts;

    TString GetCurrMacroName(){return _curMacro;}
    Int_t GetNActions(){return _Nactions;}
   private :

    TString _hipoFileName;
    TString _curMacro;
    TString _tempDir="~/.clas12root";
    Int_t _Nactions=0;
    Int_t _Nruns=0;
    TChain _chain;
    Int_t _iHipoFile=0;
    Long64_t _nEntriesToProcess=-1;
    short _verbose=0;
  };//class HipoROOTOut
  
}
