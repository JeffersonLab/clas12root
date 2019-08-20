#pragma once


#include "HipoROOTOut.h"
#include <TH1.h>
#include <TH2.h>
#include <TList.h>

namespace clas12root {

  
  class HipoHist : public HipoROOTOut {


  public :
    HipoHist(TString filename);
    virtual ~HipoHist();


 
    virtual HipoHist* Hist1D(TString varExp,Int_t nbins,Float_t min,Float_t max,TString condExp="1");
    virtual HipoHist* Hist2D(TString varExp,Int_t nbinsX,Float_t minX,Float_t maxX,
		     Int_t nbinsY,Float_t minY,Float_t maxY,TString condExp="1");
   
    
    void Draw(TString opt="");
    void OnCanvas(TString opt);
    void Save(const TString outname);
    std::vector<TH1* >* Hists(){return &_curHist;}
  protected :
    
    std::vector<TH1* > _curHist;

  private :
 
    TList* _histList=new TList{};
    
  };
}
