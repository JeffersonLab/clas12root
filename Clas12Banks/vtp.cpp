/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "vtp.h"

namespace clas12 {


  vtp::vtp(hipo::schema __schema): hipo::bank(__schema) {
    
    auto sch=getSchema();
    cr_order = sch.getEntryOrder("crate");
    wo_order = sch.getEntryOrder("word");
   }

  long vtp::makeVTPTriggers() {

    _VTPBitSet.reset();
    int nVTPTriggers = 0;
  
    int nrows1 = 0;
  
    int crate= 0;
    int nwords= 0;
    int word1VTP, word2VTP, word3VTP= 0;
    const int trig2VTP=100;
  
    int Nentries=getSize();
    int loop1=0;
    while (loop1<Nentries) {

      crate = getCrate(loop1);
    
      if (crate == trig2VTP) {
	nwords = getWord(loop1);
	loop1++;
	if (nwords == 4)
	  nVTPTriggers = 0;
	else
	  nVTPTriggers = (nwords - 4) / 2;
	// decode them                                        
	loop1+=4;
	for (int loop2 = 0; loop2 < nVTPTriggers; loop2++) {
	  word1VTP = getWord(loop1++);
	  if (((word1VTP >> 27) & 0x1F) == 0x1D) {
	    word2VTP = getWord(loop1++);
	    decodeVTPTrigger(word1VTP, word2VTP);
	  }
	}
      } 
      else {
	loop1++; 
      }
    }
    return _VTPBitSet.to_ulong();
  }

  void  vtp::decodeVTPTrigger(int word1vtp, int word2vtp) {
    int time, trgL, trgH;
    time = (word1vtp >> 16) & 0x7FF; // 11 bits time
    trgL = (word1vtp & 0xFFFF); // 16 bits
    trgH = (word2vtp & 0xFFFF); // 16 bits
    long pattern=(trgL&0xFFFF)|((trgH<<16)&0xFFFF0000);
    addVTPTriggerToEvent(pattern);
  }

  void  vtp::addVTPTriggerToEvent(long pattern){
    //Add this pattern to this event
    //Eaxh pattern only contains 1 sector
    //here we make a new bit pattern containing all
    //set the bits that are =1
    const int bitsize= _VTPBitSet.size();
    //cout<<"pattern "<<pattern<<endl;
    for(int ib=0;ib<bitsize;ib++){
      short is1=(pattern& (1<<ib)) != 0;
      if(is1)
	_VTPBitSet.set(ib,1);
    }
  }

  
}
