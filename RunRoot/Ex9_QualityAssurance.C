#include <cstdlib>
#include <iostream>
#include "clas12reader.h"
#include "jsonFileMerger.h"


using namespace clas12;
using namespace std;

void Ex9_QualityAssurance(){


  /*
   * The jsonFileMerger class included in clas12root allows to
   * quickly merge several jason files. It takes as an
   * argument the absolute path for the merged output.
   *
   * addFile takes as argument the absolute path for an input 
   * .json file to be merged.
   *
   * mergeAllFiles merges all the added files and saves the output
   * to the specified location
   */
  jsonFileMerger merger("/absolute/path/for/output.json");
  merger.addFile("/absolute/path/for/input1.json");
  merger.addFile("/absolute/path/for/input2.json");
  merger.mergeAllFiles();
  
  //clas12reader declared as usual.
  clas12reader c12("/path/to/data.hipo");

  /*
   * applyQA specifies to the clas12reader that quality assurance
   * cuts will be applied, based on the .json file given as an 
   * argument. This file should contain the Clas12 Quality Assurance
   * database.
   */
  c12.applyQA("/absolute/path/to/qaDB.json");

  /*
   * Several quality assurance requirements can be specified.
   * requireOkForAsymmetry requires that an event was deemed
   * suitable for asymmetry measurements.
   *
   * addRequirement requires that the event was not identified
   * as, for example, a marginal outlier. Several requirements
   * can be assigned at the same time.
   * 
   * See RGA analysis note and clasqaDB github repository for
   * additional information.
   */
  c12.getQAReader()->requireOkForAsymmetry(true);
  c12.getQAReader()->requireGolden(true);
  c12.getQAReader()->addQARequirement("MarginalOutlier");
  c12.getQAReader()->addQARequirement("TotalOutlier");

  //The analysis can then proceed as usual.
  while(c12.next()) {
    //Do rest of analysis...
  }

}
