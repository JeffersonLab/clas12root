{
  //hipo file to be analysed
  //RGA pass 1, skim3, 5000 events
  HipoChain chain;
  chain.Add("./tests/data/EptopX2pi_sample.hipo");

  //check file exists
  int nfiles = chain.GetNFiles();
  cout<<nfiles<<endl;
  if (nfiles==0){
    throw std::invalid_argument("No files found");
  }
}
