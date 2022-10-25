{


  clas12root::HipoChainWriter chain("/where/will/I/put/my/");
  // or clas12root::HipoChainWriter chain("/where/will/I/put/my/outfile.hipo");

  chain.Add("/where/is/my/file.hipo"); //Add input files

  //add options
  chain.GetWriter().writeSpecialBanks(true); //REC::Event etc.
  chain.GetWriter().skipBank("REC::Track"); //we will not save REC::Track bank
  chain.GetWriter().skipBank("REC::CovMat"); //we will not save REC::CovMat bank
  chain.db()->turnOffQADB();
  auto config_c12=chain.GetC12Reader();

  //now get reference to (unique)ptr for accessing data in loop
  //this will point to the correct place when file changes
  auto& c12=chain.C12ref();

  while(chain.Next()){
   
    auto electrons=c12->getByID(11);
    auto protons=c12->getByID(2212);

    //make some condition on writing events
    //Could construct mssing mass and cut on that
    if(electrons.size()==1 && protons.size()==1)
      chain.WriteEvent();

  }
  
  
}
