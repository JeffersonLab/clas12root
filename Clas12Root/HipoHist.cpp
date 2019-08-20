#include "HipoHist.h"
#include <TBenchmark.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TFile.h>

namespace clas12root {
  
  HipoHist::HipoHist(TString filename): HipoROOTOut(filename){
    _histList->SetName("HipoHists");
    _histList->SetOwner(kFALSE);
    
  }
  HipoHist::~HipoHist(){
 }
  HipoHist* HipoHist::Hist1D(TString varExp,Int_t nbins,Float_t min,Float_t max,TString condExp){

    auto hist=new TH1F(varExp+"_"+condExp,varExp +" for "+condExp,nbins,min,max);
    _histList->Add(hist);
    _curHist.push_back(hist);   
    
    varExp=ExpandExpression(varExp,",");
    if(condExp==TString(""))condExp="1";
    if(condExp!=TString("1")) condExp=ExpandExpression(condExp,"");
    AddAction(varExp,condExp);
     return this;
  }
  HipoHist* HipoHist::Hist2D(TString varExp,Int_t nbinsX,Float_t minX,Float_t maxX,Int_t nbinsY,Float_t minY,Float_t maxY,TString condExp){
    
    auto hist=new TH2F(varExp+"_"+condExp,varExp+" for "+condExp,nbinsX,minX,maxX,nbinsY,minY,maxY);
    _histList->Add(hist);
    _curHist.push_back(hist);   
    
    varExp=ExpandExpression(varExp,","); 
    if(condExp==TString(""))condExp="1";
    if(condExp!=TString("1")) condExp=ExpandExpression(condExp,""); 
    AddAction(varExp,condExp);
    return this;
  }
  
  void HipoHist::Draw(TString opt){
    gBenchmark->Reset();
    gBenchmark->Start("compile time");
    CompileAction();
    gBenchmark->Stop("compile time");
    gBenchmark->Print("compile time");
    gBenchmark->Start("loop time");
    Loop();
    gBenchmark->Stop("loop time");
    gBenchmark->Print("loop time");
    OnCanvas(opt);
    CleanAction();
    _curHist.clear();
  }
  
  void HipoHist::OnCanvas(TString opt){

    Int_t left=-1;
    Int_t Npads=0;
    Int_t nx=0;
    Int_t ny=0;
    if((left=opt.First("("))!=-1){
      left++;
      Int_t right=opt.First(")");
      if(right==-1) cout<<"HipoHist::OnCanvas mismatched () "<<opt<<endl;
      TString dims=opt(left,right-left);
      auto xandy=dims.Tokenize("x");
      if(xandy->GetEntries()<2)
	xandy=dims.Tokenize("X");
      if(xandy->GetEntries()>1){
	nx=TString(xandy->At(0)->GetName()).Atoi();
	ny=TString(xandy->At(1)->GetName()).Atoi();
	TCanvas* canvas=new TCanvas();
	canvas->Divide(nx,ny);
	canvas->Draw();
	Npads=nx*ny;
      }
      //remove split option
      left--;
      TString splitopt=opt(left,right-left+1);
      opt.ReplaceAll(splitopt,"");
    }
    //Simple 1 hist on 1 canvas
    TCanvas* canvas1=nullptr;
    if(_curHist.size()==1){
      if(!gPad) canvas1=new TCanvas();
      _curHist[0]->DrawCopy(opt);
      gPad->Draw();
      return;	
    }
    if(!Npads){
      //Simple many hists on many canvas
      for(auto* hist : _curHist){
	if(!opt.Contains("same")){
	  canvas1=new TCanvas();
	}
	hist->DrawCopy(opt);
	canvas1->Draw();
      }
    }
    else{
      //Use use divided canvas for many hists
      Int_t ipad=1;
      auto* canvas=dynamic_cast<TCanvas*>(gPad);
      Int_t Ncan=0;
      for(auto* hist : _curHist){
	if(ipad>Npads){
	  canvas=new TCanvas();
	  canvas->Divide(nx,ny);
	  canvas->Draw();
	  ipad=1;
	}
	canvas->cd(ipad++);
	hist->DrawCopy(opt);
      }
    }
  }
  void HipoHist::Save(const TString outname){
    auto outfile=TFile::Open(outname,"recreate");
    outfile->WriteTObject(_histList);
    delete outfile;
  }
  
}
