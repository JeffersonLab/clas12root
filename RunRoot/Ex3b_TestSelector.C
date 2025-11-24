//usage :
//clas12proof  4 RunRoot/testSelector.C+ RunRoot/Ex3b_TestSelector.C
//Where 4  is however many workers you would like
//myFirstSelector.C is the source dode of the selector
//   this can be created with any name using makeHipoSelector executable
//Note you must also change myFirstSelector in the macro below to your
//selector name
{
  //clas12databases::SetCCDBLocalConnection("ccdb.sqlite");
  //clas12databases::SetRCDBRootConnection("rcdb.root");

  clas12root::HipoChain chain;
  chain.Add("/where/are/my/files/f1.hipo");
  chain.Add("/where/are/my/files/f2.hipo");
  auto config_c12=chain.GetC12Reader();

  
  /*
   * Several quality assurance requirements can be specified.
   * requireOkForAsymmetry requires that an event was deemed
   * suitable for asymmetry measurements.
   *
   * addRequirement requires that the event was not identified
   * as, for example, a marginal outlier. Several requirements
   * can be assigned at the same time.
   * 
   * See RGA analysis note and clas12-qadb github repository for
   * additional information.
   *
   * applyQA specifies to the clas12reader that quality assurance
   * cuts will be applied, based on the cook pass given as an 
   * argument. This file should contain the Clas12 Quality Assurance
   * database.

   */
  config_c12->applyQA(GETPASSSTRINGHERE);//GETPASSSTRINGHERE="latest", "pass1, "pass2",...
  config_c12->db()->qadb_addQARequirement("MarginalOutlier");
  config_c12->db()->qadb_addQARequirement("TotalOutlier");
  config_c12->db()->qadb_addQARequirement("TerminalOutlier");
  config_c12->db()->qadb_addQARequirement("MarginalOutlier");
  config_c12->db()->qadb_addQARequirement("SectorLoss");
  config_c12->db()->qadb_addQARequirement("LowLiveTime");
 

  
  clas12root::testSelector sel(&chain);
  gProof->Process(&sel,chain.GetNRecords());
}
