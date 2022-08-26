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
  * File:   datastream.cpp
  * Author: gavalian
  *
  * Created on May 5, 2020, 9:24 PM
  */

#include "datastream.h"

#include <cstdlib>

namespace hipo {
 /**
  * The constructor for reader, printWarning routine
  * will printout a warning message if the library
  * was not compiled with compression libraries LZ4 or GZIP
  */
  datastreamXrootd::datastreamXrootd(){

  }

  /*datastreamXrootd::datastreamXrootd(const char *address){

  }*/
  /**
   * Default destructor. Does nothing
   */
  datastreamXrootd::~datastreamXrootd(){
    #ifdef __XROOTD__
     if(cli!=NULL) delete cli;
    #endif
  }


  void  datastreamXrootd::open(const char *filename){
#ifdef __XROOTD__
      printf("[datastream::xrootd] >>> open : %s\n" + filename);
      XrdClient *cli = new new XrdClient(filename);
      cli->Open(open_mode,open_opts);
#endif
  }

  long   datastreamXrootd::size(){
    #ifdef __XROOTD__
     XrdClientStatInfo stats;
     cli->Stat(&stats);
     return stats.size();
     #endif
     #ifndef __XROOTD__
      return 0;
     #endif
  }

  long   datastreamXrootd::position(){ return streamPosition;}
  long   datastreamXrootd::position(long pos){ streamPosition = pos; return streamPosition;}
  int datastreamXrootd::read(char *s, int size){
      #ifdef __XROOTD__
       cli->Read(s,streamPosition,size);
       return size;
      #endif
      #ifndef __XROOTD__
       return 0;
      #endif
  }
}
