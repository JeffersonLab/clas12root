#pragma once
#include "rcdb_vals.h"
#include <TNamed.h>

namespace clas12root {
  ///////////////////////////////RCDB
  class TRcdbVals : public TNamed{

  public :
    TRcdbVals()=default;
 
  TRcdbVals(clas12::rcdb_vals vals):_data{vals} {};
    const char* GetTitle() const final{return _data.file_name.data();}
    const char* GetName() const final{return Form("Run_%d", _data.run_number);}

    clas12::rcdb_vals _data{};

    ClassDef(clas12root::TRcdbVals,1);
  };
  //#pragma link C++ class clas12root::TRcdbVals+;
  //#pragma link C++ class clas12::rcdb_vals+;
}

///////////////////////////////   
