/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   detector.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef DETECTOR_H
#define DETECTOR_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include <map>
#include <hipo4/bank.h>


namespace clas12 {

  class detectorHit {
    public:
        int   detector;
        int   layer;
       double time;
       double energy;
       double path;
       double x,y,z;

       detectorHit(){};
       ~detectorHit(){};

       void show();
  };

  class detector : public hipo::bank {

  private:

    int detector_id_order;
    int   layer_order;
    int  energy_order;
    int  pindex_order;
    int    path_order;
    int    time_order;
    int       x_order;
    int       y_order;
    int       z_order;

    std::map<int,int> rmap;

  public:


    detector(){};

    detector(const char *bankName, hipo::reader &r) : hipo::bank(bankName,r){
       detector_id_order = getEntryOrder("detector");
       layer_order   = getEntryOrder("layer");
       energy_order  = getEntryOrder("energy");
       path_order    = getEntryOrder("path");
       time_order    = getEntryOrder("time");
       pindex_order  = getEntryOrder("pindex");
       x_order  = getEntryOrder("x");
       y_order  = getEntryOrder("y");
       z_order  = getEntryOrder("z");
    }

    ~detector();

    double   getEnergy(int detector, int layer, int pindex);
    double   getTime(int detector, int layer, int pindex);
    double   getPath(int detector, int layer, int pindex);


    void   init(const char *bankName, hipo::reader &r);
    void   scanIndex();

    int    getDetector(int index) { return getInt(detector_id_order,index);}
    int    getLayer(int index) { return getInt(layer_order,index);}
    int    getIndex(int index) { return getInt(pindex_order,index);}
    float  getPath(int index) { return getFloat(path_order,index);}
    float  getTime(int index) { return getFloat(time_order,index);}
    float  getEnergy(int index) { return getFloat(energy_order,index);}
    float  getX(int index) { return getFloat(x_order,index);}
    float  getY(int index) { return getFloat(y_order,index);}
    float  getZ(int index) { return getFloat(z_order,index);}


    void   getDetectorHit(int detector, int layer, int index, detectorHit &hit);
    /**
    * this method is called everytime the reader reads next event.
    * used for indexing the detector response array for particle
    * detector hit retrieval.
    */
    void notify(){ scanIndex(); }
  };

}

#endif /* UTILS_H */
