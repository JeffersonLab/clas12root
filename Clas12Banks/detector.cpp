/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "detector.h"


namespace clas12 {

  detector::~detector(){}

void   detector::init(const char *bankName, hipo::reader &r){
  initBranches(bankName,r);
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

void   detector::scanIndex(){
    rmap.clear();
    int size = getSize();
    for(int i = 0; i < size; i++){
        int detector = getDetector(i);
        int    layer = getLayer(i);
        int pindex   = getIndex(i);
        int key = (detector<<16)|(layer<<8)|pindex;
        rmap[key] = i;
    }
  }

  double   detector::getTime(int detector, int layer, int pindex){
    int key = (detector<<16)|(layer<<8)|pindex;
    if(rmap.count(key)>0) {
        int position = rmap[key];
        return getTime(position);
    }
    return 0.0;
  }

  double   detector::getEnergy(int detector, int layer, int pindex){
    int key = (detector<<16)|(layer<<8)|pindex;
    if(rmap.count(key)>0) {
        int position = rmap[key];
        return getEnergy(position);
    }
    return 0.0;
  }

  double   detector::getPath(int detector, int layer, int pindex){
    int key = (detector<<16)|(layer<<8)|pindex;
    if(rmap.count(key)>0) {
        int position = rmap[key];
        return getPath(position);
    }
    return 0.0;
  }

  void   detector::getDetectorHit(int detector, int layer, int pindex, detectorHit &hit){
      int key = (detector<<16)|(layer<<8)|pindex;
      if(rmap.count(key)>0){
          int position = rmap[key];
          hit.x = getX(position);
          hit.y = getY(position);
          hit.z = getZ(position);
          hit.energy = getEnergy(position);
          hit.time   = getTime(position);
          hit.path   = getPath(position);
          hit.detector = getDetector(position);
          hit.layer    = getLayer(position);
      } else {
        hit.detector = 0;
        hit.layer    = 0;
        hit.time     = 0.0;
        hit.energy   = 0.0;
        hit.path     = 0.0;
        hit.x = hit.y = hit.z = 0;
      }
  }


  void detectorHit::show(){
      printf(" d : %3d time = %8.3f, energy = %8.3f, xyz = %8.3f %8.3f %8.3f\n",
       detector,time,energy,x,y,z);
  }
}
