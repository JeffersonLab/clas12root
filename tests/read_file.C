{
  //hipo file to be analysed
  //RGA pass 1, skim3, 5000 events
  HipoChain chain;
  chain.Add("skim3_005036_subset.hipo");

  //check file exists
  int nfiles = chain.GetNFiles();
  cout<<nfiles<<endl;
  if (nfiles==0){
    throw std::invalid_argument("No files found");
  }
}
