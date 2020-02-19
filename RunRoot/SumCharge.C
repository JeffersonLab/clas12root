{
  clas12root::HipoChain chain;
  chain.Add("/some/hipo/files/*.hipo");
  auto nFiles=chain.GetNFiles();
   
  gBenchmark->Start("charge");

  double totCharge{0};
  
  for(int ifile=0;ifile<nFiles;ifile++){
    clas12reader c12(chain.GetFileName(ifile).Data(),{0});//tag 0 events  
  
    while(c12.next()==true){
     totCharge+=c12.event()->getBeamCharge();
    }


  }
  cout<<"ACCUMULATED BEAM CHARGE "<<totCharge<<endl;
  gBenchmark->Stop("charge");
  gBenchmark->Print("charge");
}
