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
  * Created on August 15, 2022, 17:04 PM
  */

#include "tuple.h"

namespace hipo {


    tuple::tuple(){

    }

    tuple::tuple(const char *format){
    }

    tuple::~tuple()= default;

   void tuple::initBranches(int size){
        for(int i = 0; i < size; i++) branches.push_back(new hipo::structure());
        for(int i = 0; i < branches.size(); i++) branches[i]->initStructureBySize(120,1,4,800);
        currentPosition = 0;
   }

   void tuple::fill(const float *array){
      for(int i = 0; i < branches.size(); i++) branches[i]->putFloatAt(currentPosition*4,array[i]);
      currentPosition++;
      if(currentPosition>=200){
        writeAndUpdate(); currentPosition = 0;
      }
   }

   void tuple::open(const char *file){
      writer.open(file);
   }

   void tuple::close(){
      writer.close();
   }

   void tuple::writeAndUpdate(){
       for(int i = 0; i < branches.size(); i++){
         event.reset();
         event.setTag(i+1);
         event.addStructure(*branches[i]);
         writer.addEvent(event);
       }
   }
}
