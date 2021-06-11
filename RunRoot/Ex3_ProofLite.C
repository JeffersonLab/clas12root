//usage :
//clas12root  4 myFirstSelector.C+ Ex3_ProofLite.C
//Where 4  is however many workers you would like
//myFirstSelector.C is the source dode of the selector
//   this can be created with any name using makeHipoSelector executable
//Note you must also change myFirstSelector in the macro below to your
//selector name
{
  clas12root::HipoChain chain;
  chain.Add("/WHERE/IS/MY/HIPO/file.hipo");
  auto config_c12=chain.GetC12Reader();

  // //if require qadb stuff
  // if(config_c12->qadb()!=nullptr){
  //   config_c12->db()->qadb_requireOkForAsymmetry(true);
  //   config_c12->db()->qadb_requireGolden(true);
  //   config_c12->db()->qadb_addQARequirement("MarginalOutlier");
  //   config_c12->db()->qadb_addQARequirement("TotalOutlier");
  //   /*
  //    * applyQA specifies to the clas12reader that quality assurance
  //    * cuts will be applied, based on the .json file given as an 
  //    * argument. This file should contain the Clas12 Quality Assurance
  //    * database.
  //    * i.e. clas12reader will only process events that pass quality assurance
  //    * and ignore those which fail
  //    */
  //   config_c12->applyQA();
  // }

  //   c12->applyQA();
  // }



   clas12root::myFirstSelector sel(&chain);
   gProof->Process(&sel,chain.GetNRecords());
}
