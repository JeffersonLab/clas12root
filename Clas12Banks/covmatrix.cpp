/*
 * To change this license covmatrix, choose License Covmatrixs in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "covmatrix.h"
#include <iostream>
#include <algorithm>

namespace clas12 {

 
  covmatrix::covmatrix(hipo::schema __schema): hipo::bank(__schema) {
    _morder[0][0] = __schema.getEntryOrder("C11");
    _morder[0][1] = __schema.getEntryOrder("C12");
    _morder[0][2] = __schema.getEntryOrder("C13");
    _morder[0][3]  = __schema.getEntryOrder("C14");
    _morder[0][4] = __schema.getEntryOrder("C15");
    _morder[1][1]  = __schema.getEntryOrder("C22");
    _morder[1][2] = __schema.getEntryOrder("C23");
    _morder[1][3] = __schema.getEntryOrder("C24");
    _morder[1][4]  = __schema.getEntryOrder("C25");
    _morder[2][2]  = __schema.getEntryOrder("C33");
    _morder[2][3] = __schema.getEntryOrder("C34");
    _morder[2][4] = __schema.getEntryOrder("C35");
    _morder[3][3] = __schema.getEntryOrder("C44");
    _morder[3][4] = __schema.getEntryOrder("C45");
    _morder[4][4] = __schema.getEntryOrder("C55");
    _pindex_order  = __schema.getEntryOrder("pindex");
  }
  void  covmatrix::scanIndex(){
    // _rmap.clear();
   _rvec.clear();
    const int size = getRows();
    _rvec.reserve(size);
    for(int i = 0; i < size; i++){
     int pindex   = getPindex(i);
     //  _rmap[pindex] = i;
     _rvec.emplace_back(pindex);
    }
   }
  int covmatrix::getIndex(int pindex){
    //    if(_rmap.find(pindex) != _rmap.end()) {
    std::vector<int>::iterator it;
    if((it=std::find(_rvec.begin(),_rvec.end(),pindex))!=_rvec.end()){
      _index = std::distance(_rvec.begin(), it);
      //_index = _rmap[pindex];
      return _index;
    }
    return _index=-1;
  }
  const CovMatrix* covmatrix::matrix(){
    if(_index==-1){
      for(auto i=0;i<5;i++)
	for(auto j=i;j<5;j++)
	  _matrix[i][j]=0;
    }
    else{
      for(auto i=0;i<5;i++)
	for(auto j=i;j<5;j++)
	  _matrix[i][j]=getFloat(_morder[i][j],_index);
    }
    return &_matrix;
  }

  void covmatrix::print(){
    matrix();
    std::cout<<"Cov Matrix "<<std::endl;
    std::cout<<_matrix[0][0]<< " "<<_matrix[0][1]<<" "<<_matrix[0][2]<< " "<<_matrix[0][3]<<" "<<_matrix[0][4]<<std::endl;
    std::cout<<_matrix[1][0]<< " "<<_matrix[1][1]<<" "<<_matrix[1][2]<< " "<<_matrix[1][3]<<" "<<_matrix[1][4]<<std::endl;
    std::cout<<_matrix[2][0]<< " "<<_matrix[2][1]<<" "<<_matrix[2][2]<< " "<<_matrix[2][3]<<" "<<_matrix[2][4]<<std::endl;
    std::cout<<_matrix[3][0]<< " "<<_matrix[3][1]<<" "<<_matrix[3][2]<< " "<<_matrix[3][3]<<" "<<_matrix[3][4]<<std::endl;
    std::cout<<_matrix[4][0]<< " "<<_matrix[4][1]<<" "<<_matrix[4][2]<< " "<<_matrix[4][3]<<" "<<_matrix[4][4]<<std::endl;
  }
}
