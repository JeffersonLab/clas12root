//usage :
//clas12root  4 myFirstSelector.C Ex3_ProofLite.C
//Where 4  is however many workers you would like
//myFirstSelector.C is the source dode of the selector
//   this can be created with any name using makeHipoSelector executable
//Note you must also change myFirstSelector in the macro below to your
//selector name
{
  clas12root::HipoChain chain;
  chain.Add("/WHERE/IS/MY/HIPO/file.hipo");
  clas12root::myFirstSelector sel(&chain);

  //////////////////////////////////////
  //To creat rcdb data RCDB_HOME must be set prior to installation
  //chain.WriteRcdbData("rcdb.root"); //Must use this first time to create local copy
  //Then when we have local copy can just use the following
  //chain.SetRcdbFile("rcdb.root");

  gProof->Process(&sel,chain.GetNRecords());
}
