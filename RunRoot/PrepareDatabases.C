void PrepareDatabases(){


#ifdef CLAS_RCDB  
  /* For rcdb creaate a HipoChain of data files and the RCDB info will be
     downloaded and saved in a ROOT file for those runs*/
  clas12databases::SetRCDBRemoteConnection();
 
  clas12root::HipoChain chain;
  //Add you data files here
  chain.Add("/work/jlab/clas12data/skim14_005038.hipo");
  chain.WriteRcdbData("rcdb.root");

#endif


  
#ifdef CLAS_CCDB  
  /* For ccdb just download the most recent snapshot to read with sqlite
   */
  gSystem->Exec("wget https://clasweb.jlab.org/clas12offline/sqlite/ccdb/latest.sqlite");
  gSystem->Exec("mv latest.sqlite ccdb.sqlite");
#endif


  
  
  
  /*
   * The jsonFileMerger class included in clas12root allows to
   * quickly merge several jason files. It takes as an
   * argument the absolute path for the merged output.
   *
   * addFile takes as argument the absolute path for an input 
   * .json file to be merged.
   *
   * mergeAllFiles merges all the added files and saves the output
   * to the specified location
   */


/* Deprecated - clasqaDB software now premerges the database files
and automatically opens these.


#ifdef QADB 
  jsonFileMerger merger("qaDB.json");
  TString QAHOME=gSystem->Getenv("QADB");
  std::cout<<"CLASQA adding json file "<<(QAHOME+"/qa.inbending1/qaTree.json").Data()<<std::endl;
  merger.addFile((QAHOME+"/qa.inbending1/qaTree.json").Data());
  std::cout<<"CLASQA adding json file "<<(QAHOME+"/qa.inbending2/qaTree.json").Data()<<std::endl;
  merger.addFile((QAHOME+"/qa.inbending2/qaTree.json").Data());
  std::cout<<"CLASQA adding json file "<<(QAHOME+"/qa.outbending/qaTree.json").Data()<<std::endl;
  merger.addFile((QAHOME+"/qa.outbending/qaTree.json").Data());
  merger.mergeAllFiles();
  
#endif*/

  
}
