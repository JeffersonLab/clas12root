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

#ifndef HIPO_EVENT_H
#define HIPO_EVENT_H

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include "bank.h"

// if the library is compiled with C++11
// support we will use unordered map which
// is faster than standard map
#if __cplusplus > 199711L
#include <unordered_map>
#endif


namespace hipo {

  //typedef std::auto_ptr<hipo::generic_node> node_pointer;

  class event {

    private:
        std::vector<char> dataBuffer;
    public:

        event();
        event(int size);
        virtual ~event();

        void   show();
        void   init(std::vector<char> &buffer);
        void   init(const char *buffer, int size);
        void   getStructure(hipo::structure &str, int group, int item);
        int    getTag();
        void   getStructure(hipo::bank &b);
        void   read(hipo::bank &b);
        void   addStructure(hipo::structure &str);

        std::pair<int,int>  getStructurePosition(int group, int item);

        std::vector<char>  &getEventBuffer();
        int                 getSize();
        void                reset();

        //*******************************************************************
        //** static methods for reading structures from event structure
        //** from the memory. It does not have to copy event into separate
        //** buffer.
        //*******************************************************************
        static std::pair<int,int>
              getStructurePosition(const char *buffer, int group, int item);
        static void
              getStructure(const char *buffer, hipo::structure &str, int group, int item);
        static void
              getStructureNoCopy(const char *buffer, hipo::structure &str, int group, int item);
    };
}

#endif /* EVENT_H */
