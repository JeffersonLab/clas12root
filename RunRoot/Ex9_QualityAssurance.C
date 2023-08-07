#include <cstdlib>
#include <iostream>
#include "clas12reader.h"
#include "jsonFileMerger.h"


using namespace clas12;
using namespace std;

void Ex9_QualityAssurance(){
  
  //clas12reader declared as usual.
  clas12reader c12("/path/to/data.hipo");

  /*c12reader needs a database*/
  clas12databases db;
  c12.connectDataBases(&db);

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
   */
  c12.db()->qadb_requireOkForAsymmetry(true);
  c12.db()->qadb_requireGolden(true);
  c12.db()->qadb_addQARequirement("MarginalOutlier");
  c12.db()->qadb_addQARequirement("TotalOutlier");

  /*
   * applyQA specifies to the clas12reader that quality assurance
   * cuts will be applied, based on the .json file given as an 
   * argument. This file should contain the Clas12 Quality Assurance
   * database.
   */
  c12.applyQA();

  //The analysis can then proceed as usual.
  while(c12.next()) {
    //Do rest of analysis...
  }

  /*
   * The clas12-qadb software also provides the accumulated charge for events
   * that pass the quality assurance requirements.
   */
  cout<<"Accumulated charge past QA: "<<c12.qadb()->getAccCharge()<<" nC"<<endl;

}
