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

#ifndef REGION_FT_H
#define REGION_FT_H

#include "clas12defs.h"
#include "region_particle.h"


namespace clas12 {

 
  class region_ft  : public region_particle {


  public:

     region_ft(par_ptr pars,ftbpar_ptr ftbpars,covmat_ptr cm,
	       cal_ptr calp, scint_ptr scp,trck_ptr trp,utrck_ptr utrp,
	       traj_ptr trj, cher_ptr chp, ft_ptr ftp,event_ptr event,
	       rich_ptr rich,mcpar_ptr mcp=nullptr);
    ~region_ft() override=default;

    
    bool sort() override;
    
    ft_ptr ft(ushort lay) const final;
    traj_ptr traj(ushort det,ushort layer=0) const final;
 

    double getTime() const final{
      _ft->setIndex(_pcal);
      return _ft->getTime();
    }
    double getPath() const final;

    double getDetEnergy() const final{
      _ft->setIndex(_pcal);
      return _ft->getEnergy();
    }
 
    double getDeltaEnergy() const final{
      _ft->setIndex(_phod);
      return _ft->getEnergy();
    }
 
    short getSector() const final{return 0;} 
    
    private:

    //   trck_ptr _track;//FT tracker ?

    short _pcal=-1;
    short _phod=-1;
  };
  using region_ft_ptr=clas12::region_ft*;
  using region_ft_uptr=std::unique_ptr<clas12::region_ft>;

}

#endif /* REGION_FT_H */
