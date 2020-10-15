//usage :
//clas12proof  4 RunRoot/testSelector.C+ RunRoot/Ex3b_TestSelector.C
//Where 4  is however many workers you would like
//myFirstSelector.C is the source dode of the selector
//   this can be created with any name using makeHipoSelector executable
//Note you must also change myFirstSelector in the macro below to your
//selector name
{
  //clas12databases::SetCCDBLocalConnection("ccdb.sqlite");
  //clas12databases::SetQADBConnection("qaDB.json");
  //clas12databases::SetRCDBRootConnection("rcdb.root");

  clas12root::HipoChain chain;
  chain.Add("/where/are/my/files/f1.hipo");
  chain.Add("/where/are/my/files/f2.hipo");
  auto c12=chain.GetC12Reader();

  
  if(c12->qadb()!=nullptr){
    c12->db().qadb_requireOkForAsymmetry(true);
    c12->db().qadb_requireGolden(true);
    c12->db().qadb_addQARequirement("MarginalOutlier");
    c12->db().qadb_addQARequirement("TotalOutlier");
    /*
     * applyQA specifies to the clas12reader that quality assurance
     * cuts will be applied, based on the .json file given as an 
     * argument. This file should contain the Clas12 Quality Assurance
     * database.
     * i.e. clas12reader will only process events that pass quality assurance
     * and ignore those which fail
     */
    c12->applyQA();
  }

  
  //chain.Add("/WHERE/IS/MY/HIPO/FILE.hipo");
  clas12root::testSelector sel(&chain);
  gProof->Process(&sel,chain.GetNRecords());
}
