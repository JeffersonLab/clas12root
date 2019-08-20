#pragma once

#include "HipoROOTOut.h"
#include <memory>
#include <iostream>

namespace clas12root{
  
  class HipoRootAction {
  
  public :
    virtual ~HipoRootAction()=default;

    virtual void Action(clas12root::HipoROOTOut *hiprout) = 0;

    template<class T>  static void Create();

  private:
     
  };
  R__EXTERN std::unique_ptr<HipoRootAction>  gHipoRootAction;


  template<class T>  void HipoRootAction::Create()
    {
      gHipoRootAction.reset(new T());
    }


}
