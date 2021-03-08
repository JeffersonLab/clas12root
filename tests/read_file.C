{
  //hipo file to be analysed
  //Simulated electron + proton -> proton + X + 2pi, 500 events
  HipoChain chain;
  chain.Add("./tests/data/EptopX2pi_sample.hipo");

  //check file exists
  int nfiles = chain.GetNFiles();
  cout<<nfiles<<endl;
  if (nfiles==0){
    throw std::invalid_argument("No files found");
  }
}
