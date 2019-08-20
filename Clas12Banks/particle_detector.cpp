/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "particle_detector.h"
#include <sstream>
#include <algorithm>

namespace clas12 {

  particle_detector::particle_detector(hipo::schema __schema) : hipo::bank( __schema) {

    auto sch=getSchema();
    //base detector need detector and pindex items
    _detector_id_order = sch.getEntryOrder("detector");
    _pindex_order  = sch.getEntryOrder("pindex");
  }
  ///////////////////////////////////////////////////////////////////////
  /// load items to be used
  void particle_detector::readItems(std::string items){
    std::stringstream ss(items);
    std::string sitem;
    while(std::getline(ss, sitem, ':'))
      _sitems.push_back(sitem);
  }
  ///////////////////////////////////////////////////////////////////////
  ///check if item should be used
  bool particle_detector::useItem(std::string item){
    if(!(_sitems.size())) return true;
    if(std::find(_sitems.begin(),_sitems.end(),"c")!=_sitems.end())
      return true;
    return false;
  }
  
  //  function to map the detector entry to particle index
  // void   particle_detector::scanIndex(){
  //   _rmap.clear();
    
  //   const int size = getSize();
  //   for(int i = 0; i < size; i++){
  //     int detector = getDetector(i);
  //     int layer= getLayer(i);
  //     int pindex   = getPindex(i);
  //     int key = (detector<<16)|(layer<<8)|pindex;
  //      _rmap[key] = i;
  //   }
  // }
  ////////////////////////////////////////////////////////////////////////
  ///function to find the current entries associated
  ///with pindex = iparticle
  std::vector<short >  particle_detector::scanForParticle(short iparticle){
    const int size = getSize();
    std::vector<short > pindices(0);
    for(short i = 0; i < size; i++){
      int pindex   = getPindex(i);
      if(iparticle==pindex){
	int detector = getDetector(i);
	int  layer = getLayer(i);
	int key = (detector<<16)|(layer<<8)|pindex;
	pindices.push_back(i);
      }
    }
    return pindices;
  }


 
  void particle_detector::print(){
    const int size=getSize();
    std::cout<<"Print detector with "<<size<< "entries \n";
    for(int i=0;i<size;i++)
      std::cout<<getDetector(i)<<" "<<getPindex(i)<<"\n";
    std::cout<<"\n";
  }


  int particle_detector::getIndex(int pindex, int detector, int layer){
 
    std::vector<int>::iterator it;
     int key = (detector<<16)|(layer<<8)|pindex;
     if((it=std::find(_rvec.begin(),_rvec.end(),key))!=_rvec.end()){
      _index = std::distance(_rvec.begin(), it);
      return _index;
    }
    return _index=-1;
  }
   void  particle_detector::scanIndex(){
     _rvec.clear();
     const int size = getRows();
     _rvec.reserve(size);
     for(int i = 0; i < size; i++){
       int detector = getDetector(i);
       int layer= getLayer(i);
       int pindex   = getPindex(i);
       int key = (detector<<16)|(layer<<8)|pindex;
       _rvec.emplace_back(key);
     }
   }
  
}
  
