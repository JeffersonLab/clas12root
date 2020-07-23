/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   region_ft.h
 * Author: dglazier
 *
 */

#ifndef REGION_BAND_H
#define REGION_BAND_H

#include "clas12defs.h"
#include "region_particle.h"
#include "forwardtagger.h"


namespace clas12 {

 
  class region_band  : public region_particle {


  public:

     region_band(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm,
	       cal_ptr calp, scint_ptr scp,trck_ptr trp,
	       traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event);
    ~region_band()=default;

    
    bool sort() override;
    
    scint_ptr sci(ushort lay) const final;
    traj_ptr traj(ushort det,ushort layer=0) const final;
 

    double getTime() final{
      _scint->setIndex(_ptof);
      return _scint->getTime();
    }
    double getPath() final{
      _scint->setIndex(_ptof);
      return _scint->getPath();
    }

    double getDetEnergy() final{
      _scint->setIndex(_ptof);
      return _scint->getEnergy();
    }
 
    double getDeltaEnergy() final{
      _scint->setIndex(_pveto);
      return _scint->getEnergy();
    }
 
    short getSector() final{return 0;} 
    
    private:

    //   trck_ptr _track;//FT tracker ?

    short _ptof=-1;
    short _ptof1=-1;
    short _ptof2=-1;
    short _ptof3=-1;
    short _ptof4=-1;
    short _ptof5=-1;
    short _pveto=-1;
  };
  using region_band_ptr=clas12::region_band*;
  using region_band_uptr=std::unique_ptr<clas12::region_band>;

}

#endif /* REGION_BAND_H */
