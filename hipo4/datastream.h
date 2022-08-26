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
 * This class is generic class that is suppose to provide a data stream.
 * it is implemented as a test case for xrootd, functionality to switch
 * between the regular file and files provided by xrootd server.
 * File:   datastream.h
 * Author: gavalian
 *
 * Created on May 5, 2020, 9:18 PM
 */

#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <climits>

#ifdef __XROOTD__
#include <XrdClient/XrdClient.hh>
#include "XrdClient/XrdClientEnv.hh"
#include "XrdSys/XrdSysHeaders.hh"
#endif

namespace hipo {


class datastream {

    private:

       std::ifstream     inputStream;
       std::string       remoteAddress;
       int               streamType = 1;


    public:

        datastream();
        //datastream(const char *address);
        virtual ~datastream();

        virtual long   size(){ return 0;}
        virtual long   position(){ return 0;}
        virtual long   position(long pos){ return 0;}
        //virtual void  setAdress(const char *address){}
        virtual void  open(const char *filename){}
        virtual int   read(char *s, int size){return 0;}
  };

class datastreamLocalFile {

  private:
    std::ifstream     inputStream;

  public:

      datastreamLocalFile();
      ~datastreamLocalFile(){
        if(inputStream.is_open()==true){
          inputStream.close();
        }
      }

      long   size(){
          long pos = inputStream.tellg();
          inputStream.seekg(0,std::ios_base::end);
          long lastpos = inputStream.tellg();
          inputStream.seekg(pos,std::ios_base::beg);
          return lastpos;
        }

      void   open(const char *filename){
        inputStream.open(filename, std::ios::binary);
      }
      long   position(){ return inputStream.tellg();}
      long   position(long pos){ inputStream.seekg(pos,std::ios_base::beg); return pos; }
      int    read(char *s, int size){inputStream.read(s,size); return size;}
};

class datastreamXrootd {

private:
  #ifdef __XROOTD__
          kXR_unt16 open_mode = (kXR_ur);
          kXR_unt16 open_opts = (1);
          XrdClient *cli = NULL;
  #endif
  long streamPosition = 0;

public:
  datastreamXrootd();
  ~datastreamXrootd();


   long   size();
   long   position();
   long   position(long pos);
  //virtual void  setAdress(const char *address){}
   void  open(const char *filename);
   int   read(char *s, int size);
};
}
#endif /* DATASTREAM_H */
