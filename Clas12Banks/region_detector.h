/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   region_detector.h
 * Author: dglazier
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef REGION_DETECTOR_H
#define REGION_DETECTOR_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "clas12defs.h"
#include "bank.h"


namespace clas12 {

  class region_detector : public hipo::bank {


  public:


    region_detector()=default;
    //    virtual ~region_detector()=default;

    clas12::calorimeter  Thick();
    clas12::scintillator Thin(); 
    clas12::cherenkov    Track();
                         PID();
    //individual detector components
    clas12::calorimeter   _calorimeter;
    clas12::scintillator   _tof;
    clas12::cherenkov   _cherenkov;
    clas12::forwardtagger   _ft;
    clas12::tracker   _track;

     
  };

}

#endif /* REGION_DETECTOR_H */
