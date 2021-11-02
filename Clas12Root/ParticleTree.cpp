#include "ParticleTree.h"
#include "HipoRootAction.h"
#include "clas12reader.h"
#include <TObjArray.h>
#include <TMacro.h>
#include <TObjString.h>

namespace clas12root {

  ParticleTree::ParticleTree(TString filename, TString treefilename): HipoTreeMaker{filename,treefilename}{
    _tempActionName="ParticleTreeAction";

    //Calorimeters
    _mapOfParts["ECIN"]="p->cal(ECIN)->";
    _mapOfParts["ECOUT"]="p->cal(ECOUT)->";
    _mapOfParts["PCAL"]="p->cal(PCAL)->";

    //Scintillators
    _mapOfParts["FTOF1A"]="p->sci(FTOF1A)->";
    _mapOfParts["FTOF1B"]="p->sci(FTOF1B)->";
    _mapOfParts["FTOF2"]="p->sci(FTOF2)->";
    _mapOfParts["CTOF"]="p->sci(CTOF)->";
    _mapOfParts["CND1"]="p->sci(CND1)->";
    _mapOfParts["CND2"]="p->sci(CND2)->";
    _mapOfParts["CND3"]="p->sci(CND3)->";

    //FT
    _mapOfParts["FTCAL"]="p->ft(FTCAL)->";
    _mapOfParts["FTHODO"]="p->ft(FTHODO)->";

    //Cherenkov
    _mapOfParts["HTCC"]="p->che(HTCC)->";
    _mapOfParts["LTCC"]="p->che(LTCC)->";

    //Trackers
    _mapOfParts["DC"]="p->trk(DC)->";
    _mapOfParts["CVT"]="p->trk(CVT)->";

    //Particles
    _mapOfParts["PBANK"]="p->par()->";

    //Particle Region
    _mapOfParts["P"]="p->";

    //EVENT
    _mapOfParts["EVNT"]="c12.event()->";
    _mapOfParts["EVNT4"]="c12.event()->";
    _mapOfParts["EVNTFT"]="c12.ftbevent()->";

    //RUN
    _mapOfParts["RUN"]="c12.runconfig()->";

     //HELICITY
    _mapOfParts["HELFLIP"]="c12.helflip()->";
    _mapOfParts["HELONLINE"]="c12.helonline()->";
  
    //C12
    _mapOfParts["C12"]="c12.";
    _mapOfParts["C12.NPid"]="c12.getNPid";
  
  }
  ParticleTree::~ParticleTree(){
    }
  void ParticleTree::Loop(){

     
    while(IsMoreFiles())
      gHipoRootAction->Action(this);
    
  }

  void  ParticleTree::PreCompileAction(){
    std::map<TString,TString> typelabel;
    typelabel["/D"]="Double_t";
    typelabel["/F"]="Float_t";
    typelabel["/I"]="Int_t";
    typelabel["/L"]="Long64_t";
    typelabel["/S"]="Short_t";
 
    TMacro macro(GetCurrMacroName());

    TList *lines=macro.GetListOfLines();

    //First add particle branches to clas12data class
    TObject* obj=macro.GetLineWith("class clas12data");
    Int_t place =lines->IndexOf(obj)+2; 
  
    for(UInt_t i=0;i<_branchNames.size();i++){
      //First add data members to the class
      TString type=typelabel[_branchTypes[i]];
     
      TString bname=_branchNames[i];
     
      bname.ReplaceAll(".","_");
      
      TString strline=TString("     ")+type + "  "+bname+";";
      lines->AddAt(new TObjString(strline.Data()),place++);

      //Now get the value for this datamember
      TString strvar=Form("VVVV%d|",i);
     
      TString vvvvline=macro.GetLineWith(strvar)->GetString();
      vvvvline.ReplaceAll(strvar,bname);
      
      macro.GetLineWith(strvar)->SetString(vvvvline);
    }
    /////////////////////////////////////////////////////
    //Now add event branches to eventdata class
    if(_useEventData){
      obj=macro.GetLineWith("USEEVENTDATA");
      if(obj)place =lines->IndexOf(obj)+1;
      lines->AddAt(new TObjString("	_eventdata=new event_data;"),place++);
      lines->AddAt(new TObjString("	tree->Branch(\"EventData\",&_eventdata);"),place++);

      obj=macro.GetLineWith("EVENTDATABANKS");
      if(obj)place =lines->IndexOf(obj)+1;
      lines->AddAt(new TObjString("      auto evbank=c12.event();"),place++);
      lines->AddAt(new TObjString("      auto runbank=c12.runconfig();"),place++);
      lines->AddAt(new TObjString("      auto helfbank=c12.helflip();"),place++);
      lines->AddAt(new TObjString("      auto helobank=c12.helonline();"),place++);

      obj=macro.GetLineWith("Fill Event Data");
      if(obj)place =lines->IndexOf(obj)+1;
      lines->AddAt(new TObjString("	_eventdata->BeamCharge=evbank->getBeamCharge();"),place++);
      lines->AddAt(new TObjString("	_eventdata->StartTime=evbank->getStartTime();"),place++);
      lines->AddAt(new TObjString("	_eventdata->RFTime=evbank->getRFTime();"),place++);
      lines->AddAt(new TObjString("	_eventdata->ProcTime=evbank->getProcTime();"),place++);
      lines->AddAt(new TObjString("	_eventdata->LiveTime=evbank->getLiveTime();"),place++);
      lines->AddAt(new TObjString("	_eventdata->FTBStartTime=evbank->getFTBStartTime();"),place++);
      lines->AddAt(new TObjString("	_eventdata->Trigger=runbank->getTrigger();"),place++);
      lines->AddAt(new TObjString("	_eventdata->Category=evbank->getCategory();"),place++);
      lines->AddAt(new TObjString("	_eventdata->Topology=evbank->getTopology();"),place++);
      lines->AddAt(new TObjString("	_eventdata->Helicity=evbank->getHelicity();"),place++);
      lines->AddAt(new TObjString("	_eventdata->HelicityRaw=evbank->getHelicityRaw();"),place++);
      lines->AddAt(new TObjString("	_eventdata->EventNumber=runbank->getEvent();"),place++);
      // lines->AddAt(new TObjString("	if(helobank.get())_eventdata->onlineHelicity=helobank->getCurrHelicity();"),place++);
      //lines->AddAt(new TObjString("	if(helfbank.get())_eventdata->flipHelicity=helfbank->getCurrHelicity();"),place++);
 
    }

    TString strvar{"if(PCCCC)_treedata"};
    TString ccccline=macro.GetLineWith(strvar)->GetString();
    ccccline.ReplaceAll("PCCCC",_pcut);
    macro.GetLineWith(strvar)->SetString(ccccline);

    //Apply and cuts/selections
    obj=macro.GetLineWith("add Pid selections");
    place =lines->IndexOf(obj)+1; 
    for(auto const& selectp : _pidSelect){
      TString strline =Form("      c12.addAtLeastPid(%d,%d);",selectp.first,selectp.second);
      lines->AddAt(new TObjString(strline.Data()),place++);
    }
    for(auto const& selectp : _pidSelectExact){
      TString strline =Form("      c12.addExactPid(%d,%d);",selectp.first,selectp.second);
      lines->AddAt(new TObjString(strline.Data()),place++);
    }
    if(_zeroOfRestPid){
      TString strline =Form("      c12.addZeroOfRestPid();");
      lines->AddAt(new TObjString(strline.Data()),place++);

    }
    strvar="if(ECCCC)";
    ccccline=macro.GetLineWith(strvar)->GetString();
    ccccline.ReplaceAll("ECCCC",_evcut);
    macro.GetLineWith(strvar)->SetString(ccccline);
    
    macro.SaveSource(GetCurrMacroName());
  }
}
