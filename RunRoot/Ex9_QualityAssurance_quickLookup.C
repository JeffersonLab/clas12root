#include <cstdlib>
#include <iostream>
#include "qadb_reader.h"

using namespace clas12;
using namespace std;

void Ex9_QualityAssurance_quickLookup(){

  /*Create a list of runs.
   6598, 6597 used as examples here.*/
  std::set<int> Runs;
  Runs.insert(6598);
  Runs.insert(6597);

  /*Set up qadb_reader, can pass any run number (here 0), this isn't used*/
  qadb_reader qa(0);

  /*Set up the requirements.*/
  qa.requireGolden(true);

  /*Ask for the accumulated charge in the list of runs.*/
  double charge=qa.getChargeForRunlist(Runs);
  cout<<"Accumulated Charge "<<charge<<" nA"<<endl;
}
