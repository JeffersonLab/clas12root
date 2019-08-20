#pragma once


#include "HipoROOTOut.h"
#include <TTree.h>
#include <TFile.h>

namespace clas12root {

  
  class HipoTreeMaker : public HipoROOTOut {


  public :
    HipoTreeMaker(TString filename, TString treefilename);
    virtual ~HipoTreeMaker();


 
    virtual HipoTreeMaker* Branch(TString varExp,TString alias="");
    //virtual HipoTreeMaker* EvBranch(TString varExp,TString alias="");


    TTree* Tree() const {return _tree;}

    void Fill();
    void AddParticleCut(TString cut);
    void AddEventCut(TString cut);
    
  protected :
    
    vector<TString> _branchNames;
    vector<TString> _branchTypes;
    /* vector<TString> _evbranchNames; */
    /* vector<TString> _evbranchTypes; */
    TString _evcut={"0"};
    TString _pcut={"1"};

  private :

    TFile* _treefile=nullptr;
    TTree* _tree=nullptr;
  };
}
