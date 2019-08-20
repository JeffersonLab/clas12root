#include "HipoROOTOut.h"
#include "HipoRootAction.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TBenchmark.h>
#include <TMacro.h>
#include <TObjString.h>
#include <iostream>

namespace clas12root{
  
  HipoROOTOut::HipoROOTOut(TString filename): _hipoFileName(filename){
    //make a temp file directory
    TString HOME=TString(gSystem->Getenv("HOME"));
    _tempDir=HOME+"/.clas12root/";
    if(gSystem->Exec(Form("mkdir -p %s",_tempDir.Data()))){
      std::cout<<"HipoROOTOut::HipoROOTOut will store temp files in current directory"<<std::endl;
      _tempDir="";
    }
 
  }
  HipoROOTOut::~HipoROOTOut(){
    gSystem->Exec(Form("rm %sasdf*",_tempDir.Data()));
   }
  TString HipoROOTOut::NextFile(){
    if(_iHipoFile==0&&_chain.GetListOfFiles()->GetEntries()==0)
      _chain.Add(_hipoFileName);
    if(_iHipoFile>=_chain.GetListOfFiles()->GetEntries())
      return TString();
    TString fname=_chain.GetListOfFiles()->At(_iHipoFile)->GetTitle();
    _iHipoFile++;
    return fname;
  }
  Bool_t HipoROOTOut::IsMoreFiles(){
    if(_iHipoFile==0&&_chain.GetListOfFiles()->GetEntries()==0)
      _chain.Add(_hipoFileName);
    if(_iHipoFile>=_chain.GetListOfFiles()->GetEntries())
      return kFALSE;
    return kTRUE;
  }
  void HipoROOTOut::AddAction(TString varExp,TString condExp){
    if(_curMacro==TString("")){
      TString HIPOROOT=TString(gSystem->Getenv("CLAS12TOOL"))+"/Clas12Root/";
      TMacro macro(HIPOROOT+"template"+_tempActionName+".C");
      _curMacro=_tempDir+Form("asdf%d__%s__.C",_Nruns,_tempActionName.Data());
      Int_t NtempFiles=1;
      TString tempMacro=_curMacro;
      while(gSystem->FindFile(_tempDir,tempMacro)){
	cout<<_curMacro<<endl;
	_curMacro=_tempDir+Form("asdf%d__%s__.C",_Nruns+NtempFiles++*1000,_tempActionName.Data());
	tempMacro=_curMacro;
      }
      cout<<"CURR MACRO "<<_curMacro<<endl;

	
      TString strline=macro.GetLineWith("NNNN")->GetString();
      _actionClassName=_tempActionName+Form("%d",_Nruns);
      strline.ReplaceAll("NNNNN",_actionClassName);
      macro.GetLineWith("NNNNN")->SetString(strline);

      _Nruns++;
      macro.SaveSource(_curMacro);
    }

    TMacro macro(_curMacro);
    TString strline=macro.GetLineWith("XXXX")->GetString();
    strline.ReplaceAll("////","");
    strline.ReplaceAll("XXXX",varExp);
    strline.ReplaceAll("IIII",Form("%d",_Nactions++));
    strline.ReplaceAll("CCCC",condExp);
    
    TList *lines=macro.GetListOfLines();
    TObject* obj=macro.GetLineWith("XXXX");
    lines->AddAfter(obj,new TObjString(strline.Data()));

 
    macro.SaveSource(_curMacro);
 
  }
  void HipoROOTOut::ReplaceMacroText(TString from,TString to){
    TMacro macro(_curMacro);
    TString strline=macro.GetLineWith(from)->GetString();
    strline.ReplaceAll(from,to);
    macro.GetLineWith(from)->SetString(strline);
    macro.SaveSource(_curMacro);
  }
  
  void HipoROOTOut::CompileAction(){
    PreCompileAction();
    TMacro macro(_curMacro);
    macro.Print();
    
    auto result=gROOT->LoadMacro(Form("%s++",_curMacro.Data()));
  
    gROOT->ProcessLine(Form("clas12root::%s::Create<clas12root::%s>();",_actionClassName.Data(),_actionClassName.Data()));
  }
  
   void HipoROOTOut::CleanAction(){
     _iHipoFile=0;
     if(_curMacro!=TString("")){
       _curMacro="";
       _Nactions=0;
     }
   }


  ////////////////////////////////////////////////////////////////
  ///String parsing
 TString HipoROOTOut::ExpandExpression(TString varExp0,TString seperator){
    varExp0.ReplaceAll(" ","");
    varExp0.ReplaceAll("::","@@");
    varExp0.ReplaceAll("()","{}");
    varExp0=AddParenthesis(varExp0);
    
    auto exps = varExp0.Tokenize(":");
    auto Nexp = exps->GetEntries();
    TString varExp1;
    for(Int_t i=0;i<Nexp;i++){
      if(i>0) varExp1+=seperator;
      TString exp = exps->At(i)->GetName();
      varExp1+=ExpandParenthesis(exp,seperator);
    }
    delete exps;
    varExp1.ReplaceAll("@@","::");
    varExp1.ReplaceAll("{}","()");
 
    return varExp1;
  }
  
  TString HipoROOTOut::ExpandVars(TString varExp0,TString seperator){
  
    // cout<<"TString HipoROOTOut::ExpandVars "<<varExp0<<endl;
  
      
    TString varExp1;
    auto exp=varExp0;
    
    auto symbols=RemoveArithmetic(exp);
    auto plusses = exp.Tokenize("#");//+-/* replaced with |
    Int_t Npl=plusses->GetEntries();
    if(Npl){
      Int_t ns=0;
      if(exp[0]=='#') varExp1+=symbols[ns++];//e.g. negative
      
      for(Int_t ipl=0;ipl<plusses->GetEntries();ipl++){
	if(ipl>0)varExp1+=symbols[ns++];
	varExp1+=ExpandPart(plusses->At(ipl)->GetName());
      }
    }
 
    //  else varExp1+=ExpandPart(exp);
    delete plusses;
    
    return varExp1;
  }
  ///////////////////NEED TO FIX FOR FTOF1B.Path<700 conditions
  TString HipoROOTOut::ExpandPart(TString exp){
    // cout<<" :ExpandPart "<<exp<<endl;
    if(exp.IsFloat()) return exp;
    exp.ReplaceAll(" ","");
    if(exp.Sizeof()==3&&exp[0]=='F'&&exp[1]=='T') return exp;
    if(exp.Sizeof()==3&&exp[0]=='F'&&exp[1]=='D') return exp;
    if(exp.Sizeof()==3&&exp[0]=='C'&&exp[1]=='D') return exp;
    
    auto parts = exp.Tokenize(".");
    if(parts->GetEntries()!=2) return exp;
    TString part= parts->At(0)->GetName();
    exp=Form("%sget%s()",_mapOfParts[part].Data(),parts->At(1)->GetName());
    delete parts;
    return exp;
  }

  vector<TString> HipoROOTOut::RemoveArithmetic(TString& expr){
    vector<TString> symbols;
    
    const vector<char> operators1={'+','-','/','*','>','<','!'};
    const vector<TString> operators2={"==","!=",">=","<=","&&","||"};
    for(Int_t i=0;i<expr.Sizeof();i++){
      Bool_t got2=kFALSE;
      for(auto& op: operators2){
	if(expr(i,2)==op){
	  expr(i,2)="# ";
	  symbols.push_back(op);
	  i++;
	  got2=kTRUE;
	  break;
	}
      }
      if(got2) continue;
      for(auto& op: operators1)
	if(expr[i]==op){
	  expr[i]='#';
	  symbols.push_back(op);
	  break;
	}
    }
    return symbols;
  }
  TString HipoROOTOut::ExpandParenthesis(TString varExp0,TString seperator){
    // cout<<"HipoROOTOut::ExpandParenthesis "<<varExp0<<endl;
    Int_t first = varExp0.First('(');
    TString varExp1;
    
    if(first==-1){
      varExp1=varExp0; //no brackets
      varExp1=ExpandVars(varExp1,seperator);
      return varExp1;
   }
   else {  //expand parenthesis
      Int_t nb=0;
      Int_t nc=0;
      Int_t second=0;
      first=-1;
      for(Int_t i=0;i<varExp0.Sizeof();i++){

	if(varExp0[i]=='('){
	  nb++;
	  if(first==-1){ first=i;
	    varExp1+=varExp0[i];
	  }
	}
	else if(varExp0[i]==')') nb--;

	else if(varExp0[i]==','){ //function commas
	  if(first==-1){
	    first=i;
	    nc++;
	  }
	  else{
	    second=i;
	    varExp1+=Form("%s",ExpandParenthesis(varExp0(first+1,second-first-1),seperator).Data());
	    first=i;
	    second=0;
	    varExp1+=',';
	    nc--;
	  
	  }
	}
	else if(first==-1) varExp1+=varExp0[i];//just copy text if betweeh parenthesis
	if(nb==0&&first!=-1){
	  second=i;
	  if(nc==0) varExp1+=Form("%s)",ExpandParenthesis(varExp0(first+1,second-first-1),seperator).Data());
	  else  varExp1+=Form("%s)",ExpandParenthesis(varExp0(first+1,second-first-1),seperator).Data());
	  first=-1;
	  second=0;
	}
      }
    }
    varExp1=Form("%s",varExp1.Data());
     
    //   cout<< "EXPANDED EXPRESSION PARENTHESIS"<<varExp1.Data()<<" "<<varExp1.Sizeof()<<endl;
    return varExp1;
  }

  TString HipoROOTOut::AddParenthesis(TString varExp0){ //surround XXX.YYY with ( )
    if(!varExp0.Contains('.')) return varExp0;
    TString varExp1=varExp0;
    const Int_t Nc=varExp0.Sizeof();
    TString alpha;
    for (Int_t i=0;i<Nc-1;i++){
       if(varExp0[i]=='.'){
	if(i==0) continue;
	Int_t iright=0;
	Int_t ileft=0;
	Int_t ia=i+1;
	alpha=varExp0[ia];
	while(alpha.IsAlnum()&&ia<Nc){
	  ia++;
	  alpha=varExp0[ia];
	}
	if(ia!=i+1){
	  iright=ia;
	  if(varExp0[iright]==')') //check if already got )
	    iright=0;
	}
	ia=i-1;
	alpha=varExp0[ia];
	while(alpha.IsAlnum()&&ia>0){
	  ia--;
	  alpha=varExp0[ia];
	}
	if(ia!=i-1){
	  // if(ia==i-2)ileft=ia+1;
ileft=ia+1;	  // else ileft=ia;
	  if(ileft-1>=0)
	    if(varExp0[ileft-1]=='(') //check if already got )
	      iright=0;
	}
	  
	if(iright!=i+1&&iright!=0){
	  if(ileft==1) ileft=0;
	  TString expr=varExp0(ileft,iright-ileft);
	  varExp1.ReplaceAll(expr.Data(),Form("(%s)",expr.Data()));
	  i++;
	}
      }
    }
    return varExp1;
      
  }

  
}
