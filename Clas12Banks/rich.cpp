/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "rich.h"


namespace clas12 {

 
  rich::rich(hipo::schema aschema,hipo::schema ringschema):
    clas12::particle_detector(aschema),
    _ring{new richring(ringschema)}
  {
    // std::cout<<"DEBUG "<<"rich::rich()"<< std::endl;
    //aschema.show();
    _detector_id_order=0; //dont have a detector bank, but getDetector is fixed
   _id_order   = aschema.getEntryOrder("id");
   _hindex_order   = aschema.getEntryOrder("hindex");
   _pindex_order   = aschema.getEntryOrder("pindex");
   _emilay_order   = aschema.getEntryOrder("emilay");
   _emico_order   = aschema.getEntryOrder("emico");
   _enico_order   = aschema.getEntryOrder("enico");
   _emqua_order   = aschema.getEntryOrder("emqua");
   _mchi2_order   = aschema.getEntryOrder("mchi2");
   _mass_order   = aschema.getEntryOrder("mass");
   _best_PID_order   = aschema.getEntryOrder("best_PID");
   _RQ_order   = aschema.getEntryOrder("RQ");
   _ReQ_order   = aschema.getEntryOrder("ReQ");
   _el_logl_order   = aschema.getEntryOrder("el_logl");
   _pi_logl_order   = aschema.getEntryOrder("pi_logl");
   _k_logl_order   = aschema.getEntryOrder("k_logl");
   _pr_logl_order   = aschema.getEntryOrder("pr_logl");
   _best_ch_order   = aschema.getEntryOrder("best_ch");
   _best_c2_order   = aschema.getEntryOrder("best_c2");
   _best_RL_order   = aschema.getEntryOrder("best_RL");
   _best_ntot_order   = aschema.getEntryOrder("best_ntot");
   _best_mass_order   = aschema.getEntryOrder("best_mass");
    
  }
  rich::rich(hipo::schema aschema):
    clas12::particle_detector(aschema)  {
    
    _detector_id_order=0; //dont have a detector bank, but getDetector is fixed
    _id_order   = aschema.getEntryOrder("id");
    _hindex_order   = aschema.getEntryOrder("hindex");
    _pindex_order   = aschema.getEntryOrder("pindex");
   _emilay_order   = aschema.getEntryOrder("emilay");
   _emico_order   = aschema.getEntryOrder("emico");
   _enico_order   = aschema.getEntryOrder("enico");
   _emqua_order   = aschema.getEntryOrder("emqua");
   _mchi2_order   = aschema.getEntryOrder("mchi2");
   _mass_order   = aschema.getEntryOrder("mass");
   _best_PID_order   = aschema.getEntryOrder("best_PID");
   _RQ_order   = aschema.getEntryOrder("RQ");
   _ReQ_order   = aschema.getEntryOrder("ReQ");
   _el_logl_order   = aschema.getEntryOrder("el_logl");
   _pi_logl_order   = aschema.getEntryOrder("pi_logl");
   _k_logl_order   = aschema.getEntryOrder("k_logl");
   _pr_logl_order   = aschema.getEntryOrder("pr_logl");
   _best_ch_order   = aschema.getEntryOrder("best_ch");
   _best_c2_order   = aschema.getEntryOrder("best_c2");
   _best_RL_order   = aschema.getEntryOrder("best_RL");
   _best_ntot_order   = aschema.getEntryOrder("best_ntot");
   _best_mass_order   = aschema.getEntryOrder("best_mass");
 
  }

  ///////////////////////////////////////////////////////////////////
  richring::richring(hipo::schema aschema):hipo::bank(aschema){

   _id_order   = aschema.getEntryOrder("id");
   _hindex_order   = aschema.getEntryOrder("hindex");
   _pindex_order   = aschema.getEntryOrder("pindex");
   _sector_order   = aschema.getEntryOrder("sector");
   _pmt_order   = aschema.getEntryOrder("pmt");
   _anode_order   = aschema.getEntryOrder("anode");
   _hypo_order   = aschema.getEntryOrder("hypo");
   _dtime_order   = aschema.getEntryOrder("dtime");
   _etaC_order   = aschema.getEntryOrder("etaC");
   _prob_order   = aschema.getEntryOrder("prob");
   _use_order   = aschema.getEntryOrder("use");
   _dangle_order   = aschema.getEntryOrder("dangle");
   _layers_order   = aschema.getEntryOrder("layers");
   _compos_order   = aschema.getEntryOrder("compos");
 
  }


}
