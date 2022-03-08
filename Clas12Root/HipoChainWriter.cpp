#include "HipoChainWriter.h"


namespace clas12root{

  HipoChainWriter::HipoChainWriter(const TString& output){

    if(output.EndsWith(".hipo")==true){
      //create 1 output file
      _writer.setFile(output.Data());
      Info("HipoChainWriter",Form("Writing to file %s ",output.Data()),"");
     _multiFile=kFALSE;
      }
    else{
      //create 1 output file for each input file
      _outdir=output+'/'; //Add user name directory
      _multiFile=kTRUE;
      gSystem->Exec(Form("mkdir -p %s",_outdir.Data()));
      Info("HipoChainWriter",Form("Writing to directory %s ",_outdir.Data()),"");
 
    }

  }
  Bool_t HipoChainWriter::NextFile(){
    //Open new reader file
    auto next=HipoChain::NextFile();
    if(next==kFALSE) return kFALSE;

    if(_multiFile==kFALSE){
      //assign a reader to the writer
      _writer.assignReader(*GetC12Reader());
    }
    else{

       TString outName(gSystem->BaseName(CurrentFileName()));
      //add an output directory, which already exists
      outName.Prepend(_outdir);
      //close previous writer, avoid the first one
      if(_writer.isOpen()==kTRUE) _writer.closeWriter();
      
      //create a new writer output file
      _writer.setFile(outName.Data());
      //assign new reader from chain to the  writer
      _writer.assignReader(*GetC12Reader());

    }
    return kTRUE;
  }
   /*

 
  auto electrons=c12->getByID(11);
  auto protons=c12->getByID(2212);

  //make some condition on writing events
  if(electrons.size()==1 && protons.size()==1)
    c12writer.writeEvent();
  */

  /*
 if(currc12!=c12.get()){
    currc12=c12.get();
    //get current input file name and remove directory
    TString outName(gSystem->BaseName(chain.GetFileName(fileNumber)));
    //add an output directory, which already exists
    outName.Prepend("/hdd/jlab/clas12data/writer/");
    //close previous writer, avoid the first one
    if(fileNumber!=0)c12writer->closeWriter();
    //create a new writer
    c12writer.reset(new clas12writer(outName.Data()));
    //assign a reader to the new writer
    c12writer->assignReader(*currc12);
    fileNumber++;
  }

   */


  
}
