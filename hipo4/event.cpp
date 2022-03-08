//******************************************************************************
//*       ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗                  *
//*       ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗                 *
//*       ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║                 *
//*       ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║                 *
//*       ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝                 *
//*       ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝                  *
//************************ Jefferson National Lab (2017) ***********************
/*
 *   Copyright (c) 2017.  Jefferson Lab (JLab). All rights reserved. Permission
 *   to use, copy, modify, and distribute  this software and its documentation
 *   for educational, research, and not-for-profit purposes, without fee and
 *   without a signed licensing agreement.
 *
 *   IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL
 *   INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 *   OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS
 *   BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *   JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. THE HIPO DATA FORMAT SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
 *   ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO
 *   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 *   This software was developed under the United States Government license.
 *   For more information contact author at gavalian@jlab.org
 *   Department of Experimental Nuclear Physics, Jefferson Lab.
 */
 /*
  * File:   event.h
  * Author: gavalian
  *
  * Created on April 12, 2017, 10:14 AM
  */

#include "event.h"

namespace hipo {


    event::event(){
        #if __cplusplus > 199711L
            //printf("\n*****>>>>> compiled with c++11 support.\n");
        #endif
        // default allocation size for the event is 128 Kb
        dataBuffer.resize(128*1024);
        reset();
    }

    event::event(int size){
        dataBuffer.resize(size);
        reset();
    }

    event::~event()= default;

    void   event::getStructure(hipo::bank &b){
        getStructure(b,b.getSchema().getGroup(),b.getSchema().getItem());
    }

    void   event::read(hipo::bank &b){
        getStructure(b,b.getSchema().getGroup(),b.getSchema().getItem());
    }


    void event::getStructure(hipo::structure &str, int group, int item){
       std::pair<int,int> index = getStructurePosition(group,item);
       if(index.first>0){
         str.init(&dataBuffer[index.first], index.second + 8);
         str.notify();
       } else {
         str.initStructureBySize(group,item,1,0);
         str.notify();
         //printf("*** error *** : structure (%5d,%5d) does not exist\n", group,item);
       }
    }

    void     event::getStructureNoCopy(const char *buffer, hipo::structure &str, int group, int item){
       std::pair<int,int> index = getStructurePosition(buffer,group,item);
       if(index.first>0){
         str.initNoCopy(&buffer[index.first], index.second + 8);
         str.notify();
       } else {
         str.initStructureBySize(group,item,1,0);
         str.notify();
         //printf("*** error *** : structure (%5d,%5d) does not exist\n", group,item);
       }
    }
    void     event::getStructure(const char *buffer, hipo::structure &str, int group, int item){
       std::pair<int,int> index = getStructurePosition(buffer,group,item);
       if(index.first>0){
         str.init(&buffer[index.first], index.second + 8);
         str.notify();
       } else {
         str.initStructureBySize(group,item,1,0);
         str.notify();
         //printf("*** error *** : structure (%5d,%5d) does not exist\n", group,item);
       }
    }
    void    event::addStructure(hipo::structure &str){

        int str_size = str.getStructureBufferSize();
        int evt_size = getSize();
	      int evt_capacity = dataBuffer.size();

        //if(dataBuffer.size()<= () ){
        //  dataBuffer.resize(size+1024);
        //}
	      if((evt_size + str_size)<evt_capacity){
	         memcpy(&dataBuffer[evt_size], &str.getStructureBuffer()[0],str_size);
	          *(reinterpret_cast<uint32_t*>(&dataBuffer[4])) = (evt_size + str_size);
	         } else {
	            printf("event::add : error adding structure with size = %5d (capacity = %5d, size = %5d)\n",
		            str_size,evt_capacity, evt_size);
	         }
    }

    int  event::getTag(){
      int eventTag = *(reinterpret_cast<const uint32_t*>(&dataBuffer[8]));
      return eventTag;
    }

    void event::init(std::vector<char> &buffer){
        dataBuffer.resize(buffer.size());
        std::memcpy(&dataBuffer[0],&buffer[0],buffer.size());
    }

    std::pair<int,int>  event::getStructurePosition(const char *buffer, int group, int item){
      int position = 16;
      int eventSize = *(reinterpret_cast<const uint32_t*>(&buffer[4]));
      while(position+8<eventSize){
          uint16_t   gid = *(reinterpret_cast<const uint16_t*> (&buffer[position  ]));
          uint8_t    iid = *(reinterpret_cast<const uint8_t*>  (&buffer[position+2]));
          uint8_t   type = *(reinterpret_cast<const uint8_t*>  (&buffer[position+3]));
          int     length = *(reinterpret_cast<const int*>      (&buffer[position+4]));
          //printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
          if(gid==group&&iid==item) return std::make_pair(position,length);
          position += (length + 8);
      }
      return std::make_pair(-1,0);
    }

    std::pair<int,int>  event::getStructurePosition(int group, int item){
      int position = 16;
      int eventSize = *(reinterpret_cast<uint32_t*>(&dataBuffer[4]));
      while(position+8<eventSize){
          uint16_t   gid = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
          uint8_t    iid = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+2]));
          uint8_t   type = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+3]));
          int     length = *(reinterpret_cast<int*>(&dataBuffer[position+4]));
          //printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
          if(gid==group&&iid==item) return std::make_pair(position,length);
          position += (length + 8);
      }
      return std::make_pair(-1,0);
    }

    void event::init(const char *buffer, int size){
       if(dataBuffer.size()<=size){
         dataBuffer.resize(size+1024);
       }
       std::memcpy(&dataBuffer[0],buffer,size);
       *(reinterpret_cast<uint32_t*>(&dataBuffer[4])) = size;
    }

    int event::getSize(){
      return *(reinterpret_cast<uint32_t*>(&dataBuffer[4]));
    }

    void event::reset(){
        dataBuffer[0] = 'E'; dataBuffer[1] = 'V';
        dataBuffer[2] = 'N'; dataBuffer[3] = 'T';
        *(reinterpret_cast<uint32_t*>(&dataBuffer[ 4])) = 16;
        *(reinterpret_cast<uint32_t*>(&dataBuffer[ 8])) = 0;
        *(reinterpret_cast<uint32_t*>(&dataBuffer[12])) = 0;
    }
    std::vector<char> &event::getEventBuffer(){ return dataBuffer;}
    /*
    template<class T>   node<T> event::getNode(){
        node<T> en;
        en.setLength(4);
        en.setAddress(NULL);
    } */
    void event::show(){
        printf(" EVENT  SIZE = %d\n",getSize());
        int position = 16;
        int eventSize = *(reinterpret_cast<uint32_t*>(&dataBuffer[4]));
        while(position+8<eventSize){
            uint16_t   gid = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
            uint8_t    iid = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+2]));
            uint8_t   type = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+3]));
            int     length = *(reinterpret_cast<int*>(&dataBuffer[position+4]));
            //printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
            //if(gid==group&&iid==item) return std::make_pair(position,length);
            printf("%12s %9d %4d %12d %12d\n"," ",gid,iid,type,length);
            position += (length + 8);
        }
    }
}
